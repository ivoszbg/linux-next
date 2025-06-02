// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2025 Ivaylo Ivanov <ivo.ivanov.ivanov1@gmail.com>
 */

#include <linux/clk-provider.h>
#include <linux/mod_devicetable.h>
#include <linux/platform_device.h>
#include "clk-mtk.h"
#include "clk-gate.h"
#include "clk-mux.h"

#include <dt-bindings/clock/mediatek,mt6877-clk.h>
#include <linux/clk.h>

/* TOPCK MUX SEL REG */
#define CLK_CFG_UPDATE				0x0004
#define CLK_CFG_UPDATE1				0x0008
#define CLK_CFG_UPDATE2				0x000c
#define CLK_CFG_0				0x0010
#define CLK_CFG_0_SET				0x0014
#define CLK_CFG_0_CLR				0x0018
#define CLK_CFG_1				0x0020
#define CLK_CFG_1_SET				0x0024
#define CLK_CFG_1_CLR				0x0028
#define CLK_CFG_2				0x0030
#define CLK_CFG_2_SET				0x0034
#define CLK_CFG_2_CLR				0x0038
#define CLK_CFG_3				0x0040
#define CLK_CFG_3_SET				0x0044
#define CLK_CFG_3_CLR				0x0048
#define CLK_CFG_4				0x0050
#define CLK_CFG_4_SET				0x0054
#define CLK_CFG_4_CLR				0x0058
#define CLK_CFG_5				0x0060
#define CLK_CFG_5_SET				0x0064
#define CLK_CFG_5_CLR				0x0068
#define CLK_CFG_6				0x0070
#define CLK_CFG_6_SET				0x0074
#define CLK_CFG_6_CLR				0x0078
#define CLK_CFG_7				0x0080
#define CLK_CFG_7_SET				0x0084
#define CLK_CFG_7_CLR				0x0088
#define CLK_CFG_8				0x0090
#define CLK_CFG_8_SET				0x0094
#define CLK_CFG_8_CLR				0x0098
#define CLK_CFG_9				0x00A0
#define CLK_CFG_9_SET				0x00A4
#define CLK_CFG_9_CLR				0x00A8
#define CLK_CFG_10				0x00B0
#define CLK_CFG_10_SET				0x00B4
#define CLK_CFG_10_CLR				0x00B8
#define CLK_CFG_11				0x00C0
#define CLK_CFG_11_SET				0x00C4
#define CLK_CFG_11_CLR				0x00C8
#define CLK_CFG_12				0x00D0
#define CLK_CFG_12_SET				0x00D4
#define CLK_CFG_12_CLR				0x00D8
#define CLK_CFG_13				0x00E0
#define CLK_CFG_13_SET				0x00E4
#define CLK_CFG_13_CLR				0x00E8
#define CLK_CFG_14				0x00F0
#define CLK_CFG_14_SET				0x00F4
#define CLK_CFG_14_CLR				0x00F8
#define CLK_CFG_15				0x0100
#define CLK_CFG_15_SET				0x0104
#define CLK_CFG_15_CLR				0x0108
#define CLK_CFG_16				0x0110
#define CLK_CFG_16_SET				0x0114
#define CLK_CFG_16_CLR				0x0118
#define CLK_CFG_20				0x0120
#define CLK_CFG_20_SET				0x0124
#define CLK_CFG_20_CLR				0x0128
#define CLK_CFG_17				0x0180
#define CLK_CFG_17_SET				0x0184
#define CLK_CFG_17_CLR				0x0188
#define CLK_AUDDIV_0				0x0320

/* TOPCK MUX SHIFT */
#define TOP_MUX_AXI_SHIFT			0
#define TOP_MUX_SPM_SHIFT			1
#define TOP_MUX_SCP_SHIFT			2
#define TOP_MUX_BUS_AXIMEM_SHIFT		3
#define TOP_MUX_DISP0_SHIFT			4
#define TOP_MUX_MDP0_SHIFT			5
#define TOP_MUX_IMG1_SHIFT			6
#define TOP_MUX_IPE_SHIFT			7
#define TOP_MUX_DPE_SHIFT			8
#define TOP_MUX_CAM_SHIFT			9
#define TOP_MUX_CCU_SHIFT			10
#define TOP_MUX_DSP_SHIFT			11
#define TOP_MUX_DSP1_SHIFT			12
#define TOP_MUX_DSP2_SHIFT			13
#define TOP_MUX_DSP4_SHIFT			15
#define TOP_MUX_DSP7_SHIFT			18
#define TOP_MUX_CAMTG_SHIFT			19
#define TOP_MUX_CAMTG2_SHIFT			20
#define TOP_MUX_CAMTG3_SHIFT			21
#define TOP_MUX_CAMTG4_SHIFT			22
#define TOP_MUX_CAMTG5_SHIFT			23
#define TOP_MUX_UART_SHIFT			24
#define TOP_MUX_SPI_SHIFT			25
#define TOP_MUX_MSDC50_0_HCLK_SHIFT		26
#define TOP_MUX_MSDC50_0_SHIFT			27
#define TOP_MUX_MSDC30_1_SHIFT			28
#define TOP_MUX_AUDIO_SHIFT			29
#define TOP_MUX_AUD_INTBUS_SHIFT		30
#define TOP_MUX_PWRAP_ULPOSC_SHIFT		0
#define TOP_MUX_ATB_SHIFT			1
#define TOP_MUX_SSPM_SHIFT			2
#define TOP_MUX_DISP_PWM_SHIFT			4
#define TOP_MUX_USB_TOP_SHIFT			5
#define TOP_MUX_SSUSB_XHCI_SHIFT		6
#define TOP_MUX_I2C_SHIFT			9
#define TOP_MUX_SENINF_SHIFT			10
#define TOP_MUX_SENINF1_SHIFT			11
#define TOP_MUX_SENINF2_SHIFT			12
#define TOP_MUX_SENINF3_SHIFT			13
#define TOP_MUX_DXCC_SHIFT			14
#define TOP_MUX_AUD_ENGEN1_SHIFT		15
#define TOP_MUX_AUD_ENGEN2_SHIFT		16
#define TOP_MUX_AES_UFSFDE_SHIFT		17
#define TOP_MUX_UFS_SHIFT			18
#define TOP_MUX_AUD_1_SHIFT			19
#define TOP_MUX_AUD_2_SHIFT			20
#define TOP_MUX_ADSP_SHIFT			21
#define TOP_MUX_DPMAIF_MAIN_SHIFT		22
#define TOP_MUX_VENC_SHIFT			23
#define TOP_MUX_VDEC_SHIFT			24
#define TOP_MUX_CAMTM_SHIFT			25
#define TOP_MUX_PWM_SHIFT			26
#define TOP_MUX_AUDIO_H_SHIFT			27
#define TOP_MUX_MCUPM_SHIFT			28
#define TOP_MUX_SPMI_M_MST_SHIFT		30
#define TOP_MUX_DVFSRC_SHIFT			0
#define TOP_MUX_MEM_SUB_SHIFT			1
#define TOP_MUX_AES_MSDCFDE_SHIFT		2
#define TOP_MUX_UFS_MBIST_SHIFT			5
#define TOP_MUX_AP2CONN_HOST_SHIFT		6
#define TOP_MUX_MSDC_NEW_RX_SHIFT		7

/* TOPCK DIVIDER REG */
#define CLK_AUDDIV_2				0x0328
#define CLK_AUDDIV_3				0x0334
#define CLK_AUDDIV_4				0x0338

#define INV_OFS	-1
#define INV_BIT	-1

static DEFINE_SPINLOCK(mt6877_clk_lock);

