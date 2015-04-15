
/*
 * Copyright (C) 2012 Freescale Semiconductor, Inc. All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <mach/iomux-mx6dl.h>

static iomux_v3_cfg_t mx6q_ar6mx_pads[] = {
	/* AUDMUX */
	MX6Q_PAD_CSI0_DAT7__AUDMUX_AUD3_RXD,
	MX6Q_PAD_CSI0_DAT4__AUDMUX_AUD3_TXC,
	MX6Q_PAD_CSI0_DAT5__AUDMUX_AUD3_TXD,
	MX6Q_PAD_CSI0_DAT6__AUDMUX_AUD3_TXFS,

	/* CCM  */
	MX6Q_PAD_CSI0_MCLK__CCM_CLKO,
	MX6Q_PAD_NANDF_CS2__CCM_CLKO2,

	/* ECSPI 3 */
	MX6Q_PAD_DISP0_DAT2__ECSPI3_MISO,
	MX6Q_PAD_DISP0_DAT1__ECSPI3_MOSI,
	MX6Q_PAD_DISP0_DAT0__ECSPI3_SCLK,
	MX6Q_PAD_DISP0_DAT3__ECSPI3_SS0,

	MX6Q_PAD_DISP0_DAT3__GPIO_4_24,

	/* ENET */
	MX6Q_PAD_ENET_MDIO__ENET_MDIO,
	MX6Q_PAD_ENET_MDC__ENET_MDC,
	MX6Q_PAD_ENET_REF_CLK__ENET_TX_CLK,
	MX6Q_PAD_RGMII_TXC__ENET_RGMII_TXC,
	MX6Q_PAD_RGMII_TD0__ENET_RGMII_TD0,
	MX6Q_PAD_RGMII_TD1__ENET_RGMII_TD1,
	MX6Q_PAD_RGMII_TD2__ENET_RGMII_TD2,
	MX6Q_PAD_RGMII_TD3__ENET_RGMII_TD3,
	MX6Q_PAD_RGMII_TX_CTL__ENET_RGMII_TX_CTL,
	MX6Q_PAD_RGMII_RXC__ENET_RGMII_RXC,
	MX6Q_PAD_RGMII_RD0__ENET_RGMII_RD0,
	MX6Q_PAD_RGMII_RD1__ENET_RGMII_RD1,
	MX6Q_PAD_RGMII_RD2__ENET_RGMII_RD2,
	MX6Q_PAD_RGMII_RD3__ENET_RGMII_RD3,
	MX6Q_PAD_RGMII_RX_CTL__ENET_RGMII_RX_CTL,

	/* GPIO1 */
	MX6Q_PAD_GPIO_1__USBOTG_ID,		/* OTG_ID */
	MX6Q_PAD_GPIO_2__GPIO_1_2,        /* SW_RST     */
	MX6Q_PAD_GPIO_4__GPIO_1_4,        /* STATUS_LED */
	MX6Q_PAD_GPIO_5__GPIO_1_5,        /* SATA_LED   */
	MX6Q_PAD_GPIO_7__GPIO_1_7,        /* OTG_PW_EN  */
	MX6Q_PAD_GPIO_8__GPIO_1_8,        /* RTC_INT    */
	MX6Q_PAD_GPIO_9__GPIO_1_9,        /* MIC_DET    */
	MX6Q_PAD_SD2_CLK__GPIO_1_10,      /* L1_PWREN   */
	MX6Q_PAD_SD2_CMD__GPIO_1_11,      /* L0_PWREN   */
	MX6Q_PAD_SD2_DAT3__GPIO_1_12,     /* BL1EN_GPO  */
	MX6Q_PAD_SD2_DAT2__GPIO_1_13,     /* BL0EN_GPO  */
	MX6Q_PAD_SD2_DAT1__GPIO_1_14,     /* BL1_PWREN  */
	MX6Q_PAD_SD2_DAT0__GPIO_1_15,     /* BL0_PWREN  */
	MX6Q_PAD_ENET_CRS_DV__GPIO_1_25,  /* GE_nRST    */
	MX6Q_PAD_ENET_RXD1__GPIO_1_26,    /* CSI_RST_B  */
	MX6Q_PAD_ENET_TX_EN__GPIO_1_28,   /* PHY_INT    */

	/* GPIO2 */
	MX6Q_PAD_NANDF_D0__GPIO_2_0,
	MX6Q_PAD_NANDF_D1__GPIO_2_1,
	MX6Q_PAD_NANDF_D2__GPIO_2_2,
	MX6Q_PAD_NANDF_D3__GPIO_2_3,
	MX6Q_PAD_NANDF_D4__GPIO_2_4,
	MX6Q_PAD_NANDF_D5__GPIO_2_5,
	MX6Q_PAD_NANDF_D6__GPIO_2_6,
	MX6Q_PAD_NANDF_D7__GPIO_2_7,

	/* GPIO4 */
	MX6Q_PAD_DISP0_DAT4__GPIO_4_25,
	MX6Q_PAD_DISP0_DAT5__GPIO_4_26,
	MX6Q_PAD_DISP0_DAT6__GPIO_4_27,
	MX6Q_PAD_DISP0_DAT7__GPIO_4_28,

	/* GPIO5 */
	MX6Q_PAD_DISP0_DAT13__GPIO_5_7,   /* USB1_OC    */
	MX6Q_PAD_DISP0_DAT14__GPIO_5_8,   /* USB2_OC    */
	MX6Q_PAD_DISP0_DAT16__GPIO_5_10,  /* USB3_OC    */
	MX6Q_PAD_DISP0_DAT19__GPIO_5_13,  /* USBV1_EN   */
	MX6Q_PAD_DISP0_DAT20__GPIO_5_14,  /* USBV2_EN   */

	/* GPIO6 */
	MX6Q_PAD_CSI0_DAT14__GPIO_6_0,    /* CSI_PWN    */

	/* GPIO7 */
	MX6Q_PAD_SD3_DAT5__GPIO_7_0,      /* SD3_CD_B   */
	MX6Q_PAD_SD3_DAT4__GPIO_7_1,      /* SD3_WP     */
	MX6Q_PAD_SD3_RST__GPIO_7_8,       /* SPK_DET    */
	MX6Q_PAD_SD3_RST__GPIO_7_8,       /* SPK_DET    */
	MX6Q_PAD_GPIO_17__GPIO_7_12,      /* PCIE_RST_B */

	/* I2C1 */
	MX6Q_PAD_CSI0_DAT9__I2C1_SCL,
	MX6Q_PAD_CSI0_DAT8__I2C1_SDA,

	/* I2C2 */
	MX6Q_PAD_KEY_COL3__I2C2_SCL,
	MX6Q_PAD_KEY_ROW3__I2C2_SDA,

	/* I2C3 */
	MX6Q_PAD_GPIO_3__I2C3_SCL,
	MX6Q_PAD_GPIO_6__I2C3_SDA,

	/* PWM1 */
	MX6Q_PAD_DISP0_DAT8__PWM1_PWMO,

	/* PWM2 */
	MX6Q_PAD_DISP0_DAT9__PWM2_PWMO,

	/* PWM4 */
	MX6Q_PAD_SD1_CMD__PWM4_PWMO,

	/* BT */
	MX6Q_PAD_EIM_DA0__SRC_BT_CFG_0,
	MX6Q_PAD_EIM_DA1__SRC_BT_CFG_1,
	MX6Q_PAD_EIM_DA10__SRC_BT_CFG_10,
	MX6Q_PAD_EIM_DA11__SRC_BT_CFG_11,
	MX6Q_PAD_EIM_DA12__SRC_BT_CFG_12,
	MX6Q_PAD_EIM_DA13__SRC_BT_CFG_13,
	MX6Q_PAD_EIM_DA14__SRC_BT_CFG_14,
	MX6Q_PAD_EIM_DA15__SRC_BT_CFG_15,
	MX6Q_PAD_EIM_A16__SRC_BT_CFG_16,
	MX6Q_PAD_EIM_A17__SRC_BT_CFG_17,
	MX6Q_PAD_EIM_A18__SRC_BT_CFG_18,
	MX6Q_PAD_EIM_A19__SRC_BT_CFG_19,
	MX6Q_PAD_EIM_DA2__SRC_BT_CFG_2,
	MX6Q_PAD_EIM_A20__SRC_BT_CFG_20,
	MX6Q_PAD_EIM_A21__SRC_BT_CFG_21,
	MX6Q_PAD_EIM_A22__SRC_BT_CFG_22,
	MX6Q_PAD_EIM_A23__SRC_BT_CFG_23,
	MX6Q_PAD_EIM_A24__SRC_BT_CFG_24,
	MX6Q_PAD_EIM_WAIT__SRC_BT_CFG_25,
	MX6Q_PAD_EIM_LBA__SRC_BT_CFG_26,
	MX6Q_PAD_EIM_EB0__SRC_BT_CFG_27,
	MX6Q_PAD_EIM_EB1__SRC_BT_CFG_28,
	MX6Q_PAD_EIM_RW__SRC_BT_CFG_29,
	MX6Q_PAD_EIM_DA3__SRC_BT_CFG_3,
	MX6Q_PAD_EIM_EB2__SRC_BT_CFG_30,
	MX6Q_PAD_EIM_DA4__SRC_BT_CFG_4,
	MX6Q_PAD_EIM_DA5__SRC_BT_CFG_5,
	MX6Q_PAD_EIM_DA6__SRC_BT_CFG_6,
	MX6Q_PAD_EIM_DA7__SRC_BT_CFG_7,
	MX6Q_PAD_EIM_DA8__SRC_BT_CFG_8,
	MX6Q_PAD_EIM_DA9__SRC_BT_CFG_9,

	/* UART 1 */
	MX6Q_PAD_EIM_D19__UART1_CTS,
	MX6Q_PAD_EIM_D23__UART1_DCD,
	MX6Q_PAD_EIM_D25__UART1_DSR,
	MX6Q_PAD_EIM_D24__UART1_DTR,
	MX6Q_PAD_EIM_EB3__UART1_RI,
	MX6Q_PAD_EIM_D20__UART1_RTS,
	MX6Q_PAD_SD3_DAT6__UART1_RXD,
	MX6Q_PAD_SD3_DAT7__UART1_TXD,

	/* UART 2 */
	MX6Q_PAD_EIM_D28__UART2_CTS,
	MX6Q_PAD_EIM_D29__UART2_RTS,

	MX6Q_PAD_EIM_D27__UART2_RXD,
	MX6Q_PAD_EIM_D26__UART2_TXD,

	/* UART 4 */
	MX6Q_PAD_KEY_ROW0__UART4_RXD,
	MX6Q_PAD_KEY_COL0__UART4_TXD,

	/* UART 5 */
	MX6Q_PAD_KEY_COL4__UART5_RTS,
	MX6Q_PAD_KEY_ROW1__UART5_RXD,
	MX6Q_PAD_KEY_COL1__UART5_TXD,

	#if 1
	/* USDHC 3 */
	MX6Q_PAD_SD3_CLK__USDHC3_CLK_50MHZ,
	MX6Q_PAD_SD3_CMD__USDHC3_CMD_50MHZ,
	MX6Q_PAD_SD3_DAT0__USDHC3_DAT0_50MHZ,
	MX6Q_PAD_SD3_DAT1__USDHC3_DAT1_50MHZ,
	MX6Q_PAD_SD3_DAT2__USDHC3_DAT2_50MHZ,
	MX6Q_PAD_SD3_DAT3__USDHC3_DAT3_50MHZ,
	MX6Q_PAD_SD3_DAT5__GPIO_7_0,		/* J18 - SD3_CD */
	MX6Q_PAD_SD3_DAT4__GPIO_7_1,

	/* USDHC 4 */
	MX6Q_PAD_SD4_CLK__USDHC4_CLK_50MHZ,
	MX6Q_PAD_SD4_CMD__USDHC4_CMD_50MHZ,
	MX6Q_PAD_SD4_DAT0__USDHC4_DAT0_50MHZ,
	MX6Q_PAD_SD4_DAT1__USDHC4_DAT1_50MHZ,
	MX6Q_PAD_SD4_DAT2__USDHC4_DAT2_50MHZ,
	MX6Q_PAD_SD4_DAT3__USDHC4_DAT3_50MHZ,
	MX6Q_PAD_SD4_DAT4__USDHC4_DAT4_50MHZ,
	MX6Q_PAD_SD4_DAT5__USDHC4_DAT5_50MHZ,
	MX6Q_PAD_SD4_DAT6__USDHC4_DAT6_50MHZ,
	MX6Q_PAD_SD4_DAT7__USDHC4_DAT7_50MHZ,
	#endif

	/* WDOG 2 */
	MX6Q_PAD_SD1_DAT3__WDOG2_WDOG_RST_B_DEB,

	/* HDMI CEC */
	MX6Q_PAD_KEY_ROW2__HDMI_TX_CEC_LINE,
};

