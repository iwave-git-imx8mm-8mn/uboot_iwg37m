// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (c) 2020 iWave Systems Technologies Pvt. Ltd.
 */

#include <common.h>
#include <miiphy.h>
#include <netdev.h>
#include <asm/mach-imx/iomux-v3.h>
#include <asm-generic/gpio.h>
#include <asm/arch/imx8mn_pins.h>
#include <asm/arch/clock.h>
#include <asm/arch/sys_proto.h>
#include <asm/mach-imx/gpio.h>
#include <asm/mach-imx/mxc_i2c.h>
#include <i2c.h>
#include <asm/io.h>
#include <usb.h>
#include <asm/setup.h>

DECLARE_GLOBAL_DATA_PTR;

int ret;

#define UART_PAD_CTRL	(PAD_CTL_DSE6 | PAD_CTL_FSEL1)
#define WDOG_PAD_CTRL	(PAD_CTL_DSE6 | PAD_CTL_ODE | PAD_CTL_PUE | PAD_CTL_PE)

static iomux_v3_cfg_t const uart_pads[] = {
	IMX8MN_PAD_UART4_RXD__UART4_DCE_RX | MUX_PAD_CTRL(UART_PAD_CTRL),
	IMX8MN_PAD_UART4_TXD__UART4_DCE_TX | MUX_PAD_CTRL(UART_PAD_CTRL),
};

static iomux_v3_cfg_t const wdog_pads[] = {
	IMX8MN_PAD_GPIO1_IO02__WDOG1_WDOG_B  | MUX_PAD_CTRL(WDOG_PAD_CTRL),
};

#ifdef CONFIG_NAND_MXS
#ifdef CONFIG_SPL_BUILD
#define NAND_PAD_CTRL	(PAD_CTL_DSE6 | PAD_CTL_FSEL2 | PAD_CTL_HYS)
#define NAND_PAD_READY0_CTRL (PAD_CTL_DSE6 | PAD_CTL_FSEL2 | PAD_CTL_PUE)
static iomux_v3_cfg_t const gpmi_pads[] = {
	IMX8MN_PAD_NAND_ALE__RAWNAND_ALE | MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MN_PAD_NAND_CE0_B__RAWNAND_CE0_B | MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MN_PAD_NAND_CLE__RAWNAND_CLE | MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MN_PAD_NAND_DATA00__RAWNAND_DATA00 | MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MN_PAD_NAND_DATA01__RAWNAND_DATA01 | MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MN_PAD_NAND_DATA02__RAWNAND_DATA02 | MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MN_PAD_NAND_DATA03__RAWNAND_DATA03 | MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MN_PAD_NAND_DATA04__RAWNAND_DATA04 | MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MN_PAD_NAND_DATA05__RAWNAND_DATA05	| MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MN_PAD_NAND_DATA06__RAWNAND_DATA06	| MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MN_PAD_NAND_DATA07__RAWNAND_DATA07	| MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MN_PAD_NAND_RE_B__RAWNAND_RE_B | MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MN_PAD_NAND_READY_B__RAWNAND_READY_B | MUX_PAD_CTRL(NAND_PAD_READY0_CTRL),
	IMX8MN_PAD_NAND_WE_B__RAWNAND_WE_B | MUX_PAD_CTRL(NAND_PAD_CTRL),
	IMX8MN_PAD_NAND_WP_B__RAWNAND_WP_B | MUX_PAD_CTRL(NAND_PAD_CTRL),
};
#endif

static void setup_gpmi_nand(void)
{
#ifdef CONFIG_SPL_BUILD
	imx_iomux_v3_setup_multiple_pads(gpmi_pads, ARRAY_SIZE(gpmi_pads));
#endif

	init_nand_clk();
}
#endif

int board_early_init_f(void)
{
	struct wdog_regs *wdog = (struct wdog_regs *)WDOG1_BASE_ADDR;

	imx_iomux_v3_setup_multiple_pads(wdog_pads, ARRAY_SIZE(wdog_pads));

	set_wdog_reset(wdog);

	imx_iomux_v3_setup_multiple_pads(uart_pads, ARRAY_SIZE(uart_pads));

	init_uart_clk(3);

#ifdef CONFIG_NAND_MXS
	setup_gpmi_nand(); /* SPL will call the board_early_init_f */
#endif

	return 0;
}

#if IS_ENABLED(CONFIG_FEC_MXC)
#define FEC_RST_PAD IMX_GPIO_NR(4, 28)
static iomux_v3_cfg_t const fec1_rst_pads[] = {
	IMX8MN_PAD_SAI3_RXFS__GPIO4_IO28 | MUX_PAD_CTRL(NO_PAD_CTRL),
};

