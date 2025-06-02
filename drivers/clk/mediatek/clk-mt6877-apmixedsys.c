// SPDX-License-Identifier: GPL-2.0-only
//
// Copyright (c) 2025 Ivaylo Ivanov <ivo.ivanov.ivanov1@gmail.com>

#include <linux/clk-provider.h>
#include <linux/platform_device.h>
#include <dt-bindings/clock/mediatek,mt6877-clk.h>

#include "clk-fhctl.h"
#include "clk-mtk.h"
#include "clk-pll.h"
#include "clk-pllfh.h"

/* APMIXED PLL REG */
#define AP_PLL_CON3				0x00C
#define ARMPLL_LL_CON0				0x208
#define ARMPLL_LL_CON1				0x20C
#define ARMPLL_LL_CON2				0x210
#define ARMPLL_LL_CON3				0x214
#define ARMPLL_BL_CON0				0x218
#define ARMPLL_BL_CON1				0x21C
#define ARMPLL_BL_CON2				0x220
#define ARMPLL_BL_CON3				0x224
#define CCIPLL_CON0				0x238
#define CCIPLL_CON1				0x23C
#define CCIPLL_CON2				0x240
#define CCIPLL_CON3				0x244
#define MAINPLL_CON0				0x350
#define MAINPLL_CON1				0x354
#define MAINPLL_CON2				0x358
#define MAINPLL_CON3				0x35C
#define UNIVPLL_CON0				0x308
#define UNIVPLL_CON1				0x30C
#define UNIVPLL_CON2				0x310
#define UNIVPLL_CON3				0x314
#define MSDCPLL_CON0				0x360
#define MSDCPLL_CON1				0x364
#define MSDCPLL_CON2				0x368
#define MSDCPLL_CON3				0x36C
#define MMPLL_CON0				0x3A0
#define MMPLL_CON1				0x3A4
#define MMPLL_CON2				0x3A8
#define MMPLL_CON3				0x3AC
#define ADSPPLL_CON0				0x380
#define ADSPPLL_CON1				0x384
#define ADSPPLL_CON2				0x388
#define ADSPPLL_CON3				0x38C
#define TVDPLL_CON0				0x248
#define TVDPLL_CON1				0x24C
#define TVDPLL_CON2				0x250
#define TVDPLL_CON3				0x254
#define APLL1_CON0				0x328
#define APLL1_CON1				0x32C
#define APLL1_CON2				0x330
#define APLL1_CON3				0x334
#define APLL1_CON4				0x338
#define APLL2_CON0				0x33C
#define APLL2_CON1				0x340
#define APLL2_CON2				0x344
#define APLL2_CON3				0x348
#define APLL2_CON4				0x34C
#define MPLL_CON0				0x390
#define MPLL_CON1				0x394
#define MPLL_CON2				0x398
#define MPLL_CON3				0x39C
#define USBPLL_CON0				0x318
#define USBPLL_CON1				0x31C
#define USBPLL_CON2				0x320
#define USBPLL_CON3				0x324
#define IMGPLL_CON0				0x370
#define IMGPLL_CON1				0x374
#define IMGPLL_CON2				0x378
#define IMGPLL_CON3				0x37C

#define MT6877_PLL_FMAX		(3800UL * MHZ)
#define MT6877_PLL_FMIN		(1500UL * MHZ)
#define MT6877_INTEGER_BITS	(8)

#define PLL(_id, _name, _reg, _en_reg, _en_mask,		\
	    _pwr_reg, _flags, _rst_bar_mask,			\
	    _pd_reg, _pd_shift, _tuner_reg,			\
	    _tuner_en_reg, _tuner_en_bit,			\
	    _pcw_reg, _pcw_shift, _pcwbits) {			\
		.id = _id,						\
		.name = _name,						\
		.reg = _reg,						\
		.en_reg = _en_reg,					\
		.en_mask = _en_mask,					\
		.pwr_reg = _pwr_reg,					\
		.flags = _flags,					\
		.rst_bar_mask = _rst_bar_mask,				\
		.fmax = MT6877_PLL_FMAX,				\
		.fmin = MT6877_PLL_FMIN,				\
		.pd_reg = _pd_reg,					\
		.pd_shift = _pd_shift,					\
		.tuner_reg = _tuner_reg,				\
		.tuner_en_reg = _tuner_en_reg,				\
		.tuner_en_bit = _tuner_en_bit,				\
		.pcw_reg = _pcw_reg,					\
		.pcw_shift = _pcw_shift,				\
		.pcwbits = _pcwbits,					\
		.pcwibits = MT6877_INTEGER_BITS,			\
	}

