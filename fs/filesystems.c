// SPDX-License-Identifier: GPL-2.0
/*
 *  linux/fs/filesystems.c
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 *
 *  table of configured filesystems
 */

#include <linux/syscalls.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/kmod.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/fs_parser.h>

/*
 * Handling of filesystem drivers list.
 * Rules:
 *	Inclusion to/removals from/scanning of list are protected by spinlock.
 *	During the unload module must call unregister_filesystem().
 *	We can access the fields of list element if:
 *		1) spinlock is held or
 *		2) we hold the reference to the module.
 *	The latter can be guaranteed by call of try_module_get(); if it
 *	returned 0 we must skip the element, otherwise we got the reference.
 *	Once the reference is obtained we can drop the spinlock.
 */

static struct file_system_type *file_systems;
static DEFINE_RWLOCK(file_systems_lock);

#ifdef CONFIG_PROC_FS
static unsigned long file_systems_gen;

struct file_systems_string {
	struct rcu_head rcufree;
	unsigned long gen;
	size_t len;
	char string[];
};
static struct file_systems_string *file_systems_string;
static void invalidate_filesystems_string(void);
#else
static void invalidate_filesystems_string(void)
{
}
#endif

/* WARNING: This can be used only if we _already_ own a reference */
struct file_system_type *get_filesystem(struct file_system_type *fs)
{
	__module_get(fs->owner);
	return fs;
}

void put_filesystem(struct file_system_type *fs)
{
	module_put(fs->owner);
}

static struct file_system_type **find_filesystem(const char *name, unsigned len)
{
	struct file_system_type **p;
	for (p = &file_systems; *p; p = &(*p)->next)
		if (strncmp((*p)->name, name, len) == 0 &&
		    !(*p)->name[len])
			break;
	return p;
}

/**
 *	register_filesystem - register a new filesystem
 *	@fs: the file system structure
 *
 *	Adds the file system passed to the list of file systems the kernel
 *	is aware of for mount and other syscalls. Returns 0 on success,
 *	or a negative errno code on an error.
 *
 *	The &struct file_system_type that is passed is linked into the kernel 
 *	structures and must not be freed until the file system has been
 *	unregistered.
 */
 
int register_filesystem(struct file_system_type * fs)
{
	int res = 0;
	struct file_system_type ** p;

	if (fs->parameters &&
	    !fs_validate_description(fs->name, fs->parameters))
		return -EINVAL;

	BUG_ON(strchr(fs->name, '.'));
	if (fs->next)
		return -EBUSY;
	write_lock(&file_systems_lock);
	p = find_filesystem(fs->name, strlen(fs->name));
	if (*p) {
		res = -EBUSY;
	} else {
		*p = fs;
		invalidate_filesystems_string();
	}
	write_unlock(&file_systems_lock);
	return res;
}

EXPORT_SYMBOL(register_filesystem);

/**
 *	unregister_filesystem - unregister a file system
 *	@fs: filesystem to unregister
 *
 *	Remove a file system that was previously successfully registered
 *	with the kernel. An error is returned if the file system is not found.
 *	Zero is returned on a success.
 *	
 *	Once this function has returned the &struct file_system_type structure
 *	may be freed or reused.
 */
 
int unregister_filesystem(struct file_system_type * fs)
{
	struct file_system_type ** tmp;

	write_lock(&file_systems_lock);
	tmp = &file_systems;
	while (*tmp) {
		if (fs == *tmp) {
			*tmp = fs->next;
			fs->next = NULL;
			invalidate_filesystems_string();
			write_unlock(&file_systems_lock);
			synchronize_rcu();
			return 0;
		}
		tmp = &(*tmp)->next;
	}
	write_unlock(&file_systems_lock);

	return -EINVAL;
}

EXPORT_SYMBOL(unregister_filesystem);

#ifdef CONFIG_SYSFS_SYSCALL
static int fs_index(const char __user * __name)
{
	struct file_system_type * tmp;
	struct filename *name;
	int err, index;

	name = getname(__name);
	err = PTR_ERR(name);
	if (IS_ERR(name))
		return err;

	err = -EINVAL;
	read_lock(&file_systems_lock);
	for (tmp=file_systems, index=0 ; tmp ; tmp=tmp->next, index++) {
		if (strcmp(tmp->name, name->name) == 0) {
			err = index;
			break;
		}
	}
	read_unlock(&file_systems_lock);
	putname(name);
	return err;
}

static int fs_name(unsigned int index, char __user * buf)
{
	struct file_system_type * tmp;
	int len, res;

	read_lock(&file_systems_lock);
	for (tmp = file_systems; tmp; tmp = tmp->next, index--)
		if (index <= 0 && try_module_get(tmp->owner))
			break;
	read_unlock(&file_systems_lock);
	if (!tmp)
		return -EINVAL;

	/* OK, we got the reference, so we can safely block */
	len = strlen(tmp->name) + 1;
	res = copy_to_user(buf, tmp->name, len) ? -EFAULT : 0;
	put_filesystem(tmp);
	return res;
}

static int fs_maxindex(void)
{
	struct file_system_type * tmp;
	int index;

	read_lock(&file_systems_lock);
	for (tmp = file_systems, index = 0 ; tmp ; tmp = tmp->next, index++)
		;
	read_unlock(&file_systems_lock);
	return index;
}

/*
 * Whee.. Weird sysv syscall. 
 */
SYSCALL_DEFINE3(sysfs, int, option, unsigned long, arg1, unsigned long, arg2)
{
	int retval = -EINVAL;

	switch (option) {
		case 1:
			retval = fs_index((const char __user *) arg1);
			break;

		case 2:
			retval = fs_name(arg1, (char __user *) arg2);
			break;

		case 3:
			retval = fs_maxindex();
			break;
	}
	return retval;
}
#endif