static const struct mtk_fixed_factor top_divs[] = {
	FACTOR(CLK_TOP_MFGPLL1, "mfgpll1_ck", "mfg_ao_mfgpll1", 1, 1),
	FACTOR(CLK_TOP_MFGPLL4, "mfgpll4_ck", "mfg_ao_mfgpll4", 1, 1),
	FACTOR(CLK_TOP_MAINPLL_D4, "mainpll_d4", "mainpll", 1, 4),
	FACTOR(CLK_TOP_MAINPLL_D4_D2, "mainpll_d4_d2", "mainpll", 1, 8),
	FACTOR(CLK_TOP_MAINPLL_D4_D4, "mainpll_d4_d4", "mainpll", 1, 16),
	FACTOR(CLK_TOP_MAINPLL_D4_D8, "mainpll_d4_d8", "mainpll", 1, 32),
	FACTOR(CLK_TOP_MAINPLL_D4_D16, "mainpll_d4_d16", "mainpll", 1, 64),
	FACTOR(CLK_TOP_MAINPLL_D5, "mainpll_d5", "mainpll", 1, 5),
	FACTOR(CLK_TOP_MAINPLL_D5_D2, "mainpll_d5_d2", "mainpll", 1, 10),
	FACTOR(CLK_TOP_MAINPLL_D5_D4, "mainpll_d5_d4", "mainpll", 1, 20),
	FACTOR(CLK_TOP_MAINPLL_D5_D8, "mainpll_d5_d8", "mainpll", 1, 40),
	FACTOR(CLK_TOP_MAINPLL_D6, "mainpll_d6", "mainpll", 1, 6),
	FACTOR(CLK_TOP_MAINPLL_D6_D2, "mainpll_d6_d2", "mainpll", 1, 12),
	FACTOR(CLK_TOP_MAINPLL_D6_D4, "mainpll_d6_d4", "mainpll", 1, 24),
	FACTOR(CLK_TOP_MAINPLL_D6_D8, "mainpll_d6_d8", "mainpll", 1, 48),
	FACTOR(CLK_TOP_MAINPLL_D7, "mainpll_d7", "mainpll", 1, 7),
	FACTOR(CLK_TOP_MAINPLL_D7_D2, "mainpll_d7_d2", "mainpll", 1, 14),
	FACTOR(CLK_TOP_MAINPLL_D7_D4, "mainpll_d7_d4", "mainpll", 1, 28),
	FACTOR(CLK_TOP_MAINPLL_D7_D8, "mainpll_d7_d8", "mainpll", 1, 56),
	FACTOR(CLK_TOP_MAINPLL_D9, "mainpll_d9", "mainpll", 1, 9),
	FACTOR(CLK_TOP_UNIVPLL, "univpll_ck", "univpll", 1, 1),
	FACTOR(CLK_TOP_UNIVPLL_D4, "univpll_d4", "univpll", 1, 4),
	FACTOR(CLK_TOP_UNIVPLL_D4_D2, "univpll_d4_d2", "univpll", 1, 8),
	FACTOR(CLK_TOP_UNIVPLL_D4_D4, "univpll_d4_d4", "univpll", 1, 16),
	FACTOR(CLK_TOP_UNIVPLL_D4_D8, "univpll_d4_d8", "univpll", 1, 32),
	FACTOR(CLK_TOP_UNIVPLL_D5, "univpll_d5", "univpll", 1, 5),
	FACTOR(CLK_TOP_UNIVPLL_D5_D2, "univpll_d5_d2", "univpll", 1, 10),
	FACTOR(CLK_TOP_UNIVPLL_D5_D4, "univpll_d5_d4", "univpll", 1, 20),
	FACTOR(CLK_TOP_UNIVPLL_D6, "univpll_d6", "univpll", 1, 6),
	FACTOR(CLK_TOP_UNIVPLL_D6_D2, "univpll_d6_d2", "univpll", 1, 12),
	FACTOR(CLK_TOP_UNIVPLL_D6_D4, "univpll_d6_d4", "univpll", 1, 24),
	FACTOR(CLK_TOP_UNIVPLL_D6_D8, "univpll_d6_d8", "univpll", 1, 48),
	FACTOR(CLK_TOP_UNIVPLL_D6_D16, "univpll_d6_d16", "univpll", 1, 96),
	FACTOR(CLK_TOP_UNIVPLL_D7, "univpll_d7", "univpll", 1, 7),
	FACTOR(CLK_TOP_UNIVPLL_D7_D2, "univpll_d7_d2", "univpll", 1, 14),
	FACTOR(CLK_TOP_UNIVPLL_192M_D2, "univpll_192m_d2", "univpll", 1, 26),
	FACTOR(CLK_TOP_UNIVPLL_192M_D4, "univpll_192m_d4", "univpll", 1, 52),
	FACTOR(CLK_TOP_UNIVPLL_192M_D8, "univpll_192m_d8", "univpll", 1, 104),
	FACTOR(CLK_TOP_UNIVPLL_192M_D16, "univpll_192m_d16", "univpll",
	       1, 208),
	FACTOR(CLK_TOP_UNIVPLL_192M_D32, "univpll_192m_d32", "univpll",
	       1, 416),
	FACTOR(CLK_TOP_APLL1, "apll1_ck", "apll1", 1, 1),
	FACTOR(CLK_TOP_APLL1_D2, "apll1_d2", "apll1", 1, 2),
	FACTOR(CLK_TOP_APLL1_D4, "apll1_d4", "apll1", 1, 4),
	FACTOR(CLK_TOP_APLL1_D8, "apll1_d8", "apll1", 1, 8),
	FACTOR(CLK_TOP_APLL2, "apll2_ck", "apll2", 1, 1),
	FACTOR(CLK_TOP_APLL2_D2, "apll2_d2", "apll2", 1, 2),
	FACTOR(CLK_TOP_APLL2_D4, "apll2_d4", "apll2", 1, 4),
	FACTOR(CLK_TOP_APLL2_D8, "apll2_d8", "apll2", 1, 8),
	FACTOR(CLK_TOP_ADSPPLL, "adsppll_ck", "adsppll", 1, 1),
	FACTOR(CLK_TOP_MMPLL_D4, "mmpll_d4", "mmpll", 1, 4),
	FACTOR(CLK_TOP_MMPLL_D4_D2, "mmpll_d4_d2", "mmpll", 1, 8),
	FACTOR(CLK_TOP_MMPLL_D5, "mmpll_d5", "mmpll", 1, 5),
	FACTOR(CLK_TOP_MMPLL_D5_D2, "mmpll_d5_d2", "mmpll", 1, 10),
	FACTOR(CLK_TOP_MMPLL_D6, "mmpll_d6", "mmpll", 1, 6),
	FACTOR(CLK_TOP_MMPLL_D6_D2, "mmpll_d6_d2", "mmpll", 1, 12),
	FACTOR(CLK_TOP_MMPLL_D7, "mmpll_d7", "mmpll", 1, 7),
	FACTOR(CLK_TOP_MMPLL_D9, "mmpll_d9", "mmpll", 1, 9),
	FACTOR(CLK_TOP_TVDPLL, "tvdpll_ck", "tvdpll", 1, 1),
	FACTOR(CLK_TOP_TVDPLL_D2, "tvdpll_d2", "tvdpll", 1, 2),
	FACTOR(CLK_TOP_MSDCPLL, "msdcpll_ck", "msdcpll", 1, 1),
	FACTOR(CLK_TOP_MSDCPLL_D2, "msdcpll_d2", "msdcpll", 1, 2),
	FACTOR(CLK_TOP_MSDCPLL_D4, "msdcpll_d4", "msdcpll", 1, 4),
	FACTOR(CLK_TOP_CLKRTC, "clkrtc", "clk32k", 1, 1),
	FACTOR(CLK_TOP_TCK_26M_MX9, "tck_26m_mx9_ck", "clk26m", 1, 1),
	FACTOR(CLK_TOP_F26M_CK_D2, "f26m_d2", "clk26m", 1, 2),
	FACTOR(CLK_TOP_OSC, "osc_ck", "ulposc", 1, 1),
	FACTOR(CLK_TOP_OSC_D2, "osc_d2", "ulposc", 1, 2),
	FACTOR(CLK_TOP_OSC_D4, "osc_d4", "ulposc", 1, 4),
	FACTOR(CLK_TOP_OSC_D7, "osc_d7", "ulposc", 1, 7),
	FACTOR(CLK_TOP_OSC_D8, "osc_d8", "ulposc", 1, 8),
	FACTOR(CLK_TOP_OSC_D16, "osc_d16", "ulposc", 1, 16),
	FACTOR(CLK_TOP_OSC_D10, "osc_d10", "ulposc", 1, 10),
	FACTOR(CLK_TOP_ULPOSC, "ulposc_ck", "ulposc", 1, 1),
	FACTOR(CLK_TOP_F26M, "f26m_ck", "clk26m", 1, 1),
	FACTOR(CLK_TOP_AXI, "axi_ck", "axi_sel", 1, 1),
	FACTOR(CLK_TOP_DISP0, "disp0_ck", "disp0_sel", 1, 1),
	FACTOR(CLK_TOP_MDP0, "mdp0_ck", "mdp0_sel", 1, 1),
	FACTOR(CLK_TOP_IMG1, "img1_ck", "img1_sel", 1, 1),
	FACTOR(CLK_TOP_IPE, "ipe_ck", "ipe_sel", 1, 1),
	FACTOR(CLK_TOP_DPE, "dpe_ck", "dpe_sel", 1, 1),
	FACTOR(CLK_TOP_CAM, "cam_ck", "cam_sel", 1, 1),
	FACTOR(CLK_TOP_DSP, "dsp_ck", "dsp_sel", 1, 1),
	FACTOR(CLK_TOP_DSP1, "dsp1_ck", "dsp1_sel", 1, 1),
	FACTOR(CLK_TOP_DSP2, "dsp2_ck", "dsp2_sel", 1, 1),
	FACTOR(CLK_TOP_DSP4, "dsp4_ck", "dsp4_sel", 1, 1),
	FACTOR(CLK_TOP_DSP7, "dsp7_ck", "dsp7_sel", 1, 1),
	FACTOR(CLK_TOP_FUART, "fuart_ck", "uart_sel", 1, 1),
	FACTOR(CLK_TOP_SPI, "spi_ck", "spi_sel", 1, 1),
	FACTOR(CLK_TOP_MSDC50_0_HCLK, "msdc5hclk_ck", "msdc5hclk_sel", 1, 1),
	FACTOR(CLK_TOP_MSDC50_0, "msdc50_0_ck", "msdc50_0_sel", 1, 1),
	FACTOR(CLK_TOP_MSDC30_1, "msdc30_1_ck", "msdc30_1_sel", 1, 1),
	FACTOR(CLK_TOP_AUDIO, "audio_ck", "audio_sel", 1, 1),
	FACTOR(CLK_TOP_AUD_INTBUS, "aud_intbus_ck", "aud_intbus_sel", 1, 1),
	FACTOR(CLK_TOP_FPWRAP_ULPOSC, "fpwrap_ulposc_ck", "pwrap_ulposc_sel",
	       1, 1),
	FACTOR(CLK_TOP_FDISP_PWM, "fdisp_pwm_ck", "disp_pwm_sel", 1, 1),
	FACTOR(CLK_TOP_FUSB_TOP, "fusb_ck", "usb_sel", 1, 1),
	FACTOR(CLK_TOP_FSSUSB_XHCI, "fssusb_xhci_ck", "ssusb_xhci_sel", 1, 1),
	FACTOR(CLK_TOP_I2C, "i2c_ck", "i2c_sel", 1, 1),
	FACTOR(CLK_TOP_DXCC, "dxcc_ck", "dxcc_sel", 1, 1),
	FACTOR(CLK_TOP_AUD_ENGEN1, "aud_engen1_ck", "aud_engen1_sel", 1, 1),
	FACTOR(CLK_TOP_AUD_ENGEN2, "aud_engen2_ck", "aud_engen2_sel", 1, 1),
	FACTOR(CLK_TOP_AES_UFSFDE, "aes_ufsfde_ck", "aes_ufsfde_sel", 1, 1),
	FACTOR(CLK_TOP_UFS, "ufs_ck", "ufs_sel", 1, 1),
	FACTOR(CLK_TOP_AUD_1, "aud_1_ck", "aud_1_sel", 1, 1),
	FACTOR(CLK_TOP_AUD_2, "aud_2_ck", "aud_2_sel", 1, 1),
	FACTOR(CLK_TOP_ADSP, "adsp_ck", "adsp_sel", 1, 1),
	FACTOR(CLK_TOP_DPMAIF_MAIN, "dpmaif_main_ck", "dpmaif_main_sel", 1, 1),
	FACTOR(CLK_TOP_VENC, "venc_ck", "venc_sel", 1, 1),
	FACTOR(CLK_TOP_VDEC, "vdec_ck", "vdec_sel", 1, 1),
	FACTOR(CLK_TOP_PWM, "pwm_ck", "pwm_sel", 1, 1),
	FACTOR(CLK_TOP_AUDIO_H, "audio_h_ck", "audio_h_sel", 1, 1),
	FACTOR(CLK_TOP_MEM_SUB, "mem_sub_ck", "mem_sub_sel", 1, 1),
	FACTOR(CLK_TOP_AES_MSDCFDE, "aes_msdcfde_ck", "aes_msdcfde_sel", 1, 1),
	FACTOR(CLK_TOP_UFS_MBIST, "ufs_mbist_ck", "ufs_mbist_sel", 1, 1),
	FACTOR(CLK_TOP_MSDC_NEW_RX, "msdc_new_rx_ck", "msdc_new_rx_sel", 1, 1),
	FACTOR(CLK_TOP_MFG_INTERNAL1, "mfg_internal1_ck", "mfg_internal1_sel",
	       1, 1),
	FACTOR(CLK_TOP_MFG_INTERNAL2, "mfg_internal2_ck", "mfg_internal2_sel",
	       1, 1),
	FACTOR(CLK_TOP_FI2C_PSEUDO, "fi2c_pseudo_ck", "ifrao_i2c_pseudo",
	       1, 1),
};

