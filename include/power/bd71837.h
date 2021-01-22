#ifdef CONFIG_TARGET_IMX8MN_IWG37M
/* IWG37M: PMIC: Support for bd718x7 */
#ifndef BD718XX_H_
#define BD718XX_H_

#define BD718XX_REGULATOR_DRIVER "bd718x7_regulator"

enum {
	ROHM_CHIP_TYPE_BD71837 = 0,
	ROHM_CHIP_TYPE_BD71847,
	ROHM_CHIP_TYPE_BD70528,
	ROHM_CHIP_TYPE_AMOUNT
};

enum {
	BD718XX_REV			= 0x00,
	BD718XX_SWRESET			= 0x01,
	BD718XX_I2C_DEV			= 0x02,
	BD718XX_PWRCTRL0		= 0x03,
	BD718XX_PWRCTRL1		= 0x04,
	BD718XX_BUCK1_CTRL		= 0x05,
	BD718XX_BUCK2_CTRL		= 0x06,
	BD71837_BUCK3_CTRL		= 0x07,
	BD71837_BUCK4_CTRL		= 0x08,
	BD718XX_1ST_NODVS_BUCK_CTRL	= 0x09,
	BD718XX_2ND_NODVS_BUCK_CTRL	= 0x0A,
	BD718XX_3RD_NODVS_BUCK_CTRL	= 0x0B,
	BD718XX_4TH_NODVS_BUCK_CTRL	= 0x0C,
	BD718XX_BUCK1_VOLT_RUN		= 0x0D,
	BD718XX_BUCK1_VOLT_IDLE		= 0x0E,
	BD718XX_BUCK1_VOLT_SUSP		= 0x0F,
	BD718XX_BUCK2_VOLT_RUN		= 0x10,
	BD718XX_BUCK2_VOLT_IDLE		= 0x11,
	BD71837_BUCK3_VOLT_RUN		= 0x12,
	BD71837_BUCK4_VOLT_RUN		= 0x13,
	BD718XX_1ST_NODVS_BUCK_VOLT	= 0x14,
	BD718XX_2ND_NODVS_BUCK_VOLT	= 0x15,
	BD718XX_3RD_NODVS_BUCK_VOLT	= 0x16,
	BD718XX_4TH_NODVS_BUCK_VOLT	= 0x17,
	BD718XX_LDO1_VOLT		= 0x18,
	BD718XX_LDO2_VOLT		= 0x19,
	BD718XX_LDO3_VOLT		= 0x1A,
	BD718XX_LDO4_VOLT		= 0x1B,
	BD718XX_LDO5_VOLT		= 0x1C,
	BD718XX_LDO6_VOLT		= 0x1D,
	BD71837_LDO7_VOLT		= 0x1E,
	BD718XX_TRANS_COND0		= 0x1F,
	BD718XX_TRANS_COND1		= 0x20,
	BD718XX_VRFAULTEN		= 0x21,
	BD718XX_MVRFLTMASK0		= 0x22,
	BD718XX_MVRFLTMASK1		= 0x23,
	BD718XX_MVRFLTMASK2		= 0x24,
	BD718XX_RCVCFG			= 0x25,
	BD718XX_RCVNUM			= 0x26,
	BD718XX_PWRONCONFIG0		= 0x27,
	BD718XX_PWRONCONFIG1		= 0x28,
	BD718XX_RESETSRC		= 0x29,
	BD718XX_MIRQ			= 0x2A,
	BD718XX_IRQ			= 0x2B,
	BD718XX_IN_MON			= 0x2C,
	BD718XX_POW_STATE		= 0x2D,
	BD718XX_OUT32K			= 0x2E,
	BD718XX_REGLOCK			= 0x2F,
	BD718XX_MUXSW_EN		= 0x30,
	BD718XX_REG_OTPVER		= 0xFF,
	BD718XX_MAX_REGISTER		= 0x100,
};

#define BD718XX_REGLOCK_PWRSEQ		0x1
#define BD718XX_REGLOCK_VREG		0x10

#define BD718XX_BUCK_EN			0x01
#define BD718XX_LDO_EN			0x40
#define BD718XX_BUCK_SEL		0x02
#define BD718XX_LDO_SEL			0x80

