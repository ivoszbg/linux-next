// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2025 Ivaylo Ivanov <ivo.ivanov.ivanov1@gmail.com>
 */

#include <linux/clk-provider.h>
#include <linux/platform_device.h>
#include <dt-bindings/clock/mediatek,mt6877-clk.h>

#include "clk-gate.h"
#include "clk-mtk.h"

static const struct mtk_gate_regs ifrao0_cg_regs = {
	.set_ofs = 0x80,
	.clr_ofs = 0x84,
	.sta_ofs = 0x90,
};

static const struct mtk_gate_regs ifrao1_cg_regs = {
	.set_ofs = 0x88,
	.clr_ofs = 0x8c,
	.sta_ofs = 0x94,
};

static const struct mtk_gate_regs ifrao2_cg_regs = {
	.set_ofs = 0xa4,
	.clr_ofs = 0xa8,
	.sta_ofs = 0xac,
};

static const struct mtk_gate_regs ifrao3_cg_regs = {
	.set_ofs = 0xc0,
	.clr_ofs = 0xc4,
	.sta_ofs = 0xc8,
};

static const struct mtk_gate_regs ifrao4_cg_regs = {
	.set_ofs = 0xe0,
	.clr_ofs = 0xe4,
	.sta_ofs = 0xe8,
};

#define GATE_IFRAO0(_id, _name, _parent, _shift) {	\
		.id = _id,				\
		.name = _name,				\
		.parent_name = _parent,			\
		.regs = &ifrao0_cg_regs,			\
		.shift = _shift,			\
		.ops = &mtk_clk_gate_ops_setclr,	\
	}

#define GATE_IFRAO1(_id, _name, _parent, _shift) {	\
		.id = _id,				\
		.name = _name,				\
		.parent_name = _parent,			\
		.regs = &ifrao1_cg_regs,			\
		.shift = _shift,			\
		.ops = &mtk_clk_gate_ops_setclr,	\
	}

#define GATE_IFRAO2(_id, _name, _parent, _shift) {	\
		.id = _id,				\
		.name = _name,				\
		.parent_name = _parent,			\
		.regs = &ifrao2_cg_regs,			\
		.shift = _shift,			\
		.ops = &mtk_clk_gate_ops_setclr,	\
	}

#define GATE_IFRAO3(_id, _name, _parent, _shift) {	\
		.id = _id,				\
		.name = _name,				\
		.parent_name = _parent,			\
		.regs = &ifrao3_cg_regs,			\
		.shift = _shift,			\
		.ops = &mtk_clk_gate_ops_setclr,	\
	}

#define GATE_IFRAO4(_id, _name, _parent, _shift) {	\
		.id = _id,				\
		.name = _name,				\
		.parent_name = _parent,			\
		.regs = &ifrao4_cg_regs,			\
		.shift = _shift,			\
		.ops = &mtk_clk_gate_ops_setclr,	\
	}