static const char * const axi_parents[] = {
	"tck_26m_mx9_ck",
	"mainpll_d4_d4",
	"mainpll_d7_d2",
	"mainpll_d4_d2",
	"mainpll_d5_d2",
	"mainpll_d6_d2",
	"osc_d4"
};

static const char * const spm_parents[] = {
	"tck_26m_mx9_ck",
	"osc_d10",
	"mainpll_d7_d4",
	"clkrtc"
};

static const char * const scp_parents[] = {
	"tck_26m_mx9_ck",
	"univpll_d4",
	"adsppll_ck",
	"mainpll_d6",
	"univpll_d6",
	"mainpll_d4_d2",
	"apll1_ck",
	"mainpll_d7"
};

static const char * const bus_aximem_parents[] = {
	"tck_26m_mx9_ck",
	"mainpll_d7_d2",
	"mainpll_d4_d2",
	"mainpll_d5_d2",
	"mainpll_d6"
};

static const char * const disp0_parents[] = {
	"tck_26m_mx9_ck",
	"univpll_d6_d2",
	"mainpll_d5_d2",
	"mmpll_d6_d2",
	"univpll_d5_d2",
	"univpll_d4_d2",
	"mmpll_d7",
	"univpll_d6",
	"mainpll_d4",
	"tvdpll_ck",
	"mmpll_d5_d2",
	"univpll_d4"
};

static const char * const mdp0_parents[] = {
	"tck_26m_mx9_ck",
	"mainpll_d5_d2",
	"mmpll_d6_d2",
	"mainpll_d4_d2",
	"mmpll_d4_d2",
	"mainpll_d6",
	"mainpll_d5",
	"mainpll_d4",
	"tvdpll_ck",
	"univpll_d4",
	"mmpll_d5_d2"
};

static const char * const img1_parents[] = {
	"tck_26m_mx9_ck",
	"univpll_d4",
	"mainpll_d4",
	"univpll_d5",
	"mmpll_d6",
	"univpll_d6",
	"mainpll_d6",
	"mmpll_d4_d2",
	"univpll_d4_d2",
	"mainpll_d4_d2",
	"mmpll_d5_d2"
};

static const char * const ipe_parents[] = {
	"tck_26m_mx9_ck",
	"mainpll_d4",
	"mmpll_d6",
	"univpll_d6",
	"mainpll_d6",
	"mmpll_d4_d2",
	"univpll_d4_d2",
	"mainpll_d4_d2",
	"mmpll_d5_d2"
};

static const char * const dpe_parents[] = {
	"tck_26m_mx9_ck",
	"mainpll_d4",
	"mmpll_d6",
	"mainpll_d6",
	"univpll_d5_d2"
};

static const char * const cam_parents[] = {
	"tck_26m_mx9_ck",
	"mainpll_d4",
	"mmpll_d6",
	"univpll_d4",
	"univpll_d5",
	"univpll_d6",
	"mmpll_d7",
	"univpll_d4_d2",
	"mainpll_d4_d2",
	"tvdpll_d2"
};

static const char * const ccu_parents[] = {
	"tck_26m_mx9_ck",
	"mainpll_d6",
	"tvdpll_ck",
	"mainpll_d4",
	"univpll_d5",
	"mmpll_d7",
	"univpll_d4_d2",
	"mmpll_d5_d2"
};

static const char * const dsp_parents[] = {
	"tck_26m_mx9_ck",
	"univpll_d6_d2",
	"mainpll_d4_d2",
	"mmpll_d4_d2",
	"univpll_d5",
	"mmpll_d5",
	"univpll_d4",
	"mmpll_d4"
};

static const char * const dsp1_parents[] = {
	"tck_26m_mx9_ck",
	"univpll_d6_d2",
	"mainpll_d4_d2",
	"univpll_d4_d2",
	"mmpll_d4_d2",
	"univpll_d5",
	"mmpll_d5",
	"univpll_d4"
};

