/*
 * Copyright (C) 2011-2012 Freescale Semiconductor, Inc. All Rights Reserved.
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

#include <linux/types.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/pm.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/init.h>
#include <linux/input.h>
#include <linux/nodemask.h>
#include <linux/clk.h>
#include <linux/platform_device.h>
#include <linux/fsl_devices.h>
#include <linux/spi/spi.h>
#include <linux/spi/flash.h>
#include <linux/mtd/physmap.h>
#include <linux/i2c.h>
#include <linux/i2c/pca953x.h>
#include <linux/platform_data/atmel_mxt_ts.h>
#include <linux/ata.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/map.h>
#include <linux/mtd/partitions.h>
#include <linux/pmic_external.h>
#include <linux/pmic_status.h>
#include <linux/ipu.h>
#include <linux/mxcfb.h>
#include <linux/pwm_backlight.h>
#include <linux/fec.h>
#include <linux/memblock.h>
#include <linux/gpio.h>
#include <linux/ion.h>
#include <linux/etherdevice.h>
#include <linux/regulator/anatop-regulator.h>
#include <linux/regulator/consumer.h>
#include <linux/regulator/machine.h>
#include <linux/regulator/fixed.h>
#include <linux/mxc_asrc.h>
#include <sound/pcm.h>
#include <linux/mfd/mxc-hdmi-core.h>

#include <mach/common.h>
#include <mach/hardware.h>
#include <mach/mxc_dvfs.h>
#include <mach/memory.h>
#include <mach/iomux-mx6q.h>
#include <mach/imx-uart.h>
#include <mach/viv_gpu.h>
#include <mach/ahci_sata.h>
#include <mach/ipu-v3.h>
#include <mach/mxc_hdmi.h>
#include <mach/mxc_asrc.h>
#include <mach/mipi_dsi.h>
#include <mach/mipi_csi2.h>

#include <asm/irq.h>
#include <asm/setup.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/mach/time.h>

#include "usb.h"
#include "devices-imx6q.h"
#include "crm_regs.h"
#include "cpu_op-mx6.h"
#include "board-mx6q_ar6mx.h"
#include "board-mx6solo_ar6mx.h"

#define AR6MX_SD3_CD		IMX_GPIO_NR(7, 0)
#define AR6MX_SD3_WP		IMX_GPIO_NR(7, 1)
#define AR6MX_ECSPI3_CS0	IMX_GPIO_NR(4, 24)
#define AR6MX_USB_V1_PWR	IMX_GPIO_NR(5, 13)
#define AR6MX_USB_V2_PWR    IMX_GPIO_NR(5, 14)
#define AR6MX_STATUS_LED	IMX_GPIO_NR(1, 4)
#define AR6MX_SATA_LED		IMX_GPIO_NR(1, 5)
#define AR6MX_LVDS0_PWR     IMX_GPIO_NR(1, 11)
#define AR6MX_LVDS1_PWR     IMX_GPIO_NR(1, 10)
#define AR6MX_BL0_PWR       IMX_GPIO_NR(1, 15)
#define AR6MX_BL1_PWR       IMX_GPIO_NR(1, 14)
#define AR6MX_BL0_EN        IMX_GPIO_NR(1, 13)
#define AR6MX_BL1_EN        IMX_GPIO_NR(1, 12)
#define AR6MX_SPK_DET	    IMX_GPIO_NR(7, 8)
#define AR6MX_MIC_DET	    IMX_GPIO_NR(1, 9)

#define AR6MX_PCIE_RST_B    IMX_GPIO_NR(7, 12)

#define AR6MX_RTC_INT       IMX_GPIO_NR(1, 8)

#define AR6MX_VER_B0	    IMX_GPIO_NR(4, 25)
#define AR6MX_VER_B1	    IMX_GPIO_NR(4, 26)
#define AR6MX_VER_B2	    IMX_GPIO_NR(4, 27)
#define AR6MX_VER_B3	    IMX_GPIO_NR(4, 28)

#define AR6MX_OTG_PWR_EN    IMX_GPIO_NR(1, 7)

#define AR6MX_TTL_DI0	    IMX_GPIO_NR(2, 0)
#define AR6MX_TTL_DI1	    IMX_GPIO_NR(2, 1)
#define AR6MX_TTL_DI2	    IMX_GPIO_NR(2, 2)
#define AR6MX_TTL_DI3	    IMX_GPIO_NR(2, 3)
#define AR6MX_TTL_DI4	    IMX_GPIO_NR(2, 4)
#define AR6MX_TTL_DI5       IMX_GPIO_NR(2, 5)
#define AR6MX_TTL_DO0	    IMX_GPIO_NR(2, 6)
#define AR6MX_TTL_DO1       IMX_GPIO_NR(2, 7)

/* For I2C Touchscreen mrobbeloth PDi*/
#define AR6MX_CAP_TCH_INT0       IMX_GPIO_NR(6, 8)
#define ATMEL_MXT_I2C_ADDR 0x4A

/* PDi defined GPIO */
#define AR6MX_TV_POWER_REQ	      AR6MX_TTL_DI0
#define AR6MX_AIO_POWER_REQ	      AR6MX_TTL_DI0
#define AR6MX_TV_ARROW_UP	      AR6MX_TTL_DI1
#define AR6MX_AIO_VOL_UP	      AR6MX_TTL_DI1
#define AR6MX_TV_ARROW_DOWN	      AR6MX_TTL_DI2
#define AR6MX_AIO_VOL_DOWN	      AR6MX_TTL_DI2
#define AR6MX_TV_ARROW_LEFT	      AR6MX_TTL_DI3
#define AR6MX_TV_OR_AIO		      AR6MX_TTL_DI5 // float high for TV/Tab, pull low for all-in-one -JTS
#define AR6MX_ANDROID_PWRSTATE        AR6MX_TTL_DO0
#define AR6MX_INTERNAL_SPK_ENABLE     AR6MX_TTL_DO1
#define AR6MX_HDMIHPD_MAPPING	      IMX_GPIO_NR(2,4)  // recall in mxc_hdmi.c

/* PDi defined GPIO for OV5640 Camera on CSI MIPI CN4 port 

   Remember to use iomux-mx6q.h to look up mapping */

/* MX6Q_PAD_ENET_RXD1__GPIO_1_26 */
#define AR6MX_MIPICSI_RST	IMX_GPIO_NR(1, 26)

/*_MX6Q_PAD_CSI0_DAT14__GPIO_6_0 */
#define AR6MX_MIPICSI_PWN   IMX_GPIO_NR(6, 0)

extern char *gp_reg_id;
extern char *soc_reg_id;
extern char *pu_reg_id;

static struct clk *sata_clk;
static int spinor_en;
static int board_id;
static int emmc_en;