#define MX6Q_USDHC_PAD_SETTING(id, speed)	\
mx6q_sd##id##_##speed##mhz[] = {		\
	MX6Q_PAD_SD##id##_CLK__USDHC##id##_CLK_##speed##MHZ,	\
	MX6Q_PAD_SD##id##_CMD__USDHC##id##_CMD_##speed##MHZ,	\
	MX6Q_PAD_SD##id##_DAT0__USDHC##id##_DAT0_##speed##MHZ,	\
	MX6Q_PAD_SD##id##_DAT1__USDHC##id##_DAT1_##speed##MHZ,	\
	MX6Q_PAD_SD##id##_DAT2__USDHC##id##_DAT2_##speed##MHZ,	\
	MX6Q_PAD_SD##id##_DAT3__USDHC##id##_DAT3_##speed##MHZ,	\
}

static iomux_v3_cfg_t MX6Q_USDHC_PAD_SETTING(3, 50);
static iomux_v3_cfg_t MX6Q_USDHC_PAD_SETTING(3, 100);
static iomux_v3_cfg_t MX6Q_USDHC_PAD_SETTING(3, 200);

#define MX6Q_EMMC_PAD_SETTING(id, speed)	\
mx6q_sd##id##_##speed##mhz[] = {		\
	MX6Q_PAD_SD##id##_CLK__USDHC##id##_CLK_##speed##MHZ,	\
	MX6Q_PAD_SD##id##_CMD__USDHC##id##_CMD_##speed##MHZ,	\
	MX6Q_PAD_SD##id##_DAT0__USDHC##id##_DAT0_##speed##MHZ,	\
	MX6Q_PAD_SD##id##_DAT1__USDHC##id##_DAT1_##speed##MHZ,	\
	MX6Q_PAD_SD##id##_DAT2__USDHC##id##_DAT2_##speed##MHZ,	\
	MX6Q_PAD_SD##id##_DAT3__USDHC##id##_DAT3_##speed##MHZ,	\
	MX6Q_PAD_SD##id##_DAT4__USDHC##id##_DAT4_##speed##MHZ,	\
	MX6Q_PAD_SD##id##_DAT5__USDHC##id##_DAT5_##speed##MHZ,	\
	MX6Q_PAD_SD##id##_DAT6__USDHC##id##_DAT6_##speed##MHZ,	\
	MX6Q_PAD_SD##id##_DAT7__USDHC##id##_DAT7_##speed##MHZ,	\
}