static const char * const dsp2_parents[] = {
	"tck_26m_mx9_ck",
	"univpll_d6_d2",
	"mainpll_d4_d2",
	"univpll_d4_d2",
	"mmpll_d4_d2",
	"univpll_d5",
	"mmpll_d5",
	"univpll_d4"
};

static const char * const dsp4_parents[] = {
	"tck_26m_mx9_ck",
	"univpll_d6_d2",
	"univpll_d4_d2",
	"mmpll_d4_d2",
	"univpll_d5",
	"mainpll_d4",
	"univpll_d4",
	"mmpll_d4"
};

static const char * const dsp7_parents[] = {
	"tck_26m_mx9_ck",
	"univpll_d6_d2",
	"mainpll_d4_d2",
	"mmpll_d4_d2",
	"univpll_d5",
	"mmpll_d5",
	"univpll_d4",
	"mmpll_d4"
};

static const char * const camtg_parents[] = {
	"tck_26m_mx9_ck",
	"univpll_192m_d8",
	"univpll_d6_d8",
	"univpll_192m_d4",
	"univpll_d6_d16",
	"f26m_d2",
	"univpll_192m_d16",
	"univpll_192m_d32"
};

static const char * const camtg2_parents[] = {
	"tck_26m_mx9_ck",
	"univpll_192m_d8",
	"univpll_d6_d8",
	"univpll_192m_d4",
	"univpll_d6_d16",
	"f26m_d2",
	"univpll_192m_d16",
	"univpll_192m_d32"
};

static const char * const camtg3_parents[] = {
	"tck_26m_mx9_ck",
	"univpll_192m_d8",
	"univpll_d6_d8",
	"univpll_192m_d4",
	"univpll_d6_d16",
	"f26m_d2",
	"univpll_192m_d16",
	"univpll_192m_d32"
};

static const char * const camtg4_parents[] = {
	"tck_26m_mx9_ck",
	"univpll_192m_d8",
	"univpll_d6_d8",
	"univpll_192m_d4",
	"univpll_d6_d16",
	"f26m_d2",
	"univpll_192m_d16",
	"univpll_192m_d32"
};

static const char * const camtg5_parents[] = {
	"tck_26m_mx9_ck",
	"univpll_192m_d8",
	"univpll_d6_d8",
	"univpll_192m_d4",
	"univpll_d6_d16",
	"f26m_d2",
	"univpll_192m_d16",
	"univpll_192m_d32"
};

static const char * const uart_parents[] = {
	"tck_26m_mx9_ck",
	"univpll_d6_d8"
};

static const char * const spi_parents[] = {
	"tck_26m_mx9_ck",
	"mainpll_d5_d4",
	"mainpll_d6_d4",
	"msdcpll_d4"
};

static const char * const msdc5hclk_parents[] = {
	"tck_26m_mx9_ck",
	"mainpll_d4_d2",
	"mainpll_d6_d2"
};

static const char * const msdc50_0_parents[] = {
	"tck_26m_mx9_ck",
	"msdcpll_ck",
	"msdcpll_d2",
	"univpll_d4_d4",
	"mainpll_d6_d2",
	"univpll_d4_d2"
};

static const char * const msdc30_1_parents[] = {
	"tck_26m_mx9_ck",
	"univpll_d6_d2",
	"mainpll_d6_d2",
	"mainpll_d7_d2",
	"msdcpll_d2"
};

static const char * const audio_parents[] = {
	"tck_26m_mx9_ck",
	"mainpll_d5_d8",
	"mainpll_d7_d8",
	"mainpll_d4_d16"
};

static const char * const aud_intbus_parents[] = {
	"tck_26m_mx9_ck",
	"mainpll_d4_d4",
	"mainpll_d7_d4"
};

static const char * const pwrap_ulposc_parents[] = {
	"osc_d10",
	"tck_26m_mx9_ck",
	"osc_d4",
	"osc_d7",
	"osc_d8",
	"osc_d16"
};

static const char * const atb_parents[] = {
	"tck_26m_mx9_ck",
	"mainpll_d4_d2",
	"mainpll_d5_d2"
};

static const char * const sspm_parents[] = {
	"tck_26m_mx9_ck",
	"mainpll_d6_d2",
	"mainpll_d5_d2",
	"mainpll_d9",
	"mainpll_d4_d2",
	"mainpll_d7"
};

static const char * const disp_pwm_parents[] = {
	"tck_26m_mx9_ck",
	"univpll_d6_d4",
	"osc_d2",
	"osc_d4",
	"osc_d16"
};

static const char * const usb_parents[] = {
	"tck_26m_mx9_ck",
	"univpll_d5_d4",
	"univpll_d6_d4",
	"univpll_d5_d2"
};

static const char * const ssusb_xhci_parents[] = {
	"tck_26m_mx9_ck",
	"univpll_d5_d4",
	"univpll_d6_d4",
	"univpll_d5_d2"
};

static const char * const i2c_parents[] = {
	"tck_26m_mx9_ck",
	"mainpll_d4_d8",
	"univpll_d5_d4"
};

static const char * const seninf_parents[] = {
	"tck_26m_mx9_ck",
	"univpll_d4_d4",
	"univpll_d6_d2",
	"univpll_d4_d2",
	"tvdpll_d2",
	"mmpll_d7",
	"mmpll_d6",
	"univpll_d5"
};

static const char * const seninf1_parents[] = {
	"tck_26m_mx9_ck",
	"univpll_d4_d4",
	"univpll_d6_d2",
	"univpll_d4_d2",
	"tvdpll_d2",
	"mmpll_d7",
	"mmpll_d6",
	"univpll_d5"
};

static const char * const seninf2_parents[] = {
	"tck_26m_mx9_ck",
	"univpll_d4_d4",
	"univpll_d6_d2",
	"univpll_d4_d2",
	"tvdpll_d2",
	"mmpll_d7",
	"mmpll_d6",
	"univpll_d5"
};

static const char * const seninf3_parents[] = {
	"tck_26m_mx9_ck",
	"univpll_d4_d4",
	"univpll_d6_d2",
	"univpll_d4_d2",
	"tvdpll_d2",
	"mmpll_d7",
	"mmpll_d6",
	"univpll_d5"
};

static const char * const dxcc_parents[] = {
	"tck_26m_mx9_ck",
	"mainpll_d4_d2",
	"mainpll_d4_d4",
	"mainpll_d4_d8"
};

static const char * const aud_engen1_parents[] = {
	"tck_26m_mx9_ck",
	"apll1_d2",
	"apll1_d4",
	"apll1_d8"
};

static const char * const aud_engen2_parents[] = {
	"tck_26m_mx9_ck",
	"apll2_d2",
	"apll2_d4",
	"apll2_d8"
};

static const char * const aes_ufsfde_parents[] = {
	"tck_26m_mx9_ck",
	"mainpll_d4",
	"mainpll_d4_d2",
	"mainpll_d6",
	"mainpll_d4_d4",
	"univpll_d4_d2",
	"univpll_d6"
};

static const char * const ufs_parents[] = {
	"tck_26m_mx9_ck",
	"mainpll_d4_d4",
	"mainpll_d4_d8",
	"univpll_d4_d4",
	"mainpll_d6_d2",
	"mainpll_d5_d2",
	"msdcpll_d2"
};

static const char * const aud_1_parents[] = {
	"tck_26m_mx9_ck",
	"apll1_ck"
};

static const char * const aud_2_parents[] = {
	"tck_26m_mx9_ck",
	"apll2_ck"
};

static const char * const adsp_parents[] = {
	"tck_26m_mx9_ck",
	"mainpll_d6",
	"mainpll_d5_d2",
	"univpll_d4_d4",
	"univpll_d4",
	"univpll_d6",
	"ulposc_ck",
	"adsppll_ck"
};

static const char * const dpmaif_main_parents[] = {
	"tck_26m_mx9_ck",
	"univpll_d4_d4",
	"mainpll_d6",
	"mainpll_d4_d2",
	"univpll_d4_d2"
};

static const char * const venc_parents[] = {
	"tck_26m_mx9_ck",
	"mmpll_d4_d2",
	"mainpll_d6",
	"univpll_d4_d2",
	"mainpll_d4_d2",
	"univpll_d6",
	"mmpll_d6",
	"mainpll_d5_d2",
	"mainpll_d6_d2",
	"mmpll_d9",
	"univpll_d4_d4",
	"mainpll_d4",
	"univpll_d4",
	"univpll_d5",
	"univpll_d5_d2",
	"mainpll_d5"
};