static void pcie_3v3_reset(void)
{
        /* reset miniPCIe */
        gpio_request(AR6MX_PCIE_RST_B, "pcie_reset_rebB");
        gpio_direction_output(AR6MX_PCIE_RST_B, 0);
        /* The PCI Express Mini CEM specification states that PREST# is
        deasserted minimum 1ms after 3.3vVaux has been applied and stable*/
        mdelay(1);
        gpio_set_value(AR6MX_PCIE_RST_B, 1);
        gpio_free(AR6MX_PCIE_RST_B);
}

static int __init spinor_enable(char *p)
{
		spinor_en = 1;
		return 0;
}
early_param("spi-nor", spinor_enable);

static int __init emmc_enable(char *p)
{
       emmc_en = 1;
       return 0;
}
early_param("emmc_en", emmc_enable);

enum sd_pad_mode {
	SD_PAD_MODE_LOW_SPEED,
	SD_PAD_MODE_MED_SPEED,
	SD_PAD_MODE_HIGH_SPEED,
};



#define GPIO_BUTTON(gpio_num, ev_code, act_low, descr, wake)	\
{								\
	.gpio		= gpio_num,				\
	.type		= EV_KEY,				\
	.code		= ev_code,				\
	.active_low	= act_low,				\
	.desc		= "btn " descr,				\
	.wakeup		= wake,					\
}

/* GPIO map for TV's and Tab's -JTS */
static struct gpio_keys_button ard_buttons_tv[] = {
	GPIO_BUTTON(AR6MX_TV_POWER_REQ,  KEY_POWER, 1, "power", 1),
	GPIO_BUTTON(AR6MX_TV_ARROW_UP,   KEY_F16,   1, "F16",   0),  // "arrow up" key used for recovery "navigate up"
	GPIO_BUTTON(AR6MX_TV_ARROW_DOWN, KEY_F17,   1, "F17",   0),  // "arrow down" key used for recovery "navigate down"
	GPIO_BUTTON(AR6MX_TV_ARROW_LEFT, KEY_F18,   1, "F18",   0)   // "arrow left" key used for recovery "select"
};

/* GPIO map for all-in-one's -JTS */
static struct gpio_keys_button ard_buttons_aio[] = {
	GPIO_BUTTON(AR6MX_AIO_POWER_REQ, KEY_POWER, 	 1, "power",       1),
	GPIO_BUTTON(AR6MX_AIO_VOL_UP,    KEY_VOLUMEUP,   1, "volume-up",   0),  
	GPIO_BUTTON(AR6MX_AIO_VOL_DOWN,  KEY_VOLUMEDOWN, 1, "volume-down", 0)  
};

static struct gpio_keys_platform_data ard_android_button_data_tv = {
	.buttons	= ard_buttons_tv,
	.nbuttons	= ARRAY_SIZE(ard_buttons_tv)
};

static struct gpio_keys_platform_data ard_android_button_data_aio = {
	.buttons	= ard_buttons_aio,
	.nbuttons	= ARRAY_SIZE(ard_buttons_aio)
};

static struct platform_device ard_android_button_device_tv = {
	.name		= "gpio-keys",
	.id		= -1,
	.num_resources  = 0,
	.dev		= {
		.platform_data = &ard_android_button_data_tv,
	}
};

static struct platform_device ard_android_button_device_aio = {
	.name		= "gpio-keys",
	.id		= -1,
	.num_resources  = 0,
	.dev		= {
		.platform_data = &ard_android_button_data_aio,
	}
};

static void __init imx6q_add_android_device_buttons(void)
{
	// Setup TV arrow keys if AR6MX_TV_OR_AIO input is floating high, or P19X volume keys if pulled low
	if (gpio_get_value(AR6MX_TV_OR_AIO)) {
		platform_device_register(&ard_android_button_device_tv);
	} else {
		platform_device_register(&ard_android_button_device_aio);
	}
}

static int plt_sd_pad_change(unsigned int index, int clock)
{
	/* LOW speed is the default state of SD pads */
	static enum sd_pad_mode pad_mode = SD_PAD_MODE_LOW_SPEED;

	iomux_v3_cfg_t *sd_pads_200mhz = NULL;
	iomux_v3_cfg_t *sd_pads_100mhz = NULL;
	iomux_v3_cfg_t *sd_pads_50mhz = NULL;

	u32 sd_pads_200mhz_cnt;
	u32 sd_pads_100mhz_cnt;
	u32 sd_pads_50mhz_cnt;

	switch (index) {
	case 2:
		sd_pads_200mhz = mx6dl_sd3_200mhz;
		sd_pads_100mhz = mx6dl_sd3_100mhz;
		sd_pads_50mhz = mx6dl_sd3_50mhz;

		sd_pads_200mhz_cnt = ARRAY_SIZE(mx6dl_sd3_200mhz);
		sd_pads_100mhz_cnt = ARRAY_SIZE(mx6dl_sd3_100mhz);
		sd_pads_50mhz_cnt = ARRAY_SIZE(mx6dl_sd3_50mhz);
		break;
	case 3:
		sd_pads_200mhz = mx6dl_sd4_200mhz;
		sd_pads_100mhz = mx6dl_sd4_100mhz;
		sd_pads_50mhz = mx6dl_sd4_50mhz;

		sd_pads_200mhz_cnt = ARRAY_SIZE(mx6dl_sd4_200mhz);
		sd_pads_100mhz_cnt = ARRAY_SIZE(mx6dl_sd4_100mhz);
		sd_pads_50mhz_cnt = ARRAY_SIZE(mx6dl_sd4_50mhz);
		break;
	default:
		printk(KERN_ERR "no such SD host controller index %d\n", index);
		return -EINVAL;
	}

	if (clock > 100000000) {
		if (pad_mode == SD_PAD_MODE_HIGH_SPEED)
			return 0;
		BUG_ON(!sd_pads_200mhz);
		pad_mode = SD_PAD_MODE_HIGH_SPEED;
		return mxc_iomux_v3_setup_multiple_pads(sd_pads_200mhz,
							sd_pads_200mhz_cnt);
	} else if (clock > 52000000) {
		if (pad_mode == SD_PAD_MODE_MED_SPEED)
			return 0;
		BUG_ON(!sd_pads_100mhz);
		pad_mode = SD_PAD_MODE_MED_SPEED;
		return mxc_iomux_v3_setup_multiple_pads(sd_pads_100mhz,
							sd_pads_100mhz_cnt);
	} else {
		if (pad_mode == SD_PAD_MODE_LOW_SPEED)
			return 0;
		BUG_ON(!sd_pads_50mhz);
		pad_mode = SD_PAD_MODE_LOW_SPEED;
		return mxc_iomux_v3_setup_multiple_pads(sd_pads_50mhz,
							sd_pads_50mhz_cnt);
	}
}

