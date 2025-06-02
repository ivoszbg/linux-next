// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2025 Ivaylo Ivanov <ivo.ivanov.ivanov1@gmail.com>
 */

#include <linux/clk-provider.h>
#include <linux/platform_device.h>
#include <dt-bindings/clock/mediatek,mt6877-clk.h>

#include "clk-gate.h"
#include "clk-mtk.h"

static const struct mtk_gate_regs impc_cg_regs = {
	.set_ofs = 0xe08,
	.clr_ofs = 0xe04,
	.sta_ofs = 0xe00,
};

#define GATE_IMPC(_id, _name, _parent, _shift) {	\
		.id = _id,				\
		.name = _name,				\
		.parent_name = _parent,			\
		.regs = &impc_cg_regs,			\
		.shift = _shift,			\
		.ops = &mtk_clk_gate_ops_setclr,	\
	}

static const struct mtk_gate impc_clks[] = {
	GATE_IMPC(CLK_IMPC_AP_CLOCK_I2C10, "impc_ap_clock_i2c10",
			"fi2c_pseudo_ck"/* parent */, 0),
	GATE_IMPC(CLK_IMPC_AP_CLOCK_I2C11, "impc_ap_clock_i2c11",
			"fi2c_pseudo_ck"/* parent */, 1),
};

static const struct mtk_gate_regs impe_cg_regs = {
	.set_ofs = 0xe08,
	.clr_ofs = 0xe04,
	.sta_ofs = 0xe00,
};

#define GATE_IMPE(_id, _name, _parent, _shift) {	\
		.id = _id,				\
		.name = _name,				\
		.parent_name = _parent,			\
		.regs = &impe_cg_regs,			\
		.shift = _shift,			\
		.ops = &mtk_clk_gate_ops_setclr,	\
	}

static const struct mtk_gate impe_clks[] = {
	GATE_IMPE(CLK_IMPE_AP_CLOCK_I2C3, "impe_ap_clock_i2c3",
			"fi2c_pseudo_ck"/* parent */, 0),
};

static const struct mtk_gate_regs impn_cg_regs = {
	.set_ofs = 0xe08,
	.clr_ofs = 0xe04,
	.sta_ofs = 0xe00,
};

#define GATE_IMPN(_id, _name, _parent, _shift) {	\
		.id = _id,				\
		.name = _name,				\
		.parent_name = _parent,			\
		.regs = &impn_cg_regs,			\
		.shift = _shift,			\
		.ops = &mtk_clk_gate_ops_setclr,	\
	}

static const struct mtk_gate impn_clks[] = {
	GATE_IMPN(CLK_IMPN_AP_CLOCK_I2C6, "impn_ap_clock_i2c6",
			"fi2c_pseudo_ck"/* parent */, 0),
};

static const struct mtk_gate_regs imps_cg_regs = {
	.set_ofs = 0xe08,
	.clr_ofs = 0xe04,
	.sta_ofs = 0xe00,
};

#define GATE_IMPS(_id, _name, _parent, _shift) {	\
		.id = _id,				\
		.name = _name,				\
		.parent_name = _parent,			\
		.regs = &imps_cg_regs,			\
		.shift = _shift,			\
		.ops = &mtk_clk_gate_ops_setclr,	\
	}

static const struct mtk_gate imps_clks[] = {
	GATE_IMPS(CLK_IMPS_AP_CLOCK_I2C5, "imps_ap_clock_i2c5",
			"fi2c_pseudo_ck"/* parent */, 0),
	GATE_IMPS(CLK_IMPS_AP_CLOCK_I2C7, "imps_ap_clock_i2c7",
			"fi2c_pseudo_ck"/* parent */, 1),
	GATE_IMPS(CLK_IMPS_AP_CLOCK_I2C8, "imps_ap_clock_i2c8",
			"fi2c_pseudo_ck"/* parent */, 2),
	GATE_IMPS(CLK_IMPS_AP_CLOCK_I2C9, "imps_ap_clock_i2c9",
			"fi2c_pseudo_ck"/* parent */, 3),
};

static const struct mtk_gate_regs impw_cg_regs = {
	.set_ofs = 0xe08,
	.clr_ofs = 0xe04,
	.sta_ofs = 0xe00,
};

