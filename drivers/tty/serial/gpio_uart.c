#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio/consumer.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/kthread.h>

#define DEFAULT_BAUD_RATE 9600

struct bb_uart_dev {
    struct gpio_desc *tx_gpio;
    struct task_struct *tx_thread;
    int baud_rate;
};

static void bb_uart_tx_byte(struct bb_uart_dev *dev, uint8_t byte)
{
    int i;
    unsigned int bit_time_us = 1000000 / dev->baud_rate;

    // Start bit
    gpiod_set_value(dev->tx_gpio, 0);
    udelay(bit_time_us);

    // Data bits
    for (i = 0; i < 8; i++) {
        gpiod_set_value(dev->tx_gpio, (byte >> i) & 1);
        udelay(bit_time_us);
    }

    // Stop bit
    gpiod_set_value(dev->tx_gpio, 1);
    udelay(bit_time_us);
}

static int bb_uart_thread_fn(void *data)
{
    struct bb_uart_dev *dev = data;

    // Byte to spam: 0x55 = 01010101 (alternating high/low bits)
    // Makes for a nice square wave
    uint8_t spam_byte = 0x55;

    while (!kthread_should_stop()) {
        bb_uart_tx_byte(dev, spam_byte);
    }
    return 0;
}

static int bb_uart_probe(struct platform_device *pdev)
{
    struct bb_uart_dev *dev;
    struct device *device = &pdev->dev;
    struct device_node *np = device->of_node;
    int ret;

    dev = devm_kzalloc(device, sizeof(*dev), GFP_KERNEL);
    if (!dev)
        return -ENOMEM;

    // Read GPIO
    dev->tx_gpio = devm_gpiod_get(device, "tx", GPIOD_OUT_HIGH);
    if (IS_ERR(dev->tx_gpio)) {
        dev_err(device, "Failed to get TX GPIO\n");
        return PTR_ERR(dev->tx_gpio);
    }

    // Read baud rate (optional)
    if (of_property_read_u32(np, "baud-rate", &dev->baud_rate))
        dev->baud_rate = DEFAULT_BAUD_RATE;

    platform_set_drvdata(pdev, dev);

    dev->tx_thread = kthread_run(bb_uart_thread_fn, dev, "bb_uart_tx");
    if (IS_ERR(dev->tx_thread)) {
        dev_err(device, "Failed to create thread\n");
        return PTR_ERR(dev->tx_thread);
    }

    dev_info(device, "Bitbanged UART initialized at %d baud\n", dev->baud_rate);
    return 0;
}

static void bb_uart_remove(struct platform_device *pdev)
{
    struct bb_uart_dev *dev = platform_get_drvdata(pdev);

    if (dev->tx_thread)
        kthread_stop(dev->tx_thread);
}

static const struct of_device_id bb_uart_of_match[] = {
    { .compatible = "uart-gpio", },
    { /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, bb_uart_of_match);

static struct platform_driver bb_uart_driver = {
    .probe  = bb_uart_probe,
    .remove = bb_uart_remove,
    .driver = {
        .name = "bitbang-uart",
        .of_match_table = bb_uart_of_match,
    },
};

module_platform_driver(bb_uart_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ChatGPT");
MODULE_DESCRIPTION("Device Tree Aware Bitbanged GPIO UART TX Driver");