static const struct esdhc_platform_data mx6q_ar6mx_sd3_data __initconst = {
	.cd_gpio = AR6MX_SD3_CD,
	.wp_gpio = AR6MX_SD3_WP,
	.keep_power_at_suspend = 1,
	.platform_pad_change = plt_sd_pad_change,
};

static const struct esdhc_platform_data mx6q_ar6mx_sd4_data __initconst = {
	.always_present = 1,
	.support_8bit = 1,
	.keep_power_at_suspend = 1,
	.platform_pad_change = plt_sd_pad_change,
};

static const struct anatop_thermal_platform_data
mx6q_ar6mx_anatop_thermal_data __initconst = {
	.name = "anatop_thermal",
};

static const struct imxuart_platform_data ar6mx_uart2_data = {
	.flags = IMXUART_HAVE_RTSCTS | IMXUART_SDMA,
	.dma_req_tx = MX6Q_DMA_REQ_UART2_TX,
	.dma_req_rx = MX6Q_DMA_REQ_UART2_RX,
};

static const struct imxuart_platform_data ar6mx_uart1_alt_data = {
	.flags = IMXUART_USE_DCEDTE | IMXUART_SDMA,
	.dma_req_tx = MX6Q_DMA_REQ_UART1_TX,
	.dma_req_rx = MX6Q_DMA_REQ_UART1_RX,
};

static const struct imxuart_platform_data ar6mx_uart1_data = {
        .flags = IMXUART_HAVE_RTSCTS | IMXUART_USE_DCEDTE | IMXUART_SDMA,
        .dma_req_tx = MX6Q_DMA_REQ_UART1_TX,
        .dma_req_rx = MX6Q_DMA_REQ_UART1_RX,
};

static inline void mx6q_ar6mx_init_uart(void)
{
	/* board_id == 0x0F for BCM Rev 0.1 and Rev 0.3 boards */
	/* board_id == 0x01 for BCM Rev 1.0 and Rev 2.0 boards */
	if (0xF == board_id) {
		/* Possible quad core 0.3 board, only a few prototypes 
            existed and they should have been all retired*/
		if (cpu_is_mx6q()) {
			printk(KERN_INFO "Configuring UARTs for quad core v0.1 & v0.3 board.");
			/* quad 0.3 pad setup */
			mxc_iomux_v3_setup_multiple_pads(mx6q_ar6mx_uart2_rev03_pads, \
				ARRAY_SIZE(mx6q_ar6mx_uart2_rev03_pads));
			/* mxc_iomux_v3_setup_multiple_pads(mx6q_ar6mx_uart1_rev03_pads, \
				ARRAY_SIZE(mx6q_ar6mx_uart1_rev03_pads)); */
		}
		else {
			printk(KERN_INFO "Configuring UARTs for solo core v0.1 & v0.3 board.");
			/* solo 0.3 pad setup */
			/* mxc_iomux_v3_setup_multiple_pads(mx6dl_ar6mx_uart1_rev03_pads, \
				ARRAY_SIZE(mx6dl_ar6mx_uart1_rev03_pads)); */
			mxc_iomux_v3_setup_multiple_pads(mx6dl_ar6mx_uart1_rev03_pads, \
				ARRAY_SIZE(mx6dl_ar6mx_uart1_rev03_pads));
		}

		/* Note the difference in using the rev03 struct */
		imx6q_add_imx_uart(0, &ar6mx_uart1_alt_data);
		imx6q_add_imx_uart(1, NULL);
        imx6q_add_imx_uart(2, NULL);
		imx6q_add_imx_uart(3, NULL);
	} else {
        /* BCM 1.0 or later board */
		printk(KERN_INFO "Configuring UARTs for quad core v1.0+ board.");
        imx6q_add_imx_uart(0, NULL);
	    imx6q_add_imx_uart(1, &ar6mx_uart2_data);
        imx6q_add_imx_uart(2, NULL);
	    imx6q_add_imx_uart(3, NULL);
 	} 	
}

static inline void imx6q_ar6mx_init_ldb(void)
{
	gpio_request(AR6MX_LVDS0_PWR, "lvds0");
	gpio_direction_output(AR6MX_LVDS0_PWR, 1);
	gpio_request(AR6MX_BL0_PWR, "bl0_pwr");
	gpio_direction_output(AR6MX_BL0_PWR, 1);
	//gpio_request(AR6MX_BL0_EN, "bl0_en");        Doge 8/25/2015
	//gpio_direction_output(AR6MX_BL0_EN, 1);
	if (cpu_is_mx6q()) {
		gpio_request(AR6MX_LVDS1_PWR, "lvds1");
		gpio_direction_output(AR6MX_LVDS1_PWR, 1);
		gpio_request(AR6MX_BL1_PWR, "bl1_prw");
		gpio_direction_output(AR6MX_BL1_PWR, 1);
		gpio_request(AR6MX_BL1_EN, "bl1_en");
		gpio_direction_output(AR6MX_BL1_EN, 1);
	}
}

static int mx6q_ar6mx_fec_phy_init(struct phy_device *phydev)
{
	/* RX Data Pad Skew Register */
	phy_write(phydev, 0xd, 0x0002);
	phy_write(phydev, 0xe, 0x0005);
	phy_write(phydev, 0xd, 0xc002);
	phy_write(phydev, 0xe, 0x7777);

	/* TX Data Pad Skew Register */
	phy_write(phydev, 0xd, 0x0002);
	phy_write(phydev, 0xe, 0x0006);
	phy_write(phydev, 0xd, 0xc002);
	phy_write(phydev, 0xe, 0x7777);

	/* rx/tx data delay no changed, clock set max */
	phy_write(phydev, 0xd, 0x0002);
	phy_write(phydev, 0xe, 0x0008);
	phy_write(phydev, 0xd, 0xc002);
	phy_write(phydev, 0xe, 0x7fff);

	return 0;
}

static int mx6q_ar6mx_fec_power_hibernate(struct phy_device *phydev)
{
	return 0;
}

static struct fec_platform_data fec_data __initdata = {
	.init			= mx6q_ar6mx_fec_phy_init,
	.power_hibernate	= mx6q_ar6mx_fec_power_hibernate,
	.phy			= PHY_INTERFACE_MODE_RGMII,
};

static void mx6q_mipi_powerdown(int powerdown)
{
	if (powerdown)
		gpio_set_value(AR6MX_MIPICSI_PWN, 1);
	else
		gpio_set_value(AR6MX_MIPICSI_PWN, 0);

	msleep(2);
}