int __init list_bdev_fs_names(char *buf, size_t size)
{
	struct file_system_type *p;
	size_t len;
	int count = 0;

	read_lock(&file_systems_lock);
	for (p = file_systems; p; p = p->next) {
		if (!(p->fs_flags & FS_REQUIRES_DEV))
			continue;
		len = strlen(p->name) + 1;
		if (len > size) {
			pr_warn("%s: truncating file system list\n", __func__);
			break;
		}
		memcpy(buf, p->name, len);
		buf += len;
		size -= len;
		count++;
	}
	read_unlock(&file_systems_lock);
	return count;
}

#ifdef CONFIG_PROC_FS
/*
 * The fs list gets queried a lot by userspace, including rather surprising
 * programs (would you guess *sed* is on the list?). In order to reduce the
 * overhead we cache the resulting string, which normally hangs around below
 * 512 bytes in size.
 *
 * As the list almost never changes, its creation is not particularly optimized
 * for simplicity.
 *
 * We sort it out on read in order to not introduce a failure point for fs
 * registration (in principle we may be unable to alloc memory for the list).
 */
static void invalidate_filesystems_string(void)
{
	struct file_systems_string *fss;

	lockdep_assert_held_write(&file_systems_lock);
	file_systems_gen++;
	fss = file_systems_string;
	WRITE_ONCE(file_systems_string, NULL);
	kfree_rcu(fss, rcufree);
}

static noinline int regen_filesystems_string(void)
{
	struct file_system_type *tmp;
	struct file_systems_string *old, *new;
	size_t newlen, usedlen;
	unsigned long gen;

retry:
	lockdep_assert_not_held(&file_systems_lock);

	newlen = 0;
	write_lock(&file_systems_lock);
	gen = file_systems_gen;
	tmp = file_systems;
	/* pre-calc space for "%s\t%s\n" for each fs */
	while (tmp) {
		if (!(tmp->fs_flags & FS_REQUIRES_DEV))
			newlen += strlen("nodev");
		newlen += strlen("\t");
		newlen += strlen(tmp->name);
		newlen += strlen("\n");
		tmp = tmp->next;
	}
	write_unlock(&file_systems_lock);

	new = kmalloc(offsetof(struct file_systems_string, string) + newlen + 1,
		      GFP_KERNEL);
	if (!new)
		return -ENOMEM;

	new->gen = gen;
	new->len = newlen;
	new->string[newlen] = '\0';
	write_lock(&file_systems_lock);
	old = file_systems_string;

	/*
	 * Did someone beat us to it?
	 */
	if (old && old->gen == file_systems_gen) {
		write_unlock(&file_systems_lock);
		kfree(new);
		return 0;
	}

	/*
	 * Did the list change in the meantime?
	 */
	if (gen != file_systems_gen) {
		write_unlock(&file_systems_lock);
		kfree(new);
		goto retry;
	}

	/*
	 * Populated the string.
	 *
	 * We know we have just enough space because we calculated the right
	 * size the previous time we had the lock and confirmed the list has
	 * not changed after reacquiring it.
	 */
	usedlen = 0;
	tmp = file_systems;
	while (tmp) {
		usedlen += sprintf(&new->string[usedlen], "%s\t%s\n",
			(tmp->fs_flags & FS_REQUIRES_DEV) ? "" : "nodev",
			tmp->name);
		tmp = tmp->next;
	}
	BUG_ON(new->len != strlen(new->string));

	/*
	 * Pairs with consume fence in READ_ONCE() in filesystems_proc_show().
	 */
	smp_store_release(&file_systems_string, new);
	write_unlock(&file_systems_lock);
	kfree_rcu(old, rcufree);
	return 0;
}

static int filesystems_proc_show(struct seq_file *m, void *v)
{
	struct file_systems_string *fss;

	for (;;) {
		scoped_guard(rcu) {
			/*
			 * Pairs with smp_store_release() in regen_filesystems_string().
			 */
			fss = READ_ONCE(file_systems_string);
			if (likely(fss)) {
				seq_write(m, fss->string, fss->len);
				return 0;
			}
		}

		int err = regen_filesystems_string();
		if (unlikely(err))
			return err;
	}
}

static int __init proc_filesystems_init(void)
{
	struct proc_dir_entry *pde;

	pde = proc_create_single("filesystems", 0, NULL, filesystems_proc_show);
	if (!pde)
		return -ENOMEM;
	proc_make_permanent(pde);
	return 0;
}
module_init(proc_filesystems_init);
#endif

static struct file_system_type *__get_fs_type(const char *name, int len)
{
	struct file_system_type *fs;

	read_lock(&file_systems_lock);
	fs = *(find_filesystem(name, len));
	if (fs && !try_module_get(fs->owner))
		fs = NULL;
	read_unlock(&file_systems_lock);
	return fs;
}

struct file_system_type *get_fs_type(const char *name)
{
	struct file_system_type *fs;
	const char *dot = strchr(name, '.');
	int len = dot ? dot - name : strlen(name);

	fs = __get_fs_type(name, len);
	if (!fs && (request_module("fs-%.*s", len, name) == 0)) {
		fs = __get_fs_type(name, len);
		if (!fs)
			pr_warn_once("request_module fs-%.*s succeeded, but still no fs?\n",
				     len, name);
	}

	if (dot && fs && !(fs->fs_flags & FS_HAS_SUBTYPE)) {
		put_filesystem(fs);
		fs = NULL;
	}
	return fs;
}

EXPORT_SYMBOL(get_fs_type);