static iomux_v3_cfg_t MX6Q_EMMC_PAD_SETTING(4, 50);
static iomux_v3_cfg_t MX6Q_EMMC_PAD_SETTING(4, 100);
static iomux_v3_cfg_t MX6Q_EMMC_PAD_SETTING(4, 200);

static iomux_v3_cfg_t mx6q_ar6mx_hdmi_ddc_pads[] = {
	MX6Q_PAD_KEY_COL3__HDMI_TX_DDC_SCL, /* HDMI DDC SCL */
	MX6Q_PAD_KEY_ROW3__HDMI_TX_DDC_SDA, /* HDMI DDC SDA */
};

static iomux_v3_cfg_t mx6q_ar6mx_i2c2_pads[] = {
	MX6Q_PAD_KEY_COL3__I2C2_SCL,	/* I2C2 SCL */
	MX6Q_PAD_KEY_ROW3__I2C2_SDA,	/* I2C2 SDA */
};

static iomux_v3_cfg_t mx6q_ar6mx_uart2_rev03_pads[] = {
	MX6Q_PAD_EIM_D26__UART2_RXD,
	MX6Q_PAD_EIM_D27__UART2_TXD,
};

static iomux_v3_cfg_t mx6q_ar6mx_uart1_rev03_pads[] ={
        MX6Q_PAD_SD3_DAT7__UART1_RXD,
        MX6Q_PAD_SD3_DAT6__UART1_TXD,
};