static void mx6q_mipi_sensor_io_init(void)
{
	/* Camera reset */
	gpio_request(AR6MX_MIPICSI_RST, "cam-reset");
	gpio_direction_output(AR6MX_MIPICSI_RST, 1);

	/* Camera power down */
	gpio_request(AR6MX_MIPICSI_PWN, "cam-pwdn");
	gpio_direction_output(AR6MX_MIPICSI_PWN, 1);
	msleep(5);
	gpio_set_value(AR6MX_MIPICSI_PWN, 0);
	msleep(5);
	gpio_set_value(AR6MX_MIPICSI_RST, 0);
	msleep(1);
	gpio_set_value(AR6MX_MIPICSI_RST, 1);
	msleep(5);
	gpio_set_value(AR6MX_MIPICSI_PWN, 1);

}

static struct fsl_mxc_camera_platform_data mipi_csi2_data = {
	.mclk = 24000000,
	.mclk_source = 0,
	.csi = 1,
	.io_init = mx6q_mipi_sensor_io_init,
	.pwdn = mx6q_mipi_powerdown,
};

static int mx6q_ar6mx_spi_cs[] = {
	AR6MX_ECSPI3_CS0,
};

static const struct spi_imx_master mx6q_ar6mx_spi_data __initconst = {
	.chipselect     = mx6q_ar6mx_spi_cs,
	.num_chipselect = ARRAY_SIZE(mx6q_ar6mx_spi_cs),
};

static struct mtd_partition m25p32_partitions[] = {
	{
		.name	= "bootloader",
		.offset	= 0,
		.size	= 0x00100000,
	}, {
		.name	= "kernel",
		.offset	= MTDPART_OFS_APPEND,
		.size	= MTDPART_SIZ_FULL,
	},
};

static struct flash_platform_data m25p32_spi_flash_data = {
	.name		= "m25p32",
	.parts		= m25p32_partitions,
	.nr_parts	= ARRAY_SIZE(m25p32_partitions),
	.type		= "m25p32",
};

static struct mtd_partition m25p80_partitions[] = {
	{
	 .name = "bootloader",
	 .offset = 0,
	 .size = 0x00030000,
	},
	{
	 .name = "kernel",
	 .offset = MTDPART_OFS_APPEND,
	 .size = 0x003c0000,
	},
	{
	 .name = "env",
	 .offset = MTDPART_OFS_APPEND,
	 .size = MTDPART_SIZ_FULL,
	},
};

static struct flash_platform_data m25p80_spi_flash_data = {
	.name = "mx25l3205d",
	.parts = m25p80_partitions,
	.nr_parts = ARRAY_SIZE(m25p80_partitions),
	.type = "mx25l3205d",
};

static struct spi_board_info m25p32_spi0_board_info[] __initdata = {
	{
		/* The modalias must be the same as spi device driver name */
		.modalias	= "m25p80",
		.max_speed_hz = 20000000, /* max spi clock (SCK) speed in HZ */
		.bus_num = 2,
		.chip_select	= 0,
		.platform_data	= &m25p80_spi_flash_data,
	},
};
static void spi_device_init(void)
{
	spi_register_board_info(m25p32_spi0_board_info,
				ARRAY_SIZE(m25p32_spi0_board_info));
}
static struct mtd_partition mxc_nor_partitions[] = {
	{
		.name	= "Bootloader",
		.offset	= 0,
		.size	=  0x00080000,
	}, {
		.name	= "nor.Kernel",
		.offset	= MTDPART_OFS_APPEND,
		.size	= MTDPART_SIZ_FULL,
	},
};
static struct resource nor_flash_resource = {
	.start		= CS0_BASE_ADDR,
	.end		= CS0_BASE_ADDR  +  0x02000000 - 1,
	.flags		= IORESOURCE_MEM,
};

static struct physmap_flash_data nor_flash_data = {
	.probe_type	= "cfi_probe",
	.width		= 2,
	.parts		= mxc_nor_partitions,
	.nr_parts	= ARRAY_SIZE(mxc_nor_partitions),
};

static struct platform_device physmap_flash_device = {
	.name	= "physmap-flash",
	.id	= 0,
	.dev	= {
		.platform_data = &nor_flash_data,
	},
	.resource	= &nor_flash_resource,
	.num_resources	= 1,
};

static struct mxt_platform_data mxt_data = {
        .irqflags = IRQF_TRIGGER_FALLING,
};

static struct imxi2c_platform_data mx6q_ar6mx_i2c1_data = {
	.bitrate	= 100000,
};

static struct i2c_board_info mxc_i2c0_board_info[] __initdata = {
        {    
                I2C_BOARD_INFO("wm8960", 0x1a),
        },   

        {    
                I2C_BOARD_INFO("ov5640_mipi", 0x3c),
		.platform_data = (void *)&mipi_csi2_data,
        },   
};


static struct i2c_board_info mxc_i2c1_board_info[] __initdata = {
	{
		I2C_BOARD_INFO("mxc_hdmi_i2c", 0x50),
	},

        {    
                I2C_BOARD_INFO("atmel_mxt_ts", ATMEL_MXT_I2C_ADDR),
                .irq  = gpio_to_irq(AR6MX_CAP_TCH_INT0),
                .platform_data = &mxt_data,
        },   
};

static struct i2c_board_info mxc_i2c2_board_info[] __initdata = {
	{
		I2C_BOARD_INFO("s35390a", 0x30),
		.irq  = gpio_to_irq(AR6MX_RTC_INT),
	},
};

static struct imx_ssi_platform_data mx6_ar6mx_ssi1_pdata = {
	.flags = IMX_SSI_DMA | IMX_SSI_SYN,
};

static void imx6q_ar6mx_usbotg_vbus(bool on)
{
	if (on)
		gpio_set_value(AR6MX_OTG_PWR_EN, 1);
	else
		gpio_set_value(AR6MX_OTG_PWR_EN, 0);
}

static void __init imx6q_ar6mx_init_usb(void)
{
	int ret = 0;
	imx_otg_base = MX6_IO_ADDRESS(MX6Q_USB_OTG_BASE_ADDR);

	ret = gpio_request(AR6MX_OTG_PWR_EN, "usb-pwr");

	if (ret) {
		pr_err("failed to get GPIO AR6MX_OTG_PWR_EN: %d\n",
			ret);
		return;
	}

	gpio_direction_output(AR6MX_OTG_PWR_EN, 0);
	mxc_iomux_set_gpr_register(1, 13, 1, 1);
	mx6_set_otghost_vbus_func(imx6q_ar6mx_usbotg_vbus);

	gpio_request(AR6MX_USB_V1_PWR, "usb_v1");
	gpio_direction_output(AR6MX_USB_V1_PWR, 1);
	gpio_request(AR6MX_USB_V2_PWR, "usb_v2");
	gpio_direction_output(AR6MX_USB_V2_PWR, 1);
}

static struct viv_gpu_platform_data imx6q_gpu_pdata __initdata = {
	.reserved_mem_size = SZ_128M + SZ_64M,
};