#define DVS_BUCK_RUN_MASK		0x3F
#define BD718XX_1ST_NODVS_BUCK_MASK	0x07
#define BD718XX_3RD_NODVS_BUCK_MASK	0x07
#define BD718XX_4TH_NODVS_BUCK_MASK	0x3F

#define BD71847_BUCK3_MASK		0x07
#define BD71847_BUCK3_RANGE_MASK	0xC0
#define BD71847_BUCK4_MASK		0x03
#define BD71847_BUCK4_RANGE_MASK	0x40

#define BD71837_BUCK5_RANGE_MASK	0x80
#define BD71837_BUCK6_MASK		0x03

#define BD718XX_LDO1_MASK		0x03
#define BD718XX_LDO1_RANGE_MASK		0x20
#define BD718XX_LDO2_MASK		0x20
#define BD718XX_LDO3_MASK		0x0F
#define BD718XX_LDO4_MASK		0x0F
#define BD718XX_LDO6_MASK		0x0F

#define BD71837_LDO5_MASK		0x0F
#define BD71847_LDO5_MASK		0x0F
#define BD71847_LDO5_RANGE_MASK		0x20
#define BD71837_LDO7_MASK		0x0F

int power_bd71837_init(unsigned char bus);

#else

#ifndef BD71837_H_
#define BD71837_H_

#define BD71837_REGULATOR_DRIVER "bd71837_regulator"

enum {
    BD71837_REV 		= 0x00,
    BD71837_SWRESET 		= 0x01,
    BD71837_I2C_DEV		= 0x02,
    BD71837_PWRCTRL0		= 0x03,
    BD71837_PWRCTRL1 		= 0x04,
    BD71837_BUCK1_CTRL		= 0x05,
    BD71837_BUCK2_CTRL		= 0x06,
    BD71837_BUCK3_CTRL		= 0x07,
    BD71837_BUCK4_CTRL		= 0x08,
    BD71837_BUCK5_CTRL		= 0x09,
    BD71837_BUCK6_CTRL		= 0x0A,
    BD71837_BUCK7_CTRL		= 0x0B,
    BD71837_BUCK8_CTRL		= 0x0C,
    BD71837_BUCK1_VOLT_RUN	= 0x0D,
    BD71837_BUCK1_VOLT_IDLE	= 0x0E,
    BD71837_BUCK1_VOLT_SUSP	= 0x0F,
    BD71837_BUCK2_VOLT_RUN	= 0x10,
    BD71837_BUCK2_VOLT_IDLE	= 0x11,
    BD71837_BUCK3_VOLT_RUN	= 0x12,
    BD71837_BUCK4_VOLT_RUN	= 0x13,
    BD71837_BUCK5_VOLT		= 0x14,
    BD71837_BUCK6_VOLT		= 0x15,
    BD71837_BUCK7_VOLT		= 0x16,
    BD71837_BUCK8_VOLT		= 0x17,
    BD71837_LDO1_VOLT		= 0x18,
    BD71837_LDO2_VOLT		= 0x19,
    BD71837_LDO3_VOLT		= 0x1A,
    BD71837_LDO4_VOLT		= 0x1B,
    BD71837_LDO5_VOLT		= 0x1C,
    BD71837_LDO6_VOLT		= 0x1D,
    BD71837_LDO7_VOLT		= 0x1E,
    BD71837_TRANS_COND0		= 0x1F,
    BD71837_TRANS_COND1		= 0x20,
    BD71837_VRFAULTEN		= 0x21,
    BD71837_MVRFLTMASK0		= 0x22,
    BD71837_MVRFLTMASK1		= 0x23,
    BD71837_MVRFLTMASK2		= 0x24,
    BD71837_RCVCFG		= 0x25,
    BD71837_RCVNUM		= 0x26,
    BD71837_PWRONCONFIG0	= 0x27,
    BD71837_PWRONCONFIG1	= 0x28,
    BD71837_RESETSRC		= 0x29,
    BD71837_MIRQ		= 0x2A,
    BD71837_IRQ			= 0x2B,
    BD71837_IN_MON		= 0x2C,
    BD71837_POW_STATE		= 0x2D,
    BD71837_OUT32K		= 0x2E,
    BD71837_REGLOCK		= 0x2F,
    BD71837_MUXSW_EN		= 0x30,
    BD71837_REG_NUM,
};

#endif
int power_bd71837_init(unsigned char bus);

#endif
#endif