static void setup_iomux_fec(void)
{
	imx_iomux_v3_setup_multiple_pads(fec1_rst_pads,
					 ARRAY_SIZE(fec1_rst_pads));

	gpio_request(FEC_RST_PAD, "fec1_rst");
	gpio_direction_output(FEC_RST_PAD, 0);
	udelay(500);
	gpio_direction_output(FEC_RST_PAD, 1);
}

static int setup_fec(void)
{
	struct iomuxc_gpr_base_regs *gpr =
		(struct iomuxc_gpr_base_regs *)IOMUXC_GPR_BASE_ADDR;

	setup_iomux_fec();

	/* Use 125M anatop REF_CLK1 for ENET1, not from external */
	clrsetbits_le32(&gpr->gpr[1], 0x2000, 0);

	return 0;
}

int board_phy_config(struct phy_device *phydev)
{
	/* enable rgmii rxc skew and phy mode select to RGMII copper */
	phy_write(phydev, MDIO_DEVAD_NONE, 0x1d, 0x1f);
	phy_write(phydev, MDIO_DEVAD_NONE, 0x1e, 0x8);

	phy_write(phydev, MDIO_DEVAD_NONE, 0x1d, 0x00);
	phy_write(phydev, MDIO_DEVAD_NONE, 0x1e, 0x82ee);
	phy_write(phydev, MDIO_DEVAD_NONE, 0x1d, 0x05);
	phy_write(phydev, MDIO_DEVAD_NONE, 0x1e, 0x100);

	if (phydev->drv->config)
		phydev->drv->config(phydev);
	return 0;
}
#endif

int board_usb_init(int index, enum usb_init_type init)
{
	int ret = 0;

	debug("board_usb_init %d, type %d\n", index, init);

	imx8m_usb_power(index, true);

	return ret;
}

int board_usb_cleanup(int index, enum usb_init_type init)
{
	int ret = 0;

	debug("board_usb_cleanup %d, type %d\n", index, init);

	imx8m_usb_power(index, false);
	return ret;
}

int board_ehci_usb_phy_mode(struct udevice *dev)
{
	return USB_INIT_DEVICE;
}

#define FSL_SIP_GPC			0xC2000000
#define FSL_SIP_CONFIG_GPC_PM_DOMAIN	0x3
#define DISPMIX				9
#define MIPI				10

int board_init(void)
{

	if (IS_ENABLED(CONFIG_FEC_MXC))
		setup_fec();

	call_imx_sip(FSL_SIP_GPC, FSL_SIP_CONFIG_GPC_PM_DOMAIN, DISPMIX, true, 0);
	call_imx_sip(FSL_SIP_GPC, FSL_SIP_CONFIG_GPC_PM_DOMAIN, MIPI, true, 0);

	return 0;
}

int board_mmc_get_env_dev(int devno)
{
	if(devno == 0)
	return devno + 2;

	else if (devno == 2)
	return devno - 2;

	else
	return devno;
}

int mmc_map_to_kernel_blk(int devno)
{
	return devno;
}

#define GPIO_PAD_CFG_CTRL ( PAD_CTL_DSE0 | PAD_CTL_ODE | PAD_CTL_PUE )

#define BCONFIG_0 IMX_GPIO_NR(1, 9)
#define BCONFIG_1 IMX_GPIO_NR(5, 1)
#define BCONFIG_2 IMX_GPIO_NR(4, 25)
#define BCONFIG_3 IMX_GPIO_NR(4, 26)
#define BCONFIG_4 IMX_GPIO_NR(4, 27)
#define BCONFIG_5 IMX_GPIO_NR(2, 19)
#define BCONFIG_6 IMX_GPIO_NR(2, 20)

int const board_config_pads[] = {
       BCONFIG_0,
       BCONFIG_1,
       BCONFIG_2,
       BCONFIG_3,
       BCONFIG_4,
       BCONFIG_5,
       BCONFIG_6,
};

static iomux_v3_cfg_t const board_cfg[] = {
      IMX8MN_PAD_GPIO1_IO09__GPIO1_IO9 | MUX_PAD_CTRL(GPIO_PAD_CFG_CTRL),
      IMX8MN_PAD_SAI3_TXD__GPIO5_IO1 | MUX_PAD_CTRL(GPIO_PAD_CFG_CTRL),
      IMX8MN_PAD_SAI2_TXC__GPIO4_IO25 | MUX_PAD_CTRL(GPIO_PAD_CFG_CTRL),
      IMX8MN_PAD_SAI2_TXD0__GPIO4_IO26 | MUX_PAD_CTRL(GPIO_PAD_CFG_CTRL),
      IMX8MN_PAD_SAI2_MCLK__GPIO4_IO27 | MUX_PAD_CTRL(GPIO_PAD_CFG_CTRL),
      IMX8MN_PAD_SD2_RESET_B__GPIO2_IO19 | MUX_PAD_CTRL(GPIO_PAD_CFG_CTRL),
      IMX8MN_PAD_SD2_WP__GPIO2_IO20 | MUX_PAD_CTRL(GPIO_PAD_CFG_CTRL),
};