#define GATE_IMPW(_id, _name, _parent, _shift) {	\
		.id = _id,				\
		.name = _name,				\
		.parent_name = _parent,			\
		.regs = &impw_cg_regs,			\
		.shift = _shift,			\
		.ops = &mtk_clk_gate_ops_setclr,	\
	}

static const struct mtk_gate impw_clks[] = {
	GATE_IMPW(CLK_IMPW_AP_CLOCK_I2C0, "impw_ap_clock_i2c0",
			"fi2c_pseudo_ck"/* parent */, 0),
};

static const struct mtk_gate_regs impws_cg_regs = {
	.set_ofs = 0xe08,
	.clr_ofs = 0xe04,
	.sta_ofs = 0xe00,
};

#define GATE_IMPWS(_id, _name, _parent, _shift) {	\
		.id = _id,				\
		.name = _name,				\
		.parent_name = _parent,			\
		.regs = &impws_cg_regs,			\
		.shift = _shift,			\
		.ops = &mtk_clk_gate_ops_setclr,	\
	}

static const struct mtk_gate impws_clks[] = {
	GATE_IMPWS(CLK_IMPWS_AP_CLOCK_I2C1, "impws_ap_clock_i2c1",
			"fi2c_pseudo_ck"/* parent */, 0),
	GATE_IMPWS(CLK_IMPWS_AP_CLOCK_I2C2, "impws_ap_clock_i2c2",
			"fi2c_pseudo_ck"/* parent */, 1),
	GATE_IMPWS(CLK_IMPWS_AP_CLOCK_I2C4, "impws_ap_clock_i2c4",
			"fi2c_pseudo_ck"/* parent */, 2),
};

static const struct mtk_clk_desc imp_iic_wrap_impc_desc = {
	.clks = impc_clks,
	.num_clks = ARRAY_SIZE(impc_clks),
};

static const struct mtk_clk_desc imp_iic_wrap_impe_desc = {
	.clks = impe_clks,
	.num_clks = ARRAY_SIZE(impe_clks),
};

static const struct mtk_clk_desc imp_iic_wrap_impn_desc = {
	.clks = impn_clks,
	.num_clks = ARRAY_SIZE(impn_clks),
};

static const struct mtk_clk_desc imp_iic_wrap_imps_desc = {
	.clks = imps_clks,
	.num_clks = ARRAY_SIZE(imps_clks),
};

static const struct mtk_clk_desc imp_iic_wrap_impw_desc = {
	.clks = impw_clks,
	.num_clks = ARRAY_SIZE(impw_clks),
};

static const struct mtk_clk_desc imp_iic_wrap_impws_desc = {
	.clks = impws_clks,
	.num_clks = ARRAY_SIZE(impws_clks),
};

static const struct of_device_id of_match_clk_mt6877_imp_iic_wrap[] = {
	{
		.compatible = "mediatek,mt6877-imp_iic_wrap_c",
		.data = &imp_iic_wrap_impc_desc,
	}, {
		.compatible = "mediatek,mt6877-imp_iic_wrap_e",
		.data = &imp_iic_wrap_impe_desc,
	}, {
		.compatible = "mediatek,mt6877-imp_iic_wrap_n",
		.data = &imp_iic_wrap_impn_desc,
	}, {
		.compatible = "mediatek,mt6877-imp_iic_wrap_s",
		.data = &imp_iic_wrap_imps_desc,
	}, {
		.compatible = "mediatek,mt6877-imp_iic_wrap_w",
		.data = &imp_iic_wrap_impw_desc,
	}, {
		.compatible = "mediatek,mt6877-imp_iic_wrap_ws",
		.data = &imp_iic_wrap_impws_desc,
	}, {
		/* sentinel */
	}
};
MODULE_DEVICE_TABLE(of, of_match_clk_mt6877_imp_iic_wrap);

static struct platform_driver clk_mt6877_imp_iic_wrap_drv = {
	.probe = mtk_clk_simple_probe,
	.remove = mtk_clk_simple_remove,
	.driver = {
		.name = "clk-mt6877-imp_iic_wrap",
		.of_match_table = of_match_clk_mt6877_imp_iic_wrap,
	},
};
module_platform_driver(clk_mt6877_imp_iic_wrap_drv);

MODULE_DESCRIPTION("MediaTek MT6877 imp iic wrap clocks driver");
MODULE_LICENSE("GPL");
