// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2012 Red Hat
 *
 * based in parts on udlfb.c:
 * Copyright (C) 2009 Roberto De Ioris <roberto@unbit.it>
 * Copyright (C) 2009 Jaya Kumar <jayakumar.lkml@gmail.com>
 * Copyright (C) 2009 Bernie Thompson <bernie@plugable.com>
 */

#include <drm/drm.h>
#include <drm/drm_print.h>
#include <drm/drm_probe_helper.h>

#include "udl_drv.h"

/* -BULK_SIZE as per usb-skeleton. Can we get full page and avoid overhead? */
#define BULK_SIZE 512

#define NR_USB_REQUEST_CHANNEL 0x12

#define MAX_TRANSFER (PAGE_SIZE*16 - BULK_SIZE)
#define WRITES_IN_FLIGHT (20)
#define MAX_VENDOR_DESCRIPTOR_SIZE 256

static struct urb *udl_get_urb_locked(struct udl_device *udl, long timeout);

static int udl_parse_vendor_descriptor(struct udl_device *udl)
{
	struct usb_device *udev = udl_to_usb_device(udl);
	char *desc;
	char *buf;
	char *desc_end;

	u8 total_len = 0;

	buf = kzalloc(MAX_VENDOR_DESCRIPTOR_SIZE, GFP_KERNEL);
	if (!buf)
		return false;
	desc = buf;

	total_len = usb_get_descriptor(udev, 0x5f, /* vendor specific */
				    0, desc, MAX_VENDOR_DESCRIPTOR_SIZE);
	if (total_len > 5) {
		DRM_INFO("vendor descriptor length:%x data:%11ph\n",
			total_len, desc);

		if ((desc[0] != total_len) || /* descriptor length */
		    (desc[1] != 0x5f) ||   /* vendor descriptor type */
		    (desc[2] != 0x01) ||   /* version (2 bytes) */
		    (desc[3] != 0x00) ||
		    (desc[4] != total_len - 2)) /* length after type */
			goto unrecognized;

		desc_end = desc + total_len;
		desc += 5; /* the fixed header we've already parsed */

		while (desc < desc_end) {
			u8 length;
			u16 key;

			key = le16_to_cpu(*((u16 *) desc));
			desc += sizeof(u16);
			length = *desc;
			desc++;

			switch (key) {
			case 0x0200: { /* max_area */
				u32 max_area;
				max_area = le32_to_cpu(*((u32 *)desc));
				DRM_DEBUG("DL chip limited to %d pixel modes\n",
					max_area);
				udl->sku_pixel_limit = max_area;
				break;
			}
			default:
				break;
			}
			desc += length;
		}
	}

	goto success;

unrecognized:
	/* allow udlfb to load for now even if firmware unrecognized */
	DRM_ERROR("Unrecognized vendor firmware descriptor\n");

success:
	kfree(buf);
	return true;
}

/*
 * Need to ensure a channel is selected before submitting URBs
 */
int udl_select_std_channel(struct udl_device *udl)
{
	static const u8 set_def_chn[] = {0x57, 0xCD, 0xDC, 0xA7,
					 0x1C, 0x88, 0x5E, 0x15,
					 0x60, 0xFE, 0xC6, 0x97,
					 0x16, 0x3D, 0x47, 0xF2};

	void *sendbuf;
	int ret;
	struct usb_device *udev = udl_to_usb_device(udl);

	sendbuf = kmemdup(set_def_chn, sizeof(set_def_chn), GFP_KERNEL);
	if (!sendbuf)
		return -ENOMEM;

	ret = usb_control_msg(udev, usb_sndctrlpipe(udev, 0),
			      NR_USB_REQUEST_CHANNEL,
			      (USB_DIR_OUT | USB_TYPE_VENDOR), 0, 0,
			      sendbuf, sizeof(set_def_chn),
			      USB_CTRL_SET_TIMEOUT);
	kfree(sendbuf);
	return ret < 0 ? ret : 0;
}

void udl_urb_completion(struct urb *urb)
{
	struct urb_node *unode = urb->context;
	struct udl_device *udl = unode->dev;
	unsigned long flags;

	/* sync/async unlink faults aren't errors */
	if (urb->status) {
		if (!(urb->status == -ENOENT ||
		    urb->status == -ECONNRESET ||
		    urb->status == -EPROTO ||
		    urb->status == -ESHUTDOWN)) {
			DRM_ERROR("%s - nonzero write bulk status received: %d\n",
				__func__, urb->status);
		}
	}

	urb->transfer_buffer_length = udl->urbs.size; /* reset to actual */

	spin_lock_irqsave(&udl->urbs.lock, flags);
	list_add_tail(&unode->entry, &udl->urbs.list);
	udl->urbs.available++;
	spin_unlock_irqrestore(&udl->urbs.lock, flags);

	wake_up(&udl->urbs.sleep);
}

static void udl_free_urb_list(struct drm_device *dev)
{
	struct udl_device *udl = to_udl(dev);
	struct urb_node *unode;
	struct urb *urb;

	DRM_DEBUG("Waiting for completes and freeing all render urbs\n");

	/* keep waiting and freeing, until we've got 'em all */
	while (udl->urbs.count) {
		spin_lock_irq(&udl->urbs.lock);
		urb = udl_get_urb_locked(udl, MAX_SCHEDULE_TIMEOUT);
		udl->urbs.count--;
		spin_unlock_irq(&udl->urbs.lock);
		if (WARN_ON(!urb))
			break;
		unode = urb->context;
		/* Free each separately allocated piece */
		usb_free_coherent(urb->dev, udl->urbs.size,
				  urb->transfer_buffer, urb->transfer_dma);
		usb_free_urb(urb);
		kfree(unode);
	}

	wake_up_all(&udl->urbs.sleep);
}