/* HW Initialization, if return 0, initialization is successful. */
static int mx6q_ar6mx_sata_init(struct device *dev, void __iomem *addr)
{
	u32 tmpdata;
	int ret = 0;
	struct clk *clk;

	sata_clk = clk_get(dev, "imx_sata_clk");
	if (IS_ERR(sata_clk)) {
		dev_err(dev, "no sata clock.\n");
		return PTR_ERR(sata_clk);
	}
	ret = clk_enable(sata_clk);
	if (ret) {
		dev_err(dev, "can't enable sata clock.\n");
		goto put_sata_clk;
	}

	/* Set PHY Paremeters, two steps to configure the GPR13,
	 * one write for rest of parameters, mask of first write is 0x07FFFFFD,
	 * and the other one write for setting the mpll_clk_off_b
	 *.rx_eq_val_0(iomuxc_gpr13[26:24]),
	 *.los_lvl(iomuxc_gpr13[23:19]),
	 *.rx_dpll_mode_0(iomuxc_gpr13[18:16]),
	 *.sata_speed(iomuxc_gpr13[15]),
	 *.mpll_ss_en(iomuxc_gpr13[14]),
	 *.tx_atten_0(iomuxc_gpr13[13:11]),
	 *.tx_boost_0(iomuxc_gpr13[10:7]),
	 *.tx_lvl(iomuxc_gpr13[6:2]),
	 *.mpll_ck_off(iomuxc_gpr13[1]),
	 *.tx_edgerate_0(iomuxc_gpr13[0]),
	 */
	tmpdata = readl(IOMUXC_GPR13);
	writel(((tmpdata & ~0x07FFFFFD) | 0x0593A044), IOMUXC_GPR13);

	/* enable SATA_PHY PLL */
	tmpdata = readl(IOMUXC_GPR13);
	writel(((tmpdata & ~0x2) | 0x2), IOMUXC_GPR13);

	/* Get the AHB clock rate, and configure the TIMER1MS reg later */
	clk = clk_get(NULL, "ahb");
	if (IS_ERR(clk)) {
		dev_err(dev, "no ahb clock.\n");
		ret = PTR_ERR(clk);
		goto release_sata_clk;
	}
	tmpdata = clk_get_rate(clk) / 1000;
	clk_put(clk);

	ret = sata_init(addr, tmpdata);
	if (ret == 0)
		return ret;

release_sata_clk:
	clk_disable(sata_clk);
put_sata_clk:
	clk_put(sata_clk);

	return ret;
}

static void mx6q_ar6mx_sata_exit(struct device *dev)
{
	clk_disable(sata_clk);
	clk_put(sata_clk);

}

static struct ahci_platform_data mx6q_ar6mx_sata_data = {
	.init = mx6q_ar6mx_sata_init,
	.exit = mx6q_ar6mx_sata_exit,
};

static struct imx_asrc_platform_data imx_asrc_data = {
	.channel_bits	= 4,
	.clk_map_ver	= 2,
};

static struct ipuv3_fb_platform_data sabr_fb_data[] = {
	{ /*fb0*/
		.disp_dev		= "ldb",
		.interface_pix_fmt	= IPU_PIX_FMT_RGB666,
		.mode_str		= "LDB-XGA",
		.default_bpp		= 16,
		.int_clk		= false,
	}, {
		.disp_dev		= "ldb",
		.interface_pix_fmt	= IPU_PIX_FMT_RGB666,
		.mode_str		= "LDB-XGA",
		.default_bpp		= 16,
		.int_clk		= false,
	}, {
		.disp_dev               = "lcd",
		.interface_pix_fmt      = IPU_PIX_FMT_RGB565,
		.mode_str               = "CLAA-WVGA",
		.default_bpp            = 16,
		.int_clk                = false,
	},
};

static void hdmi_init(int ipu_id, int disp_id)
{
	int hdmi_mux_setting;

	if ((ipu_id > 1) || (ipu_id < 0)) {
		printk(KERN_ERR"Invalid IPU select for HDMI: %d. Set to 0\n",
			ipu_id);
		ipu_id = 0;
	}

	if ((disp_id > 1) || (disp_id < 0)) {
		printk(KERN_ERR"Invalid DI select for HDMI: %d. Set to 0\n",
			disp_id);
		disp_id = 0;
	}

	/* Configure the connection between IPU1/2 and HDMI */
	hdmi_mux_setting = 2*ipu_id + disp_id;

	/* GPR3, bits 2-3 = HDMI_MUX_CTL */
	mxc_iomux_set_gpr_register(3, 2, 2, hdmi_mux_setting);

	/* Set HDMI event as SDMA event2 while Chip version later than TO1.2 */
	if (hdmi_SDMA_check())
		mxc_iomux_set_gpr_register(0, 0, 1, 1);
}

/* On mx6x ar6mx board i2c2 iomux with hdmi ddc,
 * the pins default work at i2c2 function,
 when hdcp enable, the pins should work at ddc function */

static void hdmi_enable_ddc_pin(void)
{
	if (cpu_is_mx6dl())
		mxc_iomux_v3_setup_multiple_pads(mx6dl_ar6mx_hdmi_ddc_pads,
			ARRAY_SIZE(mx6dl_ar6mx_hdmi_ddc_pads));
	else
		mxc_iomux_v3_setup_multiple_pads(mx6q_ar6mx_hdmi_ddc_pads,
			ARRAY_SIZE(mx6q_ar6mx_hdmi_ddc_pads));
}

static void hdmi_disable_ddc_pin(void)
{
	if (cpu_is_mx6dl())
		mxc_iomux_v3_setup_multiple_pads(mx6dl_ar6mx_i2c2_pads,
			ARRAY_SIZE(mx6dl_ar6mx_i2c2_pads));
	else
		mxc_iomux_v3_setup_multiple_pads(mx6q_ar6mx_i2c2_pads,
			ARRAY_SIZE(mx6q_ar6mx_i2c2_pads));
}

static struct fsl_mxc_hdmi_platform_data hdmi_data = {
	.init = hdmi_init,
	.enable_pins = hdmi_enable_ddc_pin,
	.disable_pins = hdmi_disable_ddc_pin,
};

static struct fsl_mxc_hdmi_core_platform_data hdmi_core_data = {
	.ipu_id		= 0,
	.disp_id	= 0,
};

static struct fsl_mxc_lcd_platform_data lcdif_data = {
	.ipu_id		= 0,
	.disp_id	= 0,
	.default_ifmt	= IPU_PIX_FMT_RGB565,
};

static struct fsl_mxc_ldb_platform_data ldb_data = {
	.ipu_id		= 1,
	.disp_id	= 0,
	.ext_ref	= 1,
	.mode		= LDB_SEP0,
	.sec_ipu_id	= 1,
	.sec_disp_id	= 1,
};