static const struct mtk_pll_data plls[] = {
	/*
	 * armpll_ll/armpll_bl/ccipll are main clock source of AP MCU,
	 * should not be closed in Linux world.
	 */
	PLL(CLK_APMIXED_ARMPLL_LL, "armpll_ll", ARMPLL_LL_CON0, ARMPLL_LL_CON0, BIT(0),
		ARMPLL_LL_CON3, PLL_AO, BIT(0),
		ARMPLL_LL_CON1, 24,
		0, 0, 0,
		ARMPLL_LL_CON1, 0, 22),
	PLL(CLK_APMIXED_ARMPLL_BL, "armpll_bl", ARMPLL_BL_CON0,
		ARMPLL_BL_CON0, BIT(0),
		ARMPLL_BL_CON3, PLL_AO, BIT(0),
		ARMPLL_BL_CON1, 24,
		0, 0, 0,
		ARMPLL_BL_CON1, 0, 22),
	PLL(CLK_APMIXED_CCIPLL, "ccipll", CCIPLL_CON0,
		CCIPLL_CON0, BIT(0),
		CCIPLL_CON3, PLL_AO, BIT(0),
		CCIPLL_CON1, 24,
		0, 0, 0,
		CCIPLL_CON1, 0, 22),
	PLL(CLK_APMIXED_MAINPLL, "mainpll", MAINPLL_CON0,
		MAINPLL_CON0, BIT(0),
		MAINPLL_CON3, HAVE_RST_BAR | PLL_AO, BIT(23),
		MAINPLL_CON1, 24,
		0, 0, 0,
		MAINPLL_CON1, 0, 22),
	PLL(CLK_APMIXED_UNIVPLL, "univpll", UNIVPLL_CON0,
		UNIVPLL_CON0, BIT(0),
		UNIVPLL_CON3, HAVE_RST_BAR, BIT(23),
		UNIVPLL_CON1, 24,
		0, 0, 0,
		UNIVPLL_CON1, 0, 22),
	PLL(CLK_APMIXED_MSDCPLL, "msdcpll", MSDCPLL_CON0,
		MSDCPLL_CON0, BIT(0),
		MSDCPLL_CON3, 0, BIT(0),
		MSDCPLL_CON1, 24,
		0, 0, 0,
		MSDCPLL_CON1, 0, 22),
	PLL(CLK_APMIXED_MMPLL, "mmpll", MMPLL_CON0,
		MMPLL_CON0, BIT(0),
		MMPLL_CON3, HAVE_RST_BAR, BIT(23),
		MMPLL_CON1, 24,
		0, 0, 0,
		MMPLL_CON1, 0, 22),
	PLL(CLK_APMIXED_ADSPPLL, "adsppll", ADSPPLL_CON0,
		ADSPPLL_CON0, BIT(0),
		ADSPPLL_CON3, 0, BIT(0),
		ADSPPLL_CON1, 24,
		0, 0, 0,
		ADSPPLL_CON1, 0, 22),
	PLL(CLK_APMIXED_TVDPLL, "tvdpll", TVDPLL_CON0,
		TVDPLL_CON0, BIT(0),
		TVDPLL_CON3, 0, BIT(0),
		TVDPLL_CON1, 24,
		0, 0, 0,
		TVDPLL_CON1, 0, 22),
	PLL(CLK_APMIXED_APLL1, "apll1", APLL1_CON0,
		APLL1_CON0, BIT(0),
		APLL1_CON4, 0, BIT(0),
		APLL1_CON1, 24,
		0, AP_PLL_CON3, 0,
		APLL1_CON2, 0, 32),
	PLL(CLK_APMIXED_APLL2, "apll2", APLL2_CON0,
		APLL2_CON0, BIT(0),
		APLL2_CON4, 0, BIT(0),
		APLL2_CON1, 24,
		0, AP_PLL_CON3, 5,
		APLL2_CON2, 0, 32),
	PLL(CLK_APMIXED_MPLL, "mpll", MPLL_CON0,
		MPLL_CON0, BIT(0),
		MPLL_CON3, PLL_AO, BIT(0),
		MPLL_CON1, 24,
		0, 0, 0,
		MPLL_CON1, 0, 22),
	PLL(CLK_APMIXED_USBPLL, "usbpll", USBPLL_CON0,
		USBPLL_CON0, BIT(0),
		USBPLL_CON3, 0, BIT(0),
		USBPLL_CON1, 24,
		0, 0, 0,
		USBPLL_CON1, 0, 22),
};