static const char * const vdec_parents[] = {
	"tck_26m_mx9_ck",
	"univpll_192m_d2",
	"univpll_d5_d4",
	"mainpll_d5",
	"mainpll_d5_d2",
	"mmpll_d6_d2",
	"univpll_d5_d2",
	"mainpll_d4_d2",
	"univpll_d4_d2",
	"univpll_d7",
	"mmpll_d7",
	"mmpll_d6",
	"univpll_d5",
	"mainpll_d4",
	"univpll_d4",
	"univpll_d6"
};

static const char * const camtm_parents[] = {
	"tck_26m_mx9_ck",
	"univpll_d4_d4",
	"univpll_d6_d2",
	"univpll_d6_d4"
};

static const char * const pwm_parents[] = {
	"tck_26m_mx9_ck",
	"univpll_d4_d8"
};

static const char * const audio_h_parents[] = {
	"tck_26m_mx9_ck",
	"univpll_d7_d2",
	"apll1_ck",
	"apll2_ck"
};

static const char * const mcupm_parents[] = {
	"tck_26m_mx9_ck",
	"univpll_d6_d2",
	"mainpll_d5_d2",
	"mainpll_d6_d2"
};

static const char * const spmi_m_mst_parents[] = {
	"tck_26m_mx9_ck",
	"f26m_d2",
	"osc_d8",
	"osc_d10",
	"osc_d16",
	"osc_d7",
	"clkrtc",
	"mainpll_d7_d8",
	"mainpll_d6_d8",
	"mainpll_d5_d8"
};

static const char * const dvfsrc_parents[] = {
	"tck_26m_mx9_ck",
	"osc_d10"
};

static const char * const mem_sub_parents[] = {
	"tck_26m_mx9_ck",
	"univpll_d4_d4",
	"mainpll_d6_d2",
	"mainpll_d5_d2",
	"mainpll_d4_d2",
	"mainpll_d6",
	"mmpll_d7",
	"mainpll_d5",
	"univpll_d5",
	"mainpll_d4"
};

static const char * const aes_msdcfde_parents[] = {
	"tck_26m_mx9_ck",
	"mainpll_d4_d2",
	"mainpll_d6",
	"mainpll_d4_d4",
	"univpll_d4_d2",
	"univpll_d6"
};

static const char * const ufs_mbist_parents[] = {
	"tck_26m_mx9_ck",
	"mainpll_d4_d2",
	"univpll_d4_d2",
	"tvdpll_d2"
};

static const char * const mfg_internal2_parents[] = {
	"mfg_internal1_ck",
	"mfgpll1_ck"
};

static const char * const mfg_internal1_parents[] = {
	"tck_26m_mx9_ck",
	"mfgpll4_ck"
};

static const char * const ap2conn_host_parents[] = {
	"tck_26m_mx9_ck",
	"mainpll_d7_d4"
};

static const char * const msdc_new_rx_parents[] = {
	"tck_26m_mx9_ck",
	"msdcpll_ck",
	"msdcpll_d2",
	"univpll_d4_d4",
	"mainpll_d6_d2",
	"univpll_d4_d2"
};

static const char * const apll_i2s0_mck_parents[] = {
	"aud_1_sel",
	"aud_2_sel"
};

static const char * const apll_i2s1_mck_parents[] = {
	"aud_1_sel",
	"aud_2_sel"
};

static const char * const apll_i2s2_mck_parents[] = {
	"aud_1_sel",
	"aud_2_sel"
};

static const char * const apll_i2s3_mck_parents[] = {
	"aud_1_sel",
	"aud_2_sel"
};

static const char * const apll_i2s4_mck_parents[] = {
	"aud_1_sel",
	"aud_2_sel"
};

static const char * const apll_i2s5_mck_parents[] = {
	"aud_1_sel",
	"aud_2_sel"
};

static const char * const apll_i2s6_mck_parents[] = {
	"aud_1_sel",
	"aud_2_sel"
};

static const char * const apll_i2s7_mck_parents[] = {
	"aud_1_sel",
	"aud_2_sel"
};

static const char * const apll_i2s8_mck_parents[] = {
	"aud_1_sel",
	"aud_2_sel"
};

static const char * const apll_i2s9_mck_parents[] = {
	"aud_1_sel",
	"aud_2_sel"
};