/* IWG37M: Print Board Information */
static void printboard_info(void)
{
     int i, bom_rev = 0, pcb_rev = 0, revision = 0;
      
     struct tag_serialnr serialnr;
      
     /*IWG27M : Adding CPU Unique ID read support*/
     get_board_serial(&serialnr);     
     imx_iomux_v3_setup_multiple_pads(board_cfg, ARRAY_SIZE(board_cfg));

     for (i=0;i<ARRAY_SIZE(board_config_pads);i++) 
     {
	   gpio_request(board_config_pads[i], "SOM-Revision-GPIO");
	   gpio_direction_input(board_config_pads[i]);
	   revision |= gpio_get_value(board_config_pads[i]) << i;
     }

     pcb_rev = ((revision) & 0x03) +1;
     bom_rev = (((revision) & 0x78)>>3);

     printf ("Board Info:\n");
     printf ("\tBSP Version               : %s\n", BSP_VERSION);
     printf ("\tSOM Version               : iW-PRGJZ-AP-01-R%x.%x\n",pcb_rev,bom_rev);
     printf ("\tCPU Unique ID             : 0x%08X%08X\n",serialnr.high,serialnr.low);
     if (ret == 1) {
     printf ("\tCarrier Board Version     : iW-PREVD-01-R2.1\n");
     printf ("\n");
        } else if (ret == 0) {
     printf ("\tCarrier Board Version     : iW-PREVD-01-R2.0\n");
     printf ("\n");
        }
        else
     printf ("\tCarrier Board Version     : iW-PREVD-01-R2.x\n");
}

int carrier_rev(void)
{
        struct udevice *bus;
        struct udevice *i2c_dev = NULL;
        uint8_t valb;

        ret = uclass_get_device_by_seq(UCLASS_I2C, 1, &bus);
        if (ret) {
                printf("%s: Can't find bus\n", __func__);
                return -EINVAL;
        }

        ret = dm_i2c_probe(bus, 0x44, 0, &i2c_dev);
        if (ret) {
                printf("%s: Can't find device id=0x%x\n",
                       ret);
                return -ENODEV;
        }

        ret = dm_i2c_read(i2c_dev, 0x44, &valb, 1);
        if (ret) {
                printf("%s dm_i2c_read failed, err %d\n", __func__, ret);
                return -EIO;
        }

        ret = dm_i2c_reg_write(i2c_dev, 0x04, 0x0b);

        ret = dm_i2c_reg_write(i2c_dev, 0x17, 0xff);

        ret = dm_i2c_reg_read(i2c_dev, 0x12);
        return ret;
}

void iwg37m_fdt_update(void *fdt)
{
        int value=0;
        int reg[1];
        reg[0] = cpu_to_fdt32(ret);
        value = fdt_setprop(fdt,fdt_path_offset(fdt, "/carrier_revision"), "a", reg, sizeof(reg));
}

#define OTG_OC IMX_GPIO_NR(1, 13)
static iomux_v3_cfg_t const otg_oc_pads[] = {
	IMX8MN_PAD_GPIO1_IO13__GPIO1_IO13 | MUX_PAD_CTRL(NO_PAD_CTRL),
};

static void usb_otg_oc_enable(void)
{
	imx_iomux_v3_setup_multiple_pads(otg_oc_pads, ARRAY_SIZE(otg_oc_pads));
	gpio_request(OTG_OC, "OTG-OC-GPIO");
	gpio_direction_input(OTG_OC);
}

int board_late_init(void)
{
	carrier_rev();
	printboard_info();
	usb_otg_oc_enable();
#ifdef CONFIG_ENV_IS_IN_MMC
	board_late_mmc_env_init();
#endif

#ifdef CONFIG_ENV_VARS_UBOOT_RUNTIME_CONFIG
	env_set("board_name", "IWG37M");
	env_set("board_rev", "iMX8MN");
#endif
	return 0;
}

#ifdef CONFIG_FSL_FASTBOOT
#ifdef CONFIG_ANDROID_RECOVERY
int is_recovery_key_pressing(void)
{
	return 0; /*TODO*/
}
#endif /*CONFIG_ANDROID_RECOVERY*/
#endif /*CONFIG_FSL_FASTBOOT*/