static struct imx_ipuv3_platform_data ipu_data[] = {
	{
		.rev		= 4,
		.csi_clk[0]	= "clko2_clk",
	}, {
		.rev		= 4,
		.csi_clk[0]	= "clko2_clk",
	},
};

static struct ion_platform_data imx_ion_data = {
		.nr = 1,
		.heaps = {
				{
				.type = ION_HEAP_TYPE_CARVEOUT,
				.name = "vpu_ion",
				.size = SZ_64M,
				},
		},
};

static struct gpio mx6q_ar6mx_ver_gpios[] = {
	{ AR6MX_VER_B0, GPIOF_DIR_IN, "ver-b0" },
	{ AR6MX_VER_B1, GPIOF_DIR_IN, "ver-b1" },
	{ AR6MX_VER_B2, GPIOF_DIR_IN, "ver-b2" },
	{ AR6MX_VER_B3, GPIOF_DIR_IN, "ver-b3" },
};

static void ar6mx_suspend_enter(void)
{
	/* suspend preparation */
	/* Moved to kernel/power/earlysuspend.c  -JTS
	printk(KERN_DEBUG "sabreauto_suspend_enter(): set pwr (ctrl, status) low\n");
	gpio_set_value(AR6MX_ANDROID_PWRSTATE, 0);
	mdelay(1);
	*/
}

static void ar6mx_suspend_exit(void)
{
	/* resmue resore */
	/* Moved to kernel/power/earlysuspend.c  -JTS
	printk(KERN_DEBUG "sabreauto_suspend_exit(): set pwr (ctrl, status) high \n");
	gpio_set_value(AR6MX_ANDROID_PWRSTATE, 1);
	mdelay(1);
	*/

	/* Try to recover PCie bus to prevent please wait message
	   pdi - mrobbeloth */
	printk(KERN_DEBUG "sabreauto_suspend_exit(): resetting mPCIe bus\n");
	pcie_3v3_reset();
}
static const struct pm_platform_data mx6q_ar6mx_pm_data __initconst = {
	.name		= "imx_pm",
	.suspend_enter	= ar6mx_suspend_enter,
	.suspend_exit	= ar6mx_suspend_exit,
};

static struct regulator_consumer_supply ar6mx_vmmc_consumers[] = {
	REGULATOR_SUPPLY("vmmc", "sdhci-esdhc-imx.2"),
	REGULATOR_SUPPLY("vmmc", "sdhci-esdhc-imx.3"),
};

static struct regulator_init_data ar6mx_vmmc_init = {
	.num_consumer_supplies = ARRAY_SIZE(ar6mx_vmmc_consumers),
	.consumer_supplies = ar6mx_vmmc_consumers,
};

static struct fixed_voltage_config ar6mx_vmmc_reg_config = {
	.supply_name	= "vmmc",
	.microvolts	= 3300000,
	.gpio		= -1,
	.init_data	= &ar6mx_vmmc_init,
};

static struct platform_device ar6mx_vmmc_reg_devices = {
	.name		= "reg-fixed-voltage",
	.id		= 0,
	.dev		= {
				.platform_data = &ar6mx_vmmc_reg_config,
	},
};

static struct platform_device mx6_sabresd_audio_wm8960_device = {
	.name = "imx-wm8960",
};

static struct mxc_audio_platform_data wm8960_data;

static int wm8960_clk_enable(int enable)
{
#if 0
	if (enable)
		clk_enable(clko);
	else
		clk_disable(clko);
#endif
	return 0;
}

static int mxc_wm8960_init(void)
{
	struct clk *clko;
	struct clk *new_parent;
	int rate;

	clko = clk_get(NULL, "clko_clk");
	if (IS_ERR(clko)) {
		pr_err("can't get CLKO clock.\n");
		return PTR_ERR(clko);
	}
	new_parent = clk_get(NULL, "clko2_clk");
	if (!IS_ERR(new_parent)) {
		clk_set_parent(clko, new_parent);
		clk_put(new_parent);
	}
	rate = clk_round_rate(clko, 24000000);

	clk_set_rate(clko, rate);
	wm8960_data.sysclk = rate;
	clk_enable(clko);
}

static struct mxc_audio_platform_data wm8960_data = {
	.ssi_num = 1,
	.src_port = 2,
	.ext_port = 3,
	.hp_gpio = -1,
	.hp_active_low = 1,
	.mic_gpio = -1,
	.mic_active_low = 1,
	.init = mxc_wm8960_init,
	.clock_enable = wm8960_clk_enable,
};

static int __init imx6q_init_audio(void)
{
	/* We leverage WM8962 platform driver on WM8960. */
	mxc_register_device(&mx6_sabresd_audio_wm8960_device, &wm8960_data);
	imx6q_add_imx_ssi(1, &mx6_ar6mx_ssi1_pdata);

	return 0;
}

static struct mxc_mlb_platform_data mx6_ar6mx_mlb150_data = {
	.reg_nvcc		= NULL,
	.mlb_clk		= "mlb150_clk",
	.mlb_pll_clk		= "pll6",
};

static struct mxc_dvfs_platform_data ar6mx_dvfscore_data = {
	.reg_id			= "cpu_vddgp",
	.soc_id			= "cpu_vddsoc",
	.pu_id			= "cpu_vddvpu",
	.clk1_id		= "cpu_clk",
	.clk2_id		= "gpc_dvfs_clk",
	.gpc_cntr_offset	= MXC_GPC_CNTR_OFFSET,
	.ccm_cdcr_offset	= MXC_CCM_CDCR_OFFSET,
	.ccm_cacrr_offset	= MXC_CCM_CACRR_OFFSET,
	.ccm_cdhipr_offset	= MXC_CCM_CDHIPR_OFFSET,
	.prediv_mask		= 0x1F800,
	.prediv_offset		= 11,
	.prediv_val			= 3,
	.div3ck_mask		= 0xE0000000,
	.div3ck_offset		= 29,
	.div3ck_val			= 2,
	.emac_val		= 0x08,
	.upthr_val		= 25,
	.dnthr_val		= 9,
	.pncthr_val		= 33,
	.upcnt_val		= 10,
	.dncnt_val		= 10,
	.delay_time		= 80,
};

static void __init fixup_mxc_board(struct machine_desc *desc, struct tag *tags,
				   char **cmdline, struct meminfo *mi)
{
	char *str;
	struct tag *t;

	for_each_tag(t, tags) {
		if (t->hdr.tag == ATAG_CMDLINE) {
			/* GPU reserved memory */
			str = t->u.cmdline.cmdline;
			str = strstr(str, "gpumem=");
			if (str != NULL) {
				str += 7;
				imx6q_gpu_pdata.reserved_mem_size = memparse(str, &str);
			}
			break;
		}
	}
}