enum fh_pll_id {
	FH_ARMPLL_LL,
	FH_ARMPLL_BL0,
	FH_ARMPLL_B,
	FH_CCIPLL,
	FH_MEMPLL,
	FH_EMIPLL,
	FH_MPLL,
	FH_MMPLL,
	FH_MAINPLL,
	FH_MSDCPLL,
	FH_ADSPPLL,
	FH_IMGPLL,
	FH_TVDPLL,
	FH_NR_FH,
};

#define FH(_pllid, _fhid, _offset) {					\
		.data = {						\
			.pll_id = _pllid,				\
			.fh_id = _fhid,					\
			.fh_ver = FHCTL_PLLFH_V2,			\
			.fhx_offset = _offset,				\
			.dds_mask = GENMASK(21, 0),			\
			.slope0_value = 0x6003c97,			\
			.slope1_value = 0x6003c97,			\
			.sfstrx_en = BIT(2),				\
			.frddsx_en = BIT(1),				\
			.fhctlx_en = BIT(0),				\
			.tgl_org = BIT(31),				\
			.dvfs_tri = BIT(31),				\
			.pcwchg = BIT(31),				\
			.dt_val = 0x0,					\
			.df_val = 0x9,					\
			.updnlmt_shft = 16,				\
			.msk_frddsx_dys = GENMASK(23, 20),		\
			.msk_frddsx_dts = GENMASK(19, 16),		\
		},							\
	}

static struct mtk_pllfh_data pllfhs[] = {
	FH(CLK_APMIXED_ARMPLL_LL, FH_ARMPLL_LL, 0x003C),
	FH(CLK_APMIXED_ARMPLL_BL, FH_ARMPLL_BL0, 0x0050),
	FH(CLK_APMIXED_CCIPLL, FH_CCIPLL, 0x0078),
	FH(CLK_APMIXED_MPLL, FH_MPLL, 0x00B4),
	FH(CLK_APMIXED_MMPLL, FH_MMPLL, 0x00C8),
	FH(CLK_APMIXED_MAINPLL, FH_MAINPLL, 0x00DC),
	FH(CLK_APMIXED_MSDCPLL, FH_MSDCPLL, 0x00F0),
	FH(CLK_APMIXED_ADSPPLL, FH_ADSPPLL, 0x0104),
	FH(CLK_APMIXED_TVDPLL, FH_TVDPLL, 0x0012C),
};

static const struct of_device_id of_match_clk_mt6877_apmixed[] = {
	{ .compatible = "mediatek,mt6877-apmixedsys", },
	{}
};
MODULE_DEVICE_TABLE(of, of_match_clk_mt6877_apmixed);

static int clk_mt6877_apmixed_probe(struct platform_device *pdev)
{
	struct clk_hw_onecell_data *clk_data;
	struct device_node *node = pdev->dev.of_node;
	const u8 *fhctl_node = "mediatek,mt6877-fhctl";
	int r;

	clk_data = mtk_alloc_clk_data(CLK_APMIXED_NR_CLK);
	if (!clk_data)
		return -ENOMEM;

	fhctl_parse_dt(fhctl_node, pllfhs, ARRAY_SIZE(pllfhs));

	r = mtk_clk_register_pllfhs(node, plls, ARRAY_SIZE(plls),
				    pllfhs, ARRAY_SIZE(pllfhs), clk_data);
	if (r)
		goto free_apmixed_data;

	r = of_clk_add_hw_provider(node, of_clk_hw_onecell_get, clk_data);
	if (r)
		goto unregister_plls;

	platform_set_drvdata(pdev, clk_data);

	return r;

unregister_plls:
	mtk_clk_unregister_pllfhs(plls, ARRAY_SIZE(plls), pllfhs,
				  ARRAY_SIZE(pllfhs), clk_data);
free_apmixed_data:
	mtk_free_clk_data(clk_data);
	return r;
}

static void clk_mt6877_apmixed_remove(struct platform_device *pdev)
{
	struct device_node *node = pdev->dev.of_node;
	struct clk_hw_onecell_data *clk_data = platform_get_drvdata(pdev);

	of_clk_del_provider(node);
	mtk_clk_unregister_pllfhs(plls, ARRAY_SIZE(plls), pllfhs,
				  ARRAY_SIZE(pllfhs), clk_data);
	mtk_free_clk_data(clk_data);
}

static struct platform_driver clk_mt6877_apmixed_drv = {
	.probe = clk_mt6877_apmixed_probe,
	.remove = clk_mt6877_apmixed_remove,
	.driver = {
		.name = "clk-mt6877-apmixed",
		.of_match_table = of_match_clk_mt6877_apmixed,
	},
};
module_platform_driver(clk_mt6877_apmixed_drv);

MODULE_DESCRIPTION("MediaTek MT6877 apmixedsys clocks driver");
MODULE_LICENSE("GPL");