static const struct mtk_mux top_muxes[] = {
	/* CLK_CFG_0 */
	MUX_GATE_CLR_SET_UPD_FLAGS(CLK_TOP_AXI_SEL, "axi_sel",
				   axi_parents, CLK_CFG_0, CLK_CFG_0_SET,
				   CLK_CFG_0_CLR, 0, 3,
				   7, CLK_CFG_UPDATE,
				   TOP_MUX_AXI_SHIFT, CLK_IS_CRITICAL),
	MUX_GATE_CLR_SET_UPD_FLAGS(CLK_TOP_SPM_SEL, "spm_sel",
				   spm_parents, CLK_CFG_0, CLK_CFG_0_SET,
				   CLK_CFG_0_CLR, 8, 2,
				   15, CLK_CFG_UPDATE,
				   TOP_MUX_SPM_SHIFT, CLK_IS_CRITICAL),
	MUX_GATE_CLR_SET_UPD(CLK_TOP_SCP_SEL, "scp_sel",
			     scp_parents, CLK_CFG_0, CLK_CFG_0_SET,
			     CLK_CFG_0_CLR, 16, 3,
			     23, CLK_CFG_UPDATE,
			     TOP_MUX_SCP_SHIFT),
	MUX_GATE_CLR_SET_UPD_FLAGS(CLK_TOP_BUS_AXIMEM_SEL, "bus_aximem_sel",
				   bus_aximem_parents, CLK_CFG_0,
				   CLK_CFG_0_SET, CLK_CFG_0_CLR, 24, 3,
				   31, CLK_CFG_UPDATE,
				   TOP_MUX_BUS_AXIMEM_SHIFT, CLK_IS_CRITICAL),
	/* CLK_CFG_1 */
	MUX_GATE_CLR_SET_UPD(CLK_TOP_DISP0_SEL, "disp0_sel",
			     disp0_parents, CLK_CFG_1, CLK_CFG_1_SET,
			     CLK_CFG_1_CLR, 0, 4, 7, CLK_CFG_UPDATE,
			     TOP_MUX_DISP0_SHIFT),
	MUX_GATE_CLR_SET_UPD(CLK_TOP_MDP0_SEL, "mdp0_sel",
			     mdp0_parents, CLK_CFG_1, CLK_CFG_1_SET,
			     CLK_CFG_1_CLR, 8, 4, 15, CLK_CFG_UPDATE,
			     TOP_MUX_MDP0_SHIFT),
	MUX_GATE_CLR_SET_UPD(CLK_TOP_IMG1_SEL, "img1_sel",
			     img1_parents, CLK_CFG_1, CLK_CFG_1_SET,
			     CLK_CFG_1_CLR, 16, 4, 23, CLK_CFG_UPDATE,
			     TOP_MUX_IMG1_SHIFT),
	MUX_GATE_CLR_SET_UPD(CLK_TOP_IPE_SEL, "ipe_sel",
			     ipe_parents, CLK_CFG_1, CLK_CFG_1_SET,
			     CLK_CFG_1_CLR, 24, 4, 31, CLK_CFG_UPDATE,
			     TOP_MUX_IPE_SHIFT),
	/* CLK_CFG_2 */
	MUX_GATE_CLR_SET_UPD(CLK_TOP_DPE_SEL, "dpe_sel",
			     dpe_parents, CLK_CFG_2, CLK_CFG_2_SET,
			     CLK_CFG_2_CLR, 0, 3, 7, CLK_CFG_UPDATE,
			     TOP_MUX_DPE_SHIFT),
	MUX_GATE_CLR_SET_UPD(CLK_TOP_CAM_SEL, "cam_sel",
			     cam_parents, CLK_CFG_2, CLK_CFG_2_SET,
			     CLK_CFG_2_CLR, 8, 4, 15, CLK_CFG_UPDATE,
			     TOP_MUX_CAM_SHIFT),
	MUX_GATE_CLR_SET_UPD(CLK_TOP_CCU_SEL, "ccu_sel",
			     ccu_parents, CLK_CFG_2, CLK_CFG_2_SET,
			     CLK_CFG_2_CLR, 16, 3, 23, CLK_CFG_UPDATE,
			     TOP_MUX_CCU_SHIFT),
	MUX_GATE_CLR_SET_UPD(CLK_TOP_DSP_SEL, "dsp_sel",
			     dsp_parents, CLK_CFG_2, CLK_CFG_2_SET,
			     CLK_CFG_2_CLR, 24, 3, 31, CLK_CFG_UPDATE,
			     TOP_MUX_DSP_SHIFT),
	/* CLK_CFG_3 */
	MUX_GATE_CLR_SET_UPD(CLK_TOP_DSP1_SEL, "dsp1_sel",
			     dsp1_parents, CLK_CFG_3, CLK_CFG_3_SET,
			     CLK_CFG_3_CLR, 0, 3, 7, CLK_CFG_UPDATE,
			     TOP_MUX_DSP1_SHIFT),
	MUX_GATE_CLR_SET_UPD(CLK_TOP_DSP2_SEL, "dsp2_sel",
			     dsp2_parents, CLK_CFG_3, CLK_CFG_3_SET,
			     CLK_CFG_3_CLR, 8, 3, 15, CLK_CFG_UPDATE,
			     TOP_MUX_DSP2_SHIFT),
	MUX_GATE_CLR_SET_UPD(CLK_TOP_DSP4_SEL, "dsp4_sel",
			     dsp4_parents, CLK_CFG_3, CLK_CFG_3_SET,
			     CLK_CFG_3_CLR, 24, 3, 31, CLK_CFG_UPDATE,
			     TOP_MUX_DSP4_SHIFT),
	/* CLK_CFG_4 */
	MUX_GATE_CLR_SET_UPD(CLK_TOP_DSP7_SEL, "dsp7_sel",
			     dsp7_parents, CLK_CFG_4, CLK_CFG_4_SET,
			     CLK_CFG_4_CLR, 16, 3, 23, CLK_CFG_UPDATE,
			     TOP_MUX_DSP7_SHIFT),
	MUX_GATE_CLR_SET_UPD(CLK_TOP_CAMTG_SEL, "camtg_sel",
			     camtg_parents, CLK_CFG_4, CLK_CFG_4_SET,
			     CLK_CFG_4_CLR, 24, 3, 31, CLK_CFG_UPDATE,
			     TOP_MUX_CAMTG_SHIFT),
	/* CLK_CFG_5 */
	MUX_GATE_CLR_SET_UPD(CLK_TOP_CAMTG2_SEL, "camtg2_sel",
			     camtg2_parents, CLK_CFG_5, CLK_CFG_5_SET,
			     CLK_CFG_5_CLR, 0, 3, 7, CLK_CFG_UPDATE,
			     TOP_MUX_CAMTG2_SHIFT),
	MUX_GATE_CLR_SET_UPD(CLK_TOP_CAMTG3_SEL, "camtg3_sel",
			     camtg3_parents, CLK_CFG_5, CLK_CFG_5_SET,
			     CLK_CFG_5_CLR, 8, 3, 15, CLK_CFG_UPDATE,
			     TOP_MUX_CAMTG3_SHIFT),
	MUX_GATE_CLR_SET_UPD(CLK_TOP_CAMTG4_SEL, "camtg4_sel",
			     camtg4_parents, CLK_CFG_5, CLK_CFG_5_SET,
			     CLK_CFG_5_CLR, 16, 3, 23, CLK_CFG_UPDATE,
			     TOP_MUX_CAMTG4_SHIFT),
	MUX_GATE_CLR_SET_UPD(CLK_TOP_CAMTG5_SEL, "camtg5_sel",
			     camtg5_parents, CLK_CFG_5, CLK_CFG_5_SET,
			     CLK_CFG_5_CLR, 24, 3, 31, CLK_CFG_UPDATE,
			     TOP_MUX_CAMTG5_SHIFT),
	/* CLK_CFG_6 */
	MUX_GATE_CLR_SET_UPD(CLK_TOP_UART_SEL, "uart_sel",
			     uart_parents, CLK_CFG_6, CLK_CFG_6_SET,
			     CLK_CFG_6_CLR, 0, 1, 7, CLK_CFG_UPDATE,
			     TOP_MUX_UART_SHIFT),
	MUX_GATE_CLR_SET_UPD_FLAGS(CLK_TOP_SPI_SEL, "spi_sel",
				   spi_parents, CLK_CFG_6, CLK_CFG_6_SET,
				   CLK_CFG_6_CLR, 8, 2, 15, CLK_CFG_UPDATE,
				   TOP_MUX_SPI_SHIFT, CLK_IS_CRITICAL),
	MUX_GATE_CLR_SET_UPD(CLK_TOP_MSDC50_0_HCLK_SEL, "msdc5hclk_sel",
			     msdc5hclk_parents, CLK_CFG_6, CLK_CFG_6_SET,
			     CLK_CFG_6_CLR, 16, 2, 23, CLK_CFG_UPDATE,
			     TOP_MUX_MSDC50_0_HCLK_SHIFT),
	MUX_GATE_CLR_SET_UPD(CLK_TOP_MSDC50_0_SEL, "msdc50_0_sel",
			     msdc50_0_parents, CLK_CFG_6, CLK_CFG_6_SET,
			     CLK_CFG_6_CLR, 24, 3, 31, CLK_CFG_UPDATE,
			     TOP_MUX_MSDC50_0_SHIFT),
	/* CLK_CFG_7 */
	MUX_GATE_CLR_SET_UPD(CLK_TOP_MSDC30_1_SEL, "msdc30_1_sel",
			     msdc30_1_parents, CLK_CFG_7, CLK_CFG_7_SET,
			     CLK_CFG_7_CLR, 0, 3, 7, CLK_CFG_UPDATE,
			     TOP_MUX_MSDC30_1_SHIFT),
	MUX_GATE_CLR_SET_UPD(CLK_TOP_AUDIO_SEL, "audio_sel",
			     audio_parents, CLK_CFG_7, CLK_CFG_7_SET,
			     CLK_CFG_7_CLR, 8, 2, 15, CLK_CFG_UPDATE,
			     TOP_MUX_AUDIO_SHIFT),
	MUX_GATE_CLR_SET_UPD(CLK_TOP_AUD_INTBUS_SEL, "aud_intbus_sel",
			     aud_intbus_parents, CLK_CFG_7, CLK_CFG_7_SET,
			     CLK_CFG_7_CLR, 16, 2, 23, CLK_CFG_UPDATE,
			     TOP_MUX_AUD_INTBUS_SHIFT),
	MUX_GATE_CLR_SET_UPD(CLK_TOP_PWRAP_ULPOSC_SEL, "pwrap_ulposc_sel",
			     pwrap_ulposc_parents, CLK_CFG_7, CLK_CFG_7_SET,
			     CLK_CFG_7_CLR, 24, 3, 31, CLK_CFG_UPDATE1,
			     TOP_MUX_PWRAP_ULPOSC_SHIFT),
	/* CLK_CFG_8 */
	MUX_GATE_CLR_SET_UPD_FLAGS(CLK_TOP_ATB_SEL, "atb_sel",
				   atb_parents, CLK_CFG_8, CLK_CFG_8_SET,
				   CLK_CFG_8_CLR, 0, 2, 7, CLK_CFG_UPDATE1,
				   TOP_MUX_ATB_SHIFT, CLK_IS_CRITICAL),
	MUX_GATE_CLR_SET_UPD_FLAGS(CLK_TOP_SSPM_SEL, "sspm_sel",
				   sspm_parents, CLK_CFG_8, CLK_CFG_8_SET,
				   CLK_CFG_8_CLR, 8, 3, 15, CLK_CFG_UPDATE1,
				   TOP_MUX_SSPM_SHIFT, CLK_IS_CRITICAL),
	MUX_GATE_CLR_SET_UPD(CLK_TOP_DISP_PWM_SEL, "disp_pwm_sel",
			     disp_pwm_parents, CLK_CFG_8, CLK_CFG_8_SET,
			     CLK_CFG_8_CLR, 24, 3, 31, CLK_CFG_UPDATE1,
			     TOP_MUX_DISP_PWM_SHIFT),
	/* CLK_CFG_9 */
	MUX_GATE_CLR_SET_UPD(CLK_TOP_USB_TOP_SEL, "usb_sel",
			     usb_parents, CLK_CFG_9, CLK_CFG_9_SET,
			     CLK_CFG_9_CLR, 0, 2, 7, CLK_CFG_UPDATE1,
			     TOP_MUX_USB_TOP_SHIFT),
	MUX_GATE_CLR_SET_UPD(CLK_TOP_SSUSB_XHCI_SEL, "ssusb_xhci_sel",
			     ssusb_xhci_parents, CLK_CFG_9, CLK_CFG_9_SET,
			     CLK_CFG_9_CLR, 8, 2, 15, CLK_CFG_UPDATE1,
			     TOP_MUX_SSUSB_XHCI_SHIFT),
	/* CLK_CFG_10 */
	MUX_GATE_CLR_SET_UPD(CLK_TOP_I2C_SEL, "i2c_sel",
			     i2c_parents, CLK_CFG_10, CLK_CFG_10_SET,
			     CLK_CFG_10_CLR, 0, 2, 7, CLK_CFG_UPDATE1,
			     TOP_MUX_I2C_SHIFT),
	MUX_GATE_CLR_SET_UPD(CLK_TOP_SENINF_SEL, "seninf_sel",
			     seninf_parents, CLK_CFG_10, CLK_CFG_10_SET,
			     CLK_CFG_10_CLR, 8, 3, 15, CLK_CFG_UPDATE1,
			     TOP_MUX_SENINF_SHIFT),
	MUX_GATE_CLR_SET_UPD(CLK_TOP_SENINF1_SEL, "seninf1_sel",
			     seninf1_parents, CLK_CFG_10, CLK_CFG_10_SET,
			     CLK_CFG_10_CLR, 16, 3, 23, CLK_CFG_UPDATE1,
			     TOP_MUX_SENINF1_SHIFT),
	MUX_GATE_CLR_SET_UPD(CLK_TOP_SENINF2_SEL, "seninf2_sel",
			     seninf2_parents, CLK_CFG_10, CLK_CFG_10_SET,
			     CLK_CFG_10_CLR, 24, 3, 31, CLK_CFG_UPDATE1,
			     TOP_MUX_SENINF2_SHIFT),
	/* CLK_CFG_11 */
	MUX_GATE_CLR_SET_UPD(CLK_TOP_SENINF3_SEL, "seninf3_sel",
			     seninf3_parents, CLK_CFG_11, CLK_CFG_11_SET,
			     CLK_CFG_11_CLR, 0, 3, 7, CLK_CFG_UPDATE1,
			     TOP_MUX_SENINF3_SHIFT),
	MUX_GATE_CLR_SET_UPD_FLAGS(CLK_TOP_DXCC_SEL, "dxcc_sel",
				   dxcc_parents, CLK_CFG_11, CLK_CFG_11_SET,
				   CLK_CFG_11_CLR, 8, 2, 15, CLK_CFG_UPDATE1,
				   TOP_MUX_DXCC_SHIFT, CLK_IS_CRITICAL),
	MUX_GATE_CLR_SET_UPD(CLK_TOP_AUD_ENGEN1_SEL, "aud_engen1_sel",
			     aud_engen1_parents, CLK_CFG_11, CLK_CFG_11_SET,
			     CLK_CFG_11_CLR, 16, 2, 23, CLK_CFG_UPDATE1,
			     TOP_MUX_AUD_ENGEN1_SHIFT),
	MUX_GATE_CLR_SET_UPD(CLK_TOP_AUD_ENGEN2_SEL, "aud_engen2_sel",
			     aud_engen2_parents, CLK_CFG_11, CLK_CFG_11_SET,
			     CLK_CFG_11_CLR, 24, 2, 31, CLK_CFG_UPDATE1,
			     TOP_MUX_AUD_ENGEN2_SHIFT),
	/* CLK_CFG_12 */
	MUX_GATE_CLR_SET_UPD(CLK_TOP_AES_UFSFDE_SEL, "aes_ufsfde_sel",
			     aes_ufsfde_parents, CLK_CFG_12, CLK_CFG_12_SET,
			     CLK_CFG_12_CLR, 0, 3, 7, CLK_CFG_UPDATE1,
			     TOP_MUX_AES_UFSFDE_SHIFT),
	MUX_GATE_CLR_SET_UPD(CLK_TOP_UFS_SEL, "ufs_sel",
			     ufs_parents, CLK_CFG_12, CLK_CFG_12_SET,
			     CLK_CFG_12_CLR, 8, 3, 15, CLK_CFG_UPDATE1,
			     TOP_MUX_UFS_SHIFT),
	MUX_GATE_CLR_SET_UPD(CLK_TOP_AUD_1_SEL, "aud_1_sel",
			     aud_1_parents, CLK_CFG_12, CLK_CFG_12_SET,
			     CLK_CFG_12_CLR, 16, 1, 23, CLK_CFG_UPDATE1,
			     TOP_MUX_AUD_1_SHIFT),
	MUX_GATE_CLR_SET_UPD(CLK_TOP_AUD_2_SEL, "aud_2_sel",
			     aud_2_parents, CLK_CFG_12, CLK_CFG_12_SET,
			     CLK_CFG_12_CLR, 24, 1, 31, CLK_CFG_UPDATE1,
			     TOP_MUX_AUD_2_SHIFT),
	/* CLK_CFG_13 */
	MUX_GATE_CLR_SET_UPD(CLK_TOP_ADSP_SEL, "adsp_sel",
			     adsp_parents, CLK_CFG_13, CLK_CFG_13_SET,
			     CLK_CFG_13_CLR, 0, 3, 7, CLK_CFG_UPDATE1,
			     TOP_MUX_ADSP_SHIFT),
	MUX_GATE_CLR_SET_UPD(CLK_TOP_DPMAIF_MAIN_SEL, "dpmaif_main_sel",
			     dpmaif_main_parents, CLK_CFG_13, CLK_CFG_13_SET,
			     CLK_CFG_13_CLR, 8, 3, 15, CLK_CFG_UPDATE1,
			     TOP_MUX_DPMAIF_MAIN_SHIFT),
	MUX_GATE_CLR_SET_UPD(CLK_TOP_VENC_SEL, "venc_sel",
			     venc_parents, CLK_CFG_13, CLK_CFG_13_SET,
			     CLK_CFG_13_CLR, 16, 4, 23, CLK_CFG_UPDATE1,
			     TOP_MUX_VENC_SHIFT),
	MUX_GATE_CLR_SET_UPD(CLK_TOP_VDEC_SEL, "vdec_sel",
			     vdec_parents, CLK_CFG_13, CLK_CFG_13_SET,
			     CLK_CFG_13_CLR, 24, 4, 31, CLK_CFG_UPDATE1,
			     TOP_MUX_VDEC_SHIFT),
	/* CLK_CFG_14 */
	MUX_GATE_CLR_SET_UPD(CLK_TOP_CAMTM_SEL, "camtm_sel",
			     camtm_parents, CLK_CFG_14, CLK_CFG_14_SET,
			     CLK_CFG_14_CLR, 0, 2, 7, CLK_CFG_UPDATE1,
			     TOP_MUX_CAMTM_SHIFT),
	MUX_GATE_CLR_SET_UPD(CLK_TOP_PWM_SEL, "pwm_sel",
			     pwm_parents, CLK_CFG_14, CLK_CFG_14_SET,
			     CLK_CFG_14_CLR, 8, 1, 15, CLK_CFG_UPDATE1,
			     TOP_MUX_PWM_SHIFT),
	MUX_GATE_CLR_SET_UPD(CLK_TOP_AUDIO_H_SEL, "audio_h_sel",
			     audio_h_parents, CLK_CFG_14, CLK_CFG_14_SET,
			     CLK_CFG_14_CLR, 16, 2, 23, CLK_CFG_UPDATE1,
			     TOP_MUX_AUDIO_H_SHIFT),
	MUX_GATE_CLR_SET_UPD_FLAGS(CLK_TOP_MCUPM_SEL, "mcupm_sel",
				   mcupm_parents, CLK_CFG_14, CLK_CFG_14_SET,
				   CLK_CFG_14_CLR, 24, 2, 31, CLK_CFG_UPDATE1,
				   TOP_MUX_MCUPM_SHIFT, CLK_IS_CRITICAL),
	/* CLK_CFG_15 */
	MUX_GATE_CLR_SET_UPD(CLK_TOP_SPMI_M_MST_SEL, "spmi_m_mst_sel",
			     spmi_m_mst_parents, CLK_CFG_15, CLK_CFG_15_SET,
			     CLK_CFG_15_CLR, 8, 4, 15, CLK_CFG_UPDATE1,
			     TOP_MUX_SPMI_M_MST_SHIFT),
	MUX_GATE_CLR_SET_UPD_FLAGS(CLK_TOP_DVFSRC_SEL, "dvfsrc_sel",
				   dvfsrc_parents, CLK_CFG_15, CLK_CFG_15_SET,
				   CLK_CFG_15_CLR, 16, 1, 23, CLK_CFG_UPDATE2,
				   TOP_MUX_DVFSRC_SHIFT, CLK_IS_CRITICAL),
	MUX_GATE_CLR_SET_UPD_FLAGS(CLK_TOP_MEM_SUB_SEL, "mem_sub_sel",
				   mem_sub_parents, CLK_CFG_15, CLK_CFG_15_SET,
				   CLK_CFG_15_CLR, 24, 4, 31, CLK_CFG_UPDATE2,
				   TOP_MUX_MEM_SUB_SHIFT, CLK_IS_CRITICAL),
	/* CLK_CFG_16 */
	MUX_GATE_CLR_SET_UPD(CLK_TOP_AES_MSDCFDE_SEL, "aes_msdcfde_sel",
			     aes_msdcfde_parents, CLK_CFG_16, CLK_CFG_16_SET,
			     CLK_CFG_16_CLR, 0, 3, 7, CLK_CFG_UPDATE2,
			     TOP_MUX_AES_MSDCFDE_SHIFT),
	MUX_GATE_CLR_SET_UPD(CLK_TOP_UFS_MBIST_SEL, "ufs_mbist_sel",
			     ufs_mbist_parents, CLK_CFG_16, CLK_CFG_16_SET,
			     CLK_CFG_16_CLR, 24, 2, 31, CLK_CFG_UPDATE2,
			     TOP_MUX_UFS_MBIST_SHIFT),
	/* CLK_CFG_20 */
	MUX_CLR_SET_UPD(CLK_TOP_MFG_INTERNAL2_SEL, "mfg_internal2_sel",
			mfg_internal2_parents, CLK_CFG_20, CLK_CFG_20_SET,
			CLK_CFG_20_CLR, 18, 1, INV_OFS, INV_BIT),
	MUX_CLR_SET_UPD(CLK_TOP_MFG_INTERNAL1_SEL, "mfg_internal1_sel",
			mfg_internal1_parents, CLK_CFG_20, CLK_CFG_20_SET,
			CLK_CFG_20_CLR, 19, 1, INV_OFS, INV_BIT),
	/* CLK_CFG_17 */
	MUX_GATE_CLR_SET_UPD_FLAGS(CLK_TOP_AP2CONN_HOST_SEL, "ap2conn_host_sel",
				   ap2conn_host_parents, CLK_CFG_17, CLK_CFG_17_SET,
				   CLK_CFG_17_CLR, 0, 1, 7, CLK_CFG_UPDATE2,
				   TOP_MUX_AP2CONN_HOST_SHIFT, CLK_IS_CRITICAL),
	MUX_GATE_CLR_SET_UPD(CLK_TOP_MSDC_NEW_RX_SEL, "msdc_new_rx_sel",
			     msdc_new_rx_parents, CLK_CFG_17, CLK_CFG_17_SET,
			     CLK_CFG_17_CLR, 8, 3, 15, CLK_CFG_UPDATE2,
			     TOP_MUX_MSDC_NEW_RX_SHIFT),
};

