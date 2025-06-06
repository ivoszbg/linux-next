// SPDX-License-Identifier: GPL-2.0
/*
 * STM32 Low-Power Timer parent driver.
 * Copyright (C) STMicroelectronics 2017
 * Author: Fabrice Gasnier <fabrice.gasnier@st.com>
 * Inspired by Benjamin Gaignard's stm32-timers driver
 */

#include <linux/bitfield.h>
#include <linux/mfd/stm32-lptimer.h>
#include <linux/module.h>
#include <linux/of_platform.h>
#include <linux/platform_device.h>

#define STM32_LPTIM_MAX_REGISTER	0x3fc

static const struct regmap_config stm32_lptimer_regmap_cfg = {
	.reg_bits = 32,
	.val_bits = 32,
	.reg_stride = sizeof(u32),
	.max_register = STM32_LPTIM_MAX_REGISTER,
	.fast_io = true,
};

static int stm32_lptimer_detect_encoder(struct stm32_lptimer *ddata)
{
	u32 val;
	int ret;

	/*
	 * Quadrature encoder mode bit can only be written and read back when
	 * Low-Power Timer supports it.
	 */
	ret = regmap_update_bits(ddata->regmap, STM32_LPTIM_CFGR,
				 STM32_LPTIM_ENC, STM32_LPTIM_ENC);
	if (ret)
		return ret;

	ret = regmap_read(ddata->regmap, STM32_LPTIM_CFGR, &val);
	if (ret)
		return ret;

	ret = regmap_update_bits(ddata->regmap, STM32_LPTIM_CFGR,
				 STM32_LPTIM_ENC, 0);
	if (ret)
		return ret;

	ddata->has_encoder = !!(val & STM32_LPTIM_ENC);

	return 0;
}

static int stm32_lptimer_detect_hwcfgr(struct stm32_lptimer *ddata)
{
	u32 val;
	int ret;

	ret = regmap_read(ddata->regmap, STM32_LPTIM_VERR, &ddata->version);
	if (ret)
		return ret;

	/* Try to guess parameters from HWCFGR: e.g. encoder mode (STM32MP15) */
	ret = regmap_read(ddata->regmap, STM32_LPTIM_HWCFGR1, &val);
	if (ret)
		return ret;

	/* Fallback to legacy init if HWCFGR isn't present */
	if (!val)
		return stm32_lptimer_detect_encoder(ddata);

	ddata->has_encoder = FIELD_GET(STM32_LPTIM_HWCFGR1_ENCODER, val);

	ret = regmap_read(ddata->regmap, STM32_LPTIM_HWCFGR2, &val);
	if (ret)
		return ret;

	/* Number of capture/compare channels */
	ddata->num_cc_chans = FIELD_GET(STM32_LPTIM_HWCFGR2_CHAN_NUM, val);

	return 0;
}

static int stm32_lptimer_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct stm32_lptimer *ddata;
	void __iomem *mmio;
	int ret;

	ddata = devm_kzalloc(dev, sizeof(*ddata), GFP_KERNEL);
	if (!ddata)
		return -ENOMEM;

	mmio = devm_platform_get_and_ioremap_resource(pdev, 0, NULL);
	if (IS_ERR(mmio))
		return PTR_ERR(mmio);

	ddata->regmap = devm_regmap_init_mmio_clk(dev, "mux", mmio,
						  &stm32_lptimer_regmap_cfg);
	if (IS_ERR(ddata->regmap))
		return PTR_ERR(ddata->regmap);

	ddata->clk = devm_clk_get(dev, NULL);
	if (IS_ERR(ddata->clk))
		return PTR_ERR(ddata->clk);

	ret = stm32_lptimer_detect_hwcfgr(ddata);
	if (ret)
		return ret;

	platform_set_drvdata(pdev, ddata);

	return devm_of_platform_populate(&pdev->dev);
}

static const struct of_device_id stm32_lptimer_of_match[] = {
	{ .compatible = "st,stm32-lptimer", },
	{},
};
MODULE_DEVICE_TABLE(of, stm32_lptimer_of_match);

static struct platform_driver stm32_lptimer_driver = {
	.probe = stm32_lptimer_probe,
	.driver = {
		.name = "stm32-lptimer",
		.of_match_table = stm32_lptimer_of_match,
	},
};
module_platform_driver(stm32_lptimer_driver);

MODULE_AUTHOR("Fabrice Gasnier <fabrice.gasnier@st.com>");
MODULE_DESCRIPTION("STMicroelectronics STM32 Low-Power Timer");
MODULE_ALIAS("platform:stm32-lptimer");
MODULE_LICENSE("GPL v2");