static struct mxc_spdif_platform_data mxc_spdif_data = {
	.spdif_tx	= 0,	/* disable tx */
	.spdif_rx	= 1,	/* enable rx */
	.spdif_rx_clk	= 0,	/* rx clk from spdif stream */
	.spdif_clk	= NULL,	/* spdif bus clk */
};

static struct fsl_mxc_capture_platform_data capture_data[] = {
	{
		.csi = 0,
		.ipu = 0,
		.mclk_source = 0,
		.is_mipi = 0,
	}, {
		.csi = 1,
		.ipu = 0,
		.mclk_source = 0,
		.is_mipi = 1,
	},
};

static struct mipi_csi2_platform_data mipi_csi2_pdata = {
	.ipu_id	 = 0,
	.csi_id = 1,
	.v_channel = 0,
	.lanes = 2,
	.dphy_clk = "mipi_pllref_clk",
	.pixel_clk = "emi_clk",
};

static const struct imx_pcie_platform_data mx6_ar6mx_pcie_data __initconst = {
	.pcie_pwr_en	= -EINVAL,
	.pcie_rst	= AR6MX_PCIE_RST_B,
	.pcie_wake_up	= -EINVAL,
	.pcie_dis	= -EINVAL,
};

static void board_rev(void)
{
	int ret;

	ret = gpio_request_array(mx6q_ar6mx_ver_gpios,
		ARRAY_SIZE(mx6q_ar6mx_ver_gpios));
	if (ret)
		pr_err("failed to request ver gpios: %d\n", ret);
	else
		board_id = gpio_get_value(AR6MX_VER_B3) << 3 | \
		gpio_get_value(AR6MX_VER_B2) << 2 | \
		gpio_get_value(AR6MX_VER_B1) << 1 |	gpio_get_value(AR6MX_VER_B0);
}

static __init void ar6mx_init_external_gpios(void) {
        gpio_request(AR6MX_ANDROID_PWRSTATE, "android_power_state");
        gpio_direction_output(AR6MX_ANDROID_PWRSTATE, 1);
        gpio_export(AR6MX_ANDROID_PWRSTATE, true);
        // Doge 8/25/2015 export BL0_EN to user space
       	gpio_request(AR6MX_BL0_EN, "bl0_en");       
		gpio_direction_output(AR6MX_BL0_EN, 1);
        gpio_export(AR6MX_BL0_EN, true);
		// Export the green LED to user space and turn it off -JTS
		gpio_request(AR6MX_SATA_LED, "sata_led");       
		gpio_direction_output(AR6MX_SATA_LED, 0);
        gpio_export(AR6MX_SATA_LED, true);
		// Export the amber LED to user space and turn if off -JTS
		gpio_request(AR6MX_STATUS_LED, "status_led");       
		gpio_direction_output(AR6MX_STATUS_LED, 0);
        gpio_export(AR6MX_STATUS_LED, true);
	// Export the DO1 to user space and pull it low -JTS
	gpio_request(AR6MX_INTERNAL_SPK_ENABLE, "int_speaker_enable");       
	gpio_direction_output(AR6MX_INTERNAL_SPK_ENABLE, 0);
        gpio_export(AR6MX_INTERNAL_SPK_ENABLE, true);
        // export AIO, SPK_DET to user space to make "Audio Switch" work in repository revision: 1202
	gpio_request(AR6MX_TV_OR_AIO, "all_in_one_GPIO37");       
	gpio_direction_input(AR6MX_TV_OR_AIO);
        gpio_export(AR6MX_TV_OR_AIO, true);      // doge    GPIO37
	gpio_request(AR6MX_SPK_DET, "spkeaker_detect_gpio200");       
	gpio_direction_input(AR6MX_SPK_DET);
        gpio_export(AR6MX_SPK_DET, true);      // doge 7,8  (7-1)*32+8 = 200
        gpio_direction_output(AR6MX_HDMIHPD_MAPPING, 0); //DI4, GPIO36
}

/* Backlight PWM for LVDS0 */
static struct platform_pwm_backlight_data ar6mxc_pwm_backlight_data1 = {
	.pwm_id			= 0,
	.max_brightness		= 255,
	.dft_brightness		= 255,
	.pwm_period_ns		= 500000,
};

/* Backlight PWM for LVDS1 */
static struct platform_pwm_backlight_data ar6mxc_pwm_backlight_data2 = {
	.pwm_id			= 1,
	.max_brightness		= 255,
	.dft_brightness		= 255,
	.pwm_period_ns		= 500000,
};

/* Backlight PWM for CSI */
static struct platform_pwm_backlight_data ar6mxc_pwm_backlight_data4 = {
	.pwm_id			= 3,
	.max_brightness		= 255,
	.dft_brightness		= 255,
	.pwm_period_ns		= 500000,
};

/*!
 * Board specific initialization.
 */