static const struct mtk_gate infra_ao_clks[] = {
	/* IFRAO0 */
	GATE_IFRAO0(CLK_IFRAO_PMIC_TMR, "ifrao_pmic_tmr",
		    "fpwrap_ulposc_ck", 0),
	GATE_IFRAO0(CLK_IFRAO_PMIC_AP, "ifrao_pmic_ap",
		    "fpwrap_ulposc_ck", 1),
	GATE_IFRAO0(CLK_IFRAO_PMIC_MD, "ifrao_pmic_md",
		    "fpwrap_ulposc_ck", 2),
	GATE_IFRAO0(CLK_IFRAO_PMIC_CONN, "ifrao_pmic_conn",
		    "fpwrap_ulposc_ck", 3),
	GATE_IFRAO0(CLK_IFRAO_APXGPT, "ifrao_apxgpt", "axi_ck", 6),
	GATE_IFRAO0(CLK_IFRAO_GCE, "ifrao_gce",  "axi_ck", 8),
	GATE_IFRAO0(CLK_IFRAO_GCE2, "ifrao_gce2", "axi_ck", 9),
	GATE_IFRAO0(CLK_IFRAO_THERM, "ifrao_therm", "axi_ck", 10),
	GATE_IFRAO0(CLK_IFRAO_I2C_PSEUDO, "ifrao_i2c_pseudo", "i2c_ck", 11),
	GATE_IFRAO0(CLK_IFRAO_PWM_HCLK, "ifrao_pwm_hclk", "axi_ck", 15),
	GATE_IFRAO0(CLK_IFRAO_PWM1, "ifrao_pwm1", "pwm_ck", 16),
	GATE_IFRAO0(CLK_IFRAO_PWM2, "ifrao_pwm2", "pwm_ck", 17),
	GATE_IFRAO0(CLK_IFRAO_PWM3, "ifrao_pwm3", "pwm_ck", 18),
	GATE_IFRAO0(CLK_IFRAO_PWM4, "ifrao_pwm4", "pwm_ck", 19),
	GATE_IFRAO0(CLK_IFRAO_PWM, "ifrao_pwm", "pwm_ck", 21),
	GATE_IFRAO0(CLK_IFRAO_UART0, "ifrao_uart0", "fuart_ck", 22),
	GATE_IFRAO0(CLK_IFRAO_UART1, "ifrao_uart1", "fuart_ck", 23),
	GATE_IFRAO0(CLK_IFRAO_UART2, "ifrao_uart2", "fuart_ck", 24),
	GATE_IFRAO0(CLK_IFRAO_UART3, "ifrao_uart3", "fuart_ck", 25),
	GATE_IFRAO0(CLK_IFRAO_GCE_26M, "ifrao_gce_26m", "axi_ck", 27),
	GATE_IFRAO0(CLK_IFRAO_BTIF, "ifrao_btif", "axi_ck", 31),

	/* IFRAO1 */
	GATE_IFRAO1(CLK_IFRAO_SPI0, "ifrao_spi0", "spi_ck", 1),
	GATE_IFRAO1(CLK_IFRAO_MSDC0, "ifrao_msdc0", "axi_ck", 2),
	GATE_IFRAO1(CLK_IFRAO_MSDC1, "ifrao_msdc1", "axi_ck", 4),
	GATE_IFRAO1(CLK_IFRAO_MSDC0_SRC, "ifrao_msdc0_clk", "msdc50_0_ck", 6),
	GATE_IFRAO1(CLK_IFRAO_AUXADC, "ifrao_auxadc", "f26m_ck", 10),
	GATE_IFRAO1(CLK_IFRAO_CPUM, "ifrao_cpum", "axi_ck", 11),
	GATE_IFRAO1(CLK_IFRAO_CCIF1_AP, "ifrao_ccif1_ap", "axi_ck", 12),
	GATE_IFRAO1(CLK_IFRAO_CCIF1_MD, "ifrao_ccif1_md", "axi_ck", 13),
	GATE_IFRAO1(CLK_IFRAO_AUXADC_MD, "ifrao_auxadc_md", "f26m_ck", 14),
	GATE_IFRAO1(CLK_IFRAO_MSDC1_SRC, "ifrao_msdc1_clk", "msdc30_1_ck", 16),
	GATE_IFRAO1(CLK_IFRAO_MSDC0_AES, "ifrao_msdc0_aes_clk", 
		    "msdc50_0_ck", 17),
	GATE_IFRAO1(CLK_IFRAO_DEVICE_APC, "ifrao_dapc", "axi_ck", 20),
	GATE_IFRAO1(CLK_IFRAO_CCIF_AP, "ifrao_ccif_ap", "axi_ck", 23),
	GATE_IFRAO1(CLK_IFRAO_AUDIO, "ifrao_audio", "axi_ck", 25),
	GATE_IFRAO1(CLK_IFRAO_CCIF_MD, "ifrao_ccif_md", "axi_ck", 26),

	/* IFRAO2 */
	GATE_IFRAO2(CLK_IFRAO_SSUSB, "ifrao_ssusb", "fusb_ck", 1),
	GATE_IFRAO2(CLK_IFRAO_DISP_PWM, "ifrao_disp_pwm", "fdisp_pwm_ck", 2),
	GATE_IFRAO2(CLK_IFRAO_CLDMA_BCLK, "ifrao_cldmabclk", "axi_ck", 3),
	GATE_IFRAO2(CLK_IFRAO_AUDIO_26M_BCLK, "ifrao_audio26m", "f26m_ck", 4),
	GATE_IFRAO2(CLK_IFRAO_SPI1, "ifrao_spi1", "spi_ck", 6),
	GATE_IFRAO2(CLK_IFRAO_SPI2, "ifrao_spi2", "spi_ck", 9),
	GATE_IFRAO2(CLK_IFRAO_SPI3, "ifrao_spi3", "spi_ck", 10),
	GATE_IFRAO2(CLK_IFRAO_UNIPRO_SYSCLK, "ifrao_unipro_sysclk", 
		    "ufs_ck", 11),
	GATE_IFRAO2(CLK_IFRAO_UFS_MP_SAP_BCLK, "ifrao_ufs_bclk", 
		    "f26m_ck", 13),
	GATE_IFRAO2(CLK_IFRAO_APDMA, "ifrao_apdma", "axi_ck", 18),
	GATE_IFRAO2(CLK_IFRAO_SPI4, "ifrao_spi4", "spi_ck", 25),
	GATE_IFRAO2(CLK_IFRAO_SPI5, "ifrao_spi5", "spi_ck", 26),
	GATE_IFRAO2(CLK_IFRAO_CQ_DMA, "ifrao_cq_dma", "axi_ck", 27),
	GATE_IFRAO2(CLK_IFRAO_UFS, "ifrao_ufs", "ufs_ck", 28),
	GATE_IFRAO2(CLK_IFRAO_AES_UFSFDE, "ifrao_aes_ufsfde", 
		    "aes_ufsfde_ck", 29),
	GATE_IFRAO2(CLK_IFRAO_SSUSB_XHCI, "ifrao_ssusb_xhci", 
		    "fssusb_xhci_ck", 31),

	/* IFRAO3 */
	GATE_IFRAO3(CLK_IFRAO_AP_MSDC0, "ifrao_ap_msdc0", "msdc50_0_ck", 7),
	GATE_IFRAO3(CLK_IFRAO_MD_MSDC0, "ifrao_md_msdc0", "msdc50_0_ck", 8),
	GATE_IFRAO3(CLK_IFRAO_CCIF5_MD, "ifrao_ccif5_md", "axi_ck", 10),
	GATE_IFRAO3(CLK_IFRAO_CCIF2_AP, "ifrao_ccif2_ap", "axi_ck", 16),
	GATE_IFRAO3(CLK_IFRAO_CCIF2_MD, "ifrao_ccif2_md", "axi_ck", 17),
	GATE_IFRAO3(CLK_IFRAO_FBIST2FPC, "ifrao_fbist2fpc", "msdc50_0_ck", 24),
	GATE_IFRAO3(CLK_IFRAO_DPMAIF_MAIN, "ifrao_dpmaif_main", 
		    "dpmaif_main_ck", 26),
	GATE_IFRAO3(CLK_IFRAO_CCIF4_MD, "ifrao_ccif4_md", "axi_ck", 29),
	GATE_IFRAO3(CLK_IFRAO_SPI6_CK, "ifrao_spi6_ck", "spi_ck", 30),
	GATE_IFRAO3(CLK_IFRAO_SPI7_CK, "ifrao_spi7_ck", "spi_ck", 31),

	/* IFRAO4 */
	GATE_IFRAO4(CLK_IFRAO_RG_AES_CK_0P_CK, "ifrao_aes_0p_ck",
		    "aes_msdcfde_ck", 18),
};

static const struct mtk_clk_desc infra_ao_desc = {
	.clks = infra_ao_clks,
	.num_clks = ARRAY_SIZE(infra_ao_clks),
};

static const struct of_device_id of_match_clk_mt6877_infra_ao[] = {
	{
		.compatible = "mediatek,mt6877-infracfg_ao",
		.data = &infra_ao_desc,
	}, {
		/* sentinel */
	}
};
MODULE_DEVICE_TABLE(of, of_match_clk_mt6877_infra_ao);

static struct platform_driver clk_mt6877_infra_ao_drv = {
	.probe = mtk_clk_simple_probe,
	.remove = mtk_clk_simple_remove,
	.driver = {
		.name = "clk-mt6877-infra-ao",
		.of_match_table = of_match_clk_mt6877_infra_ao,
	},
};
module_platform_driver(clk_mt6877_infra_ao_drv);

MODULE_DESCRIPTION("MediaTek MT6877 infracfg clocks driver");
MODULE_LICENSE("GPL");