static int udl_alloc_urb_list(struct drm_device *dev, int count, size_t size)
{
	struct udl_device *udl = to_udl(dev);
	struct urb *urb;
	struct urb_node *unode;
	char *buf;
	size_t wanted_size = count * size;
	struct usb_device *udev = udl_to_usb_device(udl);

	spin_lock_init(&udl->urbs.lock);
	INIT_LIST_HEAD(&udl->urbs.list);
	init_waitqueue_head(&udl->urbs.sleep);
	udl->urbs.count = 0;
	udl->urbs.available = 0;

retry:
	udl->urbs.size = size;

	while (udl->urbs.count * size < wanted_size) {
		unode = kzalloc(sizeof(struct urb_node), GFP_KERNEL);
		if (!unode)
			break;
		unode->dev = udl;

		urb = usb_alloc_urb(0, GFP_KERNEL);
		if (!urb) {
			kfree(unode);
			break;
		}
		unode->urb = urb;

		buf = usb_alloc_coherent(udev, size, GFP_KERNEL,
					 &urb->transfer_dma);
		if (!buf) {
			kfree(unode);
			usb_free_urb(urb);
			if (size > PAGE_SIZE) {
				size /= 2;
				udl_free_urb_list(dev);
				goto retry;
			}
			break;
		}

		/* urb->transfer_buffer_length set to actual before submit */
		usb_fill_bulk_urb(urb, udev, usb_sndbulkpipe(udev, 1),
				  buf, size, udl_urb_completion, unode);
		urb->transfer_flags |= URB_NO_TRANSFER_DMA_MAP;

		list_add_tail(&unode->entry, &udl->urbs.list);

		udl->urbs.count++;
		udl->urbs.available++;
	}

	DRM_DEBUG("allocated %d %d byte urbs\n", udl->urbs.count, (int) size);

	return udl->urbs.count;
}

static struct urb *udl_get_urb_locked(struct udl_device *udl, long timeout)
{
	struct urb_node *unode;

	assert_spin_locked(&udl->urbs.lock);

	/* Wait for an in-flight buffer to complete and get re-queued */
	if (!wait_event_lock_irq_timeout(udl->urbs.sleep,
					 !udl->urbs.count ||
					 !list_empty(&udl->urbs.list),
					 udl->urbs.lock, timeout)) {
		DRM_INFO("wait for urb interrupted: available: %d\n",
			 udl->urbs.available);
		return NULL;
	}

	if (!udl->urbs.count)
		return NULL;

	unode = list_first_entry(&udl->urbs.list, struct urb_node, entry);
	list_del_init(&unode->entry);
	udl->urbs.available--;

	return unode->urb;
}

#define GET_URB_TIMEOUT	HZ
struct urb *udl_get_urb(struct drm_device *dev)
{
	struct udl_device *udl = to_udl(dev);
	struct urb *urb;

	spin_lock_irq(&udl->urbs.lock);
	urb = udl_get_urb_locked(udl, GET_URB_TIMEOUT);
	spin_unlock_irq(&udl->urbs.lock);
	return urb;
}

int udl_submit_urb(struct drm_device *dev, struct urb *urb, size_t len)
{
	struct udl_device *udl = to_udl(dev);
	int ret;

	if (WARN_ON(len > udl->urbs.size)) {
		ret = -EINVAL;
		goto error;
	}
	urb->transfer_buffer_length = len; /* set to actual payload len */
	ret = usb_submit_urb(urb, GFP_ATOMIC);
 error:
	if (ret) {
		udl_urb_completion(urb); /* because no one else will */
		DRM_ERROR("usb_submit_urb error %x\n", ret);
	}
	return ret;
}

/* wait until all pending URBs have been processed */
void udl_sync_pending_urbs(struct drm_device *dev)
{
	struct udl_device *udl = to_udl(dev);

	spin_lock_irq(&udl->urbs.lock);
	/* 2 seconds as a sane timeout */
	if (!wait_event_lock_irq_timeout(udl->urbs.sleep,
					 udl->urbs.available == udl->urbs.count,
					 udl->urbs.lock,
					 msecs_to_jiffies(2000)))
		drm_err(dev, "Timeout for syncing pending URBs\n");
	spin_unlock_irq(&udl->urbs.lock);
}

int udl_init(struct udl_device *udl)
{
	struct drm_device *dev = &udl->drm;
	int ret = -ENOMEM;
	struct device *dma_dev;

	DRM_DEBUG("\n");

	dma_dev = usb_intf_get_dma_device(to_usb_interface(dev->dev));
	if (dma_dev) {
		drm_dev_set_dma_dev(dev, dma_dev);
		put_device(dma_dev);
	} else {
		drm_warn(dev, "buffer sharing not supported"); /* not an error */
	}

	mutex_init(&udl->gem_lock);

	if (!udl_parse_vendor_descriptor(udl)) {
		ret = -ENODEV;
		DRM_ERROR("firmware not recognized. Assume incompatible device\n");
		goto err;
	}

	if (udl_select_std_channel(udl))
		DRM_ERROR("Selecting channel failed\n");

	if (!udl_alloc_urb_list(dev, WRITES_IN_FLIGHT, MAX_TRANSFER)) {
		DRM_ERROR("udl_alloc_urb_list failed\n");
		goto err;
	}

	DRM_DEBUG("\n");
	ret = udl_modeset_init(dev);
	if (ret)
		goto err;

	return 0;

err:
	if (udl->urbs.count)
		udl_free_urb_list(dev);
	DRM_ERROR("%d\n", ret);
	return ret;
}

int udl_drop_usb(struct drm_device *dev)
{
	udl_free_urb_list(dev);

	return 0;
}