static void __init mx6_board_init(void)
{
	int i;
	int ret;
	iomux_v3_cfg_t *common_pads = NULL;
	iomux_v3_cfg_t *can0_pads = NULL;
	iomux_v3_cfg_t *can1_pads = NULL;
	iomux_v3_cfg_t *mipi_sensor_pads = NULL;
	iomux_v3_cfg_t *i2c3_pads = NULL;
	iomux_v3_cfg_t *tuner_pads = NULL;
	iomux_v3_cfg_t *spinor_pads = NULL;
	iomux_v3_cfg_t *weimnor_pads = NULL;
	iomux_v3_cfg_t *extra_pads = NULL;

	int common_pads_cnt;
	int can0_pads_cnt;
	int can1_pads_cnt;
	int mipi_sensor_pads_cnt;
	int i2c3_pads_cnt;
	int tuner_pads_cnt;
	int spinor_pads_cnt;
	int weimnor_pads_cnt;
	int extra_pads_cnt;

	if (cpu_is_mx6q()) {
		common_pads = mx6q_ar6mx_pads;
		common_pads_cnt = ARRAY_SIZE(mx6q_ar6mx_pads);
	} else if (cpu_is_mx6dl()) {
		common_pads = mx6dl_ar6mx_pads;
		common_pads_cnt = ARRAY_SIZE(mx6dl_ar6mx_pads);
	}

	BUG_ON(!common_pads);
	mxc_iomux_v3_setup_multiple_pads(common_pads, common_pads_cnt);

	gp_reg_id = ar6mx_dvfscore_data.reg_id;
	soc_reg_id = ar6mx_dvfscore_data.soc_id;
	pu_reg_id = ar6mx_dvfscore_data.pu_id;
	board_rev();
	mx6q_ar6mx_init_uart();
	imx6q_add_mxc_hdmi_core(&hdmi_core_data);

	imx6q_add_ipuv3(0, &ipu_data[0]);
	if (cpu_is_mx6q()) {
		imx6q_add_ipuv3(1, &ipu_data[1]);
		for (i = 0; i < ARRAY_SIZE(sabr_fb_data); i++)
			imx6q_add_ipuv3fb(i, &sabr_fb_data[i]);
	} else if (cpu_is_mx6dl())
		for (i = 0; i < (ARRAY_SIZE(sabr_fb_data) + 1) / 2; i++)
			imx6q_add_ipuv3fb(i, &sabr_fb_data[i]);

	imx6q_add_vdoa();
	imx6q_add_lcdif(&lcdif_data);
	imx6q_add_ldb(&ldb_data);
	imx6q_add_v4l2_output(0);
	imx6q_add_v4l2_capture(0, &capture_data[0]);
	imx6q_add_v4l2_capture(1, &capture_data[1]);
	imx6q_add_mipi_csi2(&mipi_csi2_pdata);

	if (!board_is_mx6_reva())
		imx6q_add_imx_snvs_rtc();

	imx6q_add_imx_caam();

	imx6q_add_imx_i2c(0, &mx6q_ar6mx_i2c1_data);
	imx6q_add_imx_i2c(1, &mx6q_ar6mx_i2c1_data);
	imx6q_add_imx_i2c(2, &mx6q_ar6mx_i2c1_data);
	i2c_register_board_info(0, mxc_i2c0_board_info,
			ARRAY_SIZE(mxc_i2c0_board_info));
	i2c_register_board_info(1, mxc_i2c1_board_info,
			ARRAY_SIZE(mxc_i2c1_board_info));
	i2c_register_board_info(2, mxc_i2c2_board_info,
			ARRAY_SIZE(mxc_i2c2_board_info));

	/* SPI */
	imx6q_add_ecspi(2, &mx6q_ar6mx_spi_data);
	if (spinor_en)
		spi_device_init();

	imx6q_add_mxc_hdmi(&hdmi_data);

	imx6q_add_anatop_thermal_imx(1, &mx6q_ar6mx_anatop_thermal_data);

	imx6_init_fec(fec_data);

	imx6q_add_pm_imx(0, &mx6q_ar6mx_pm_data);

	if (emmc_en)
		imx6q_add_sdhci_usdhc_imx(3, &mx6q_ar6mx_sd4_data);
	imx6q_add_sdhci_usdhc_imx(2, &mx6q_ar6mx_sd3_data);

	imx_add_viv_gpu(&imx6_gpu_data, &imx6q_gpu_pdata);
	imx6q_ar6mx_init_usb();
	if (cpu_is_mx6q())
		imx6q_add_ahci(0, &mx6q_ar6mx_sata_data);
	imx6q_add_vpu();
	imx6q_init_audio();
	platform_device_register(&ar6mx_vmmc_reg_devices);

	imx_asrc_data.asrc_core_clk = clk_get(NULL, "asrc_clk");
	imx_asrc_data.asrc_audio_clk = clk_get(NULL, "asrc_serial_clk");
	imx6q_add_asrc(&imx_asrc_data);

	imx6q_ar6mx_init_ldb();
	imx6q_add_otp();
	imx6q_add_viim();
	imx6q_add_imx2_wdt(0, NULL);
	imx6q_add_dma();

	imx6q_add_dvfs_core(&ar6mx_dvfscore_data);

	imx6q_add_mxc_pwm(0);
	imx6q_add_mxc_pwm(1);
	imx6q_add_mxc_pwm(3);
	imx6q_add_mxc_pwm_backlight(0, &ar6mxc_pwm_backlight_data1);
	imx6q_add_mxc_pwm_backlight(1, &ar6mxc_pwm_backlight_data2);
	imx6q_add_mxc_pwm_backlight(3, &ar6mxc_pwm_backlight_data4);

	imx6q_add_hdmi_soc();
	imx6q_add_hdmi_soc_dai();

	pcie_3v3_reset();

	imx6q_add_busfreq();

	/* Connect the power mgmt state gpio mrobbeloth, PDi*/
	ar6mx_init_external_gpios();

	/* Add Android Recovery GPIOs */	
	imx6q_add_android_device_buttons();

	/* Add PCIe RC interface support */
	imx6q_add_pcie(&mx6_ar6mx_pcie_data);

	imx6q_add_perfmon(0);
	imx6q_add_perfmon(1);
	imx6q_add_perfmon(2);

	/* PDi mrobbeloth add call to register resources and data for ion 
           driver */
        if (imx_ion_data.heaps[0].size)
                platform_device_register_resndata(NULL, "ion-mxc", 0, NULL, 0, \
                &imx_ion_data, sizeof(imx_ion_data) + sizeof(struct ion_platform_heap));
}

extern void __iomem *twd_base;
static void __init mx6_timer_init(void)
{
	struct clk *uart_clk;
#ifdef CONFIG_LOCAL_TIMERS
	twd_base = ioremap(LOCAL_TWD_ADDR, SZ_256);
	BUG_ON(!twd_base);
#endif
	mx6_clocks_init(32768, 24000000, 0, 0);

	uart_clk = clk_get_sys("imx-uart.0", NULL);
	early_console_setup(UART1_BASE_ADDR, uart_clk);
}

static struct sys_timer mxc_timer = {
	.init = mx6_timer_init,
};

static void __init mx6q_reserve(void)
{
	phys_addr_t phys;
#if defined(CONFIG_MXC_GPU_VIV) || defined(CONFIG_MXC_GPU_VIV_MODULE)
	if (imx6q_gpu_pdata.reserved_mem_size) {
		phys = memblock_alloc_base(imx6q_gpu_pdata.reserved_mem_size,
			SZ_4K, SZ_2G);
		memblock_remove(phys, imx6q_gpu_pdata.reserved_mem_size);
		imx6q_gpu_pdata.reserved_mem_base = phys;
	}
#endif

#if defined(CONFIG_ION)
	if (imx_ion_data.heaps[0].size) {
		phys = memblock_alloc(imx_ion_data.heaps[0].size, SZ_4K);
		memblock_free(phys, imx_ion_data.heaps[0].size);
		memblock_remove(phys, imx_ion_data.heaps[0].size);
		imx_ion_data.heaps[0].base = phys;
	}
#endif
}

MACHINE_START(AR6MXQ, "Freescale i.MX 6Quad/DualLite/Solo BCM AR6MX Board")
	.boot_params	= MX6_PHYS_OFFSET + 0x100,
	.fixup		= fixup_mxc_board,
	.map_io		= mx6_map_io,
	.init_irq	= mx6_init_irq,
	.init_machine	= mx6_board_init,
	.timer		= &mxc_timer,
	.reserve	= mx6q_reserve,
MACHINE_END