static const struct mtk_composite top_composites[] = {
	/* CLK_AUDDIV_0 */
	MUX(CLK_TOP_APLL_I2S0_MCK_SEL, "apll_i2s0_mck_sel",
		apll_i2s0_mck_parents, 0x0320,
		16, 1),
	MUX(CLK_TOP_APLL_I2S1_MCK_SEL, "apll_i2s1_mck_sel",
		apll_i2s1_mck_parents, 0x0320,
		17, 1),
	MUX(CLK_TOP_APLL_I2S2_MCK_SEL, "apll_i2s2_mck_sel",
		apll_i2s2_mck_parents, 0x0320,
		18, 1),
	MUX(CLK_TOP_APLL_I2S3_MCK_SEL, "apll_i2s3_mck_sel",
		apll_i2s3_mck_parents, 0x0320,
		19, 1),
	MUX(CLK_TOP_APLL_I2S4_MCK_SEL, "apll_i2s4_mck_sel",
		apll_i2s4_mck_parents, 0x0320,
		20, 1),
	MUX(CLK_TOP_APLL_I2S5_MCK_SEL, "apll_i2s5_mck_sel",
		apll_i2s5_mck_parents, 0x0320,
		21, 1),
	MUX(CLK_TOP_APLL_I2S6_MCK_SEL, "apll_i2s6_mck_sel",
		apll_i2s6_mck_parents, 0x0320,
		22, 1),
	MUX(CLK_TOP_APLL_I2S7_MCK_SEL, "apll_i2s7_mck_sel",
		apll_i2s7_mck_parents, 0x0320,
		23, 1),
	MUX(CLK_TOP_APLL_I2S8_MCK_SEL, "apll_i2s8_mck_sel",
		apll_i2s8_mck_parents, 0x0320,
		24, 1),
	MUX(CLK_TOP_APLL_I2S9_MCK_SEL, "apll_i2s9_mck_sel",
		apll_i2s9_mck_parents, 0x0320,
		25, 1),
	/* CLK_AUDDIV_2 */
	DIV_GATE(CLK_TOP_APLL12_CK_DIV0, "apll12_div0",
		"apll_i2s0_mck_sel", 0x0320,
		0, CLK_AUDDIV_2, 8,
		0),
	DIV_GATE(CLK_TOP_APLL12_CK_DIV1, "apll12_div1",
		"apll_i2s1_mck_sel", 0x0320,
		1, CLK_AUDDIV_2, 8,
		8),
	DIV_GATE(CLK_TOP_APLL12_CK_DIV2, "apll12_div2",
		"apll_i2s2_mck_sel", 0x0320,
		2, CLK_AUDDIV_2, 8,
		16),
	DIV_GATE(CLK_TOP_APLL12_CK_DIV3, "apll12_div3",
		"apll_i2s3_mck_sel", 0x0320,
		3, CLK_AUDDIV_2, 8,
		24),
	/* CLK_AUDDIV_3 */
	DIV_GATE(CLK_TOP_APLL12_CK_DIV4, "apll12_div4",
		"apll_i2s4_mck_sel", 0x0320,
		4, CLK_AUDDIV_3, 8,
		0),
	DIV_GATE(CLK_TOP_APLL12_CK_DIVB, "apll12_divb",
		"apll12_div4", 0x0320,
		5, CLK_AUDDIV_3, 8,
		8),
	DIV_GATE(CLK_TOP_APLL12_CK_DIV5, "apll12_div5",
		"apll_i2s5_mck_sel", 0x0320,
		6, CLK_AUDDIV_3, 8,
		16),
	DIV_GATE(CLK_TOP_APLL12_CK_DIV6, "apll12_div6",
		"apll_i2s6_mck_sel", 0x0320,
		7, CLK_AUDDIV_3, 8,
		24),
	/* CLK_AUDDIV_4 */
	DIV_GATE(CLK_TOP_APLL12_CK_DIV7, "apll12_div7",
		"apll_i2s7_mck_sel", 0x0320,
		8, CLK_AUDDIV_4, 8,
		0),
	DIV_GATE(CLK_TOP_APLL12_CK_DIV8, "apll12_div8",
		"apll_i2s8_mck_sel", 0x0320,
		9, CLK_AUDDIV_4, 8,
		8),
	DIV_GATE(CLK_TOP_APLL12_CK_DIV9, "apll12_div9",
		"apll_i2s9_mck_sel", 0x0320,
		10, CLK_AUDDIV_4, 8,
		16),
};

static const struct mtk_clk_desc topck_desc = {
	.factor_clks = top_divs,
	.num_factor_clks = ARRAY_SIZE(top_divs),
	.mux_clks = top_muxes,
	.num_mux_clks = ARRAY_SIZE(top_muxes),
	.composite_clks = top_composites,
	.num_composite_clks = ARRAY_SIZE(top_composites),
	.clk_lock = &mt6877_clk_lock,
};

static const struct of_device_id of_match_clk_mt6877_topckgen[] = {
	{ .compatible = "mediatek,mt6877-topckgen", .data = &topck_desc },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, of_match_clk_mt6877_topckgen);

static struct platform_driver clk_mt6877_topckgen_drv = {
	.probe = mtk_clk_simple_probe,
	.remove = mtk_clk_simple_remove,
	.driver = {
		.name = "clk-mt6877-topckgen",
		.of_match_table = of_match_clk_mt6877_topckgen,
	},
};
module_platform_driver(clk_mt6877_topckgen_drv);

MODULE_DESCRIPTION("MediaTek MT6877 top clock generators driver");
MODULE_LICENSE("GPL");
