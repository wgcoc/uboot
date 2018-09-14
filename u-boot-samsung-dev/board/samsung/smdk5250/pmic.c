/*
 * (C) Copyright 2011 Samsung Electronics Co. Ltd
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#include <common.h>
#include <asm/arch/pmic.h>

void Delay(void)
{
	unsigned long i;
	for(i=0;i<DELAY;i++);
}

void IIC0_SCLH_SDAH(void)
{
	IIC0_ESCL_Hi;
	IIC0_ESDA_Hi;
	Delay();
}

void IIC0_SCLH_SDAL(void)
{
	IIC0_ESCL_Hi;
	IIC0_ESDA_Lo;
	Delay();
}

void IIC0_SCLL_SDAH(void)
{
	IIC0_ESCL_Lo;
	IIC0_ESDA_Hi;
	Delay();
}

void IIC0_SCLL_SDAL(void)
{
	IIC0_ESCL_Lo;
	IIC0_ESDA_Lo;
	Delay();
}

void IIC1_SCLH_SDAH(void)
{
	IIC1_ESCL_Hi;
	IIC1_ESDA_Hi;
	Delay();
}

void IIC1_SCLH_SDAL(void)
{
	IIC1_ESCL_Hi;
	IIC1_ESDA_Lo;
	Delay();
}

void IIC1_SCLL_SDAH(void)
{
	IIC1_ESCL_Lo;
	IIC1_ESDA_Hi;
	Delay();
}

void IIC1_SCLL_SDAL(void)
{
	IIC1_ESCL_Lo;
	IIC1_ESDA_Lo;
	Delay();
}

void IIC0_ELow(void)
{
	IIC0_SCLL_SDAL();
	IIC0_SCLH_SDAL();
	IIC0_SCLH_SDAL();
	IIC0_SCLL_SDAL();
}

void IIC0_EHigh(void)
{
	IIC0_SCLL_SDAH();
	IIC0_SCLH_SDAH();
	IIC0_SCLH_SDAH();
	IIC0_SCLL_SDAH();
}

void IIC1_ELow(void)
{
	IIC1_SCLL_SDAL();
	IIC1_SCLH_SDAL();
	IIC1_SCLH_SDAL();
	IIC1_SCLL_SDAL();
}

void IIC1_EHigh(void)
{
	IIC1_SCLL_SDAH();
	IIC1_SCLH_SDAH();
	IIC1_SCLH_SDAH();
	IIC1_SCLL_SDAH();
}

void IIC0_EStart(void)
{
	IIC0_SCLH_SDAH();
	IIC0_SCLH_SDAL();
	Delay();
	IIC0_SCLL_SDAL();
}

void IIC0_EEnd(void)
{
	IIC0_SCLL_SDAL();
	IIC0_SCLH_SDAL();
	Delay();
	IIC0_SCLH_SDAH();
}

void IIC0_EAck_write(void)
{
	unsigned long ack;

	IIC0_ESDA_INP;			// Function <- Input

	IIC0_ESCL_Lo;
	Delay();
	IIC0_ESCL_Hi;
	Delay();
	ack = GPD1DAT;
	IIC0_ESCL_Hi;
	Delay();
	IIC0_ESCL_Hi;
	Delay();

	IIC0_ESDA_OUTP;			// Function <- Output (SDA)

	ack = (ack>>0)&0x1;
//	while(ack!=0);

	IIC0_SCLL_SDAL();
}

void IIC0_EAck_read(void)
{
	IIC0_ESDA_OUTP;			// Function <- Output

	IIC0_ESCL_Lo;
	IIC0_ESCL_Lo;
	IIC0_ESDA_Lo;
	IIC0_ESCL_Hi;
	IIC0_ESCL_Hi;

	IIC0_ESDA_INP;			// Function <- Input (SDA)

	IIC0_SCLL_SDAL();
}

void IIC1_EStart(void)
{
	IIC1_SCLH_SDAH();
	IIC1_SCLH_SDAL();
	Delay();
	IIC1_SCLL_SDAL();
}

void IIC1_EEnd(void)
{
	IIC1_SCLL_SDAL();
	IIC1_SCLH_SDAL();
	Delay();
	IIC1_SCLH_SDAH();
}

void IIC1_EAck(void)
{
	unsigned long ack;

	IIC1_ESDA_INP;			// Function <- Input

	IIC1_ESCL_Lo;
	Delay();
	IIC1_ESCL_Hi;
	Delay();
	ack = GPD1DAT;
	IIC1_ESCL_Hi;
	Delay();
	IIC1_ESCL_Hi;
	Delay();

	IIC1_ESDA_OUTP;			// Function <- Output (SDA)

	ack = (ack>>2)&0x1;
//	while(ack!=0);

	IIC1_SCLL_SDAL();
}

void IIC0_ESetport(void)
{
	GPD1PUD &= ~(0xf<<0);	// Pull Up/Down Disable	SCL, SDA

	IIC0_ESCL_Hi;
	IIC0_ESDA_Hi;

	IIC0_ESCL_OUTP;		// Function <- Output (SCL)
	IIC0_ESDA_OUTP;		// Function <- Output (SDA)

	Delay();
}

void IIC1_ESetport(void)
{
	GPD1PUD &= ~(0xf<<4);	// Pull Up/Down Disable	SCL, SDA

	IIC1_ESCL_Hi;
	IIC1_ESDA_Hi;

	IIC1_ESCL_OUTP;		// Function <- Output (SCL)
	IIC1_ESDA_OUTP;		// Function <- Output (SDA)

	Delay();
}

void IIC0_EWrite (unsigned char ChipId, unsigned char IicAddr, unsigned char IicData)
{
	unsigned long i;

	IIC0_EStart();

////////////////// write chip id //////////////////
	for(i = 7; i>0; i--)
	{
		if((ChipId >> i) & 0x0001)
			IIC0_EHigh();
		else
			IIC0_ELow();
	}

	IIC0_ELow();	// write

	IIC0_EAck_write();	// ACK

////////////////// write reg. addr. //////////////////
	for(i = 8; i>0; i--)
	{
		if((IicAddr >> (i-1)) & 0x0001)
			IIC0_EHigh();
		else
			IIC0_ELow();
	}

	IIC0_EAck_write();	// ACK

////////////////// write reg. data. //////////////////
	for(i = 8; i>0; i--)
	{
		if((IicData >> (i-1)) & 0x0001)
			IIC0_EHigh();
		else
			IIC0_ELow();
	}

	IIC0_EAck_write();	// ACK

	IIC0_EEnd();
}

void IIC0_ERead (unsigned char ChipId, unsigned char IicAddr, unsigned char *IicData)
{
	unsigned long i, reg;
	unsigned char data = 0;

	IIC0_EStart();

////////////////// write chip id //////////////////
	for(i = 7; i>0; i--)
	{
		if((ChipId >> i) & 0x0001)
			IIC0_EHigh();
		else
			IIC0_ELow();
	}

	IIC0_ELow();	// write

	IIC0_EAck_write();	// ACK

////////////////// write reg. addr. //////////////////
	for(i = 8; i>0; i--)
	{
		if((IicAddr >> (i-1)) & 0x0001)
			IIC0_EHigh();
		else
			IIC0_ELow();
	}

	IIC0_EAck_write();	// ACK

	IIC0_EStart();

////////////////// write chip id //////////////////
	for(i = 7; i>0; i--)
	{
		if((ChipId >> i) & 0x0001)
			IIC0_EHigh();
		else
			IIC0_ELow();
	}

	IIC0_EHigh();	// read

	IIC0_EAck_write();	// ACK

////////////////// read reg. data. //////////////////
	IIC0_ESDA_INP;

	for(i = 8; i>0; i--)
	{
		IIC0_ESCL_Lo;
		Delay();
		IIC0_ESCL_Hi;
		Delay();
		reg = GPD1DAT;
		IIC0_ESCL_Hi;
		Delay();
		IIC0_ESCL_Hi;
		Delay();

		reg = (reg >> 0) & 0x1;

		data |= reg << (i-1);
	}

	IIC0_EAck_read();	// ACK
	IIC0_ESDA_OUTP;

	IIC0_EEnd();

	*IicData = data;
}


void IIC1_EWrite (unsigned char ChipId, unsigned char IicAddr, unsigned char IicData)
{
	unsigned long i;

	IIC1_EStart();

////////////////// write chip id //////////////////
	for(i = 7; i>0; i--)
	{
		if((ChipId >> i) & 0x0001)
			IIC1_EHigh();
		else
			IIC1_ELow();
	}

	IIC1_ELow();	// write 'W'

	IIC1_EAck();	// ACK

////////////////// write reg. addr. //////////////////
	for(i = 8; i>0; i--)
	{
		if((IicAddr >> (i-1)) & 0x0001)
			IIC1_EHigh();
		else
			IIC1_ELow();
	}

	IIC1_EAck();	// ACK

////////////////// write reg. data. //////////////////
	for(i = 8; i>0; i--)
	{
		if((IicData >> (i-1)) & 0x0001)
			IIC1_EHigh();
		else
			IIC1_ELow();
	}

	IIC1_EAck();	// ACK

	IIC1_EEnd();
}

void I2C_MAX8997_EnableReg(PMIC_RegNum eRegNum, u8 ucEnable)
{
	u8 reg_addr, reg_bitpos;
	u8 read_data;

	reg_bitpos = 0;
	if(eRegNum == 0)
	{
		reg_addr = 0x18;
	}
	else if(eRegNum == 1)
	{
		reg_addr = 0x21;
	}
	else if(eRegNum == 2)
	{
		reg_addr = 0x2A;
	}
	else if(eRegNum == 3)
	{
		reg_addr = 0x2C;
	}
	else if(eRegNum == 4)
	{
		reg_addr = 0x48;
		reg_bitpos = 0x6;
	}
	else if(eRegNum == 5)
	{
		reg_addr = 0x44;
		reg_bitpos = 0x6;
	}
	else if(eRegNum == 6)
	{
		reg_addr = 0x4D;
		reg_bitpos = 0x6;
	}
	else
		while(1);

	IIC0_ERead(MAX8997_ADDR, reg_addr, &read_data);

	read_data = (read_data & (~(1<<reg_bitpos))) | (ucEnable<<reg_bitpos);

	IIC0_EWrite(MAX8997_ADDR, reg_addr, read_data);
}

void I2C_MAX8997_VolSetting(PMIC_RegNum eRegNum, u8 ucVolLevel, u8 ucEnable)
{
	u8 reg_addr, reg_bitpos, reg_bitmask, vol_level;
	u8 read_data;

	reg_bitpos = 0;
	reg_bitmask = 0x3F;
	if(eRegNum == 0)
	{
		reg_addr = 0x19;
	}
	else if(eRegNum == 1)
	{
		reg_addr = 0x22;
	}
	else if(eRegNum == 2)
	{
		reg_addr = 0x2B;
	}
	else if(eRegNum == 3)
	{
		reg_addr = 0x2D;
	}
	else if(eRegNum == 4)
	{
		reg_addr = 0x48;
	}
	else if(eRegNum == 5)
	{
		reg_addr = 0x44;
	}
	else if(eRegNum == 6)
	{
		reg_addr = 0x4D;
	}
	else
		while(1);

	vol_level = ucVolLevel&reg_bitmask;

	IIC0_ERead(MAX8997_ADDR, reg_addr, &read_data);

	read_data = (read_data & (~(reg_bitmask<<reg_bitpos))) | (vol_level<<reg_bitpos);

	IIC0_EWrite(MAX8997_ADDR, reg_addr, read_data);

	I2C_MAX8997_EnableReg(eRegNum, ucEnable);
}

#define CALC_S5M8767_VOLT1(x)  ( (x<600) ? 0 : ((x-600)/6.25) )
#define CALC_S5M8767_VOLT2(x)  ( (x<650) ? 0 : ((x-650)/6.25) )

void I2C_S5M8767_VolSetting(PMIC_RegNum eRegNum, unsigned char ucVolLevel, unsigned char ucEnable)
{
	unsigned char reg_addr, reg_bitpos, reg_bitmask, vol_level;

	reg_bitpos = 0;
	reg_bitmask = 0xFF;
	if(eRegNum == 0)
	{
		reg_addr = 0x33;
	}
	else if(eRegNum == 1)
	{
	        reg_addr = 0x35;
	}
	else if(eRegNum == 2)
	{
	        reg_addr = 0x3E;
	}
	else if(eRegNum == 3)
	{
		reg_addr = 0x47;
	}
	else if(eRegNum == 4)
	{
		reg_addr = 0x50;
	}
	else
	      while(1);

	vol_level = ucVolLevel&reg_bitmask;
	IIC0_EWrite(S5M8767_ADDR, reg_addr, vol_level);
}

void I2C_MAX77686_EnableReg(PMIC_RegNum eRegNum, u8 ucEnable)
{
	u8 reg_addr, reg_bitpos;
	u8 read_data;

	reg_bitpos = 0;
	if(eRegNum == 0)
	{
		reg_addr = 0x10;
	}
	else if(eRegNum == 1)
	{
		reg_addr = 0x12;
		reg_bitpos = 0x4;
	}
	else if(eRegNum == 2)
	{
		reg_addr = 0x1C;
		reg_bitpos = 0x4;
	}
	else if(eRegNum == 3)
	{
		reg_addr = 0x26;
		reg_bitpos = 0x4;
	}
	else
		while(1);

	IIC0_ERead(MAX77686_ADDR, reg_addr, &read_data);

	read_data = (read_data & (~(0x3<<reg_bitpos))) | (ucEnable<<reg_bitpos);

	IIC0_EWrite(MAX77686_ADDR, reg_addr, read_data);
}

void I2C_MAX77686_VolSetting(PMIC_RegNum eRegNum, u8 ucVolLevel, u8 ucEnable)
{
	u8 reg_addr, vol_bitpos, reg_bitmask, vol_level;
	u8 read_data;

	vol_bitpos = 0;
	reg_bitmask = 0x3F;
	if(eRegNum == 0)
	{
		reg_addr = 0x11;
	}
	else if(eRegNum == 1)
	{
		reg_addr = 0x14;
		reg_bitmask = 0xff;
	}
	else if(eRegNum == 2)
	{
		reg_addr = 0x1E;
	}
	else if(eRegNum == 3)
	{
		reg_addr = 0x28;
	}
	else if(eRegNum == 4)
	{
		reg_addr = 0x41;
	}
	else if(eRegNum == 5)
	{
		reg_addr = 0x42;
	}
	else if(eRegNum == 6)
	{
		reg_addr = 0x44;
	}
	else if(eRegNum == 7)
	{
		reg_addr = 0x49;
	}
	else if(eRegNum == 8)
	{
		reg_addr = 0x46;
	}
	else
		while(1);

	vol_level = ucVolLevel;

	IIC0_ERead(MAX77686_ADDR, reg_addr, &read_data);

	read_data = (read_data & (~(reg_bitmask<<vol_bitpos))) | (vol_level<<vol_bitpos);

	if (reg_addr < 0x40) {
		/* BUCK Voltage Set */
		IIC0_EWrite(MAX77686_ADDR, reg_addr, read_data);
		/* Buck Enable */
		I2C_MAX77686_EnableReg(eRegNum, ucEnable);
	} else {
		/* LDO Voltage Set & Enable */
		read_data = (read_data | ucEnable);
		IIC0_EWrite(MAX77686_ADDR, reg_addr, read_data);
	}

}

void pmic8767_init(void)
{
	float vdd_arm, vdd_int, vdd_g3d;
	float vdd_mif;
	u8 read_data;

	vdd_arm = CONFIG_PM_VDD_ARM;
	vdd_int = CONFIG_PM_VDD_INT;
	vdd_g3d = CONFIG_PM_VDD_G3D;
	vdd_mif = CONFIG_PM_VDD_MIF;

	IIC0_ESetport();
	IIC1_ESetport();

#ifdef CONFIG_DDR3
	I2C_S5M8767_VolSetting(PMIC_BUCK1, CALC_S5M8767_VOLT2(vdd_mif * 1000), 1);
	I2C_S5M8767_VolSetting(PMIC_BUCK2, CALC_S5M8767_VOLT1(vdd_arm * 1000), 1);
	I2C_S5M8767_VolSetting(PMIC_BUCK3, CALC_S5M8767_VOLT1(vdd_int * 1000), 1);
	I2C_S5M8767_VolSetting(PMIC_BUCK4, CALC_S5M8767_VOLT1(vdd_g3d * 1000), 1);

	IIC0_EWrite(S5M8767_ADDR, 0x59, 0x4C); // BUCK8 1.4 -> 1.7V
	IIC0_EWrite(S5M8767_ADDR, 0x5D, 0xDC); // LDO2 1.2 -> 1.5V

	IIC0_EWrite(S5M8767_ADDR, 0x34, 0x78);
	IIC0_EWrite(S5M8767_ADDR, 0x3d, 0x58);
	IIC0_EWrite(S5M8767_ADDR, 0x46, 0x78);

	IIC0_EWrite(S5M8767_ADDR, 0x5A, 0x58);

	IIC0_EWrite(S5M8767_ADDR, 0x65, 0xCE); // LDO7 1.0 -> 1.2V
#else
	I2C_S5M8767_VolSetting(PMIC_BUCK1, CALC_S5M8767_VOLT2(vdd_mif * 1000), 1);
	I2C_S5M8767_VolSetting(PMIC_BUCK2, CALC_S5M8767_VOLT1(vdd_arm * 1000), 1);
	I2C_S5M8767_VolSetting(PMIC_BUCK3, CALC_S5M8767_VOLT1(vdd_int * 1000), 1);
	I2C_S5M8767_VolSetting(PMIC_BUCK4, CALC_S5M8767_VOLT1(vdd_g3d * 1000), 1);
	IIC0_EWrite(S5M8767_ADDR, 0x34, 0x78);
	IIC0_EWrite(S5M8767_ADDR, 0x3d, 0x58);
	IIC0_EWrite(S5M8767_ADDR, 0x46, 0x78);

	IIC0_EWrite(S5M8767_ADDR, 0x5A, 0x58);

	IIC0_EWrite(S5M8767_ADDR, 0x63, 0xE8); // LDO5
	IIC0_EWrite(S5M8767_ADDR, 0x64, 0xD0); // LDO6
	IIC0_EWrite(S5M8767_ADDR, 0x65, 0xD0); // LDO7

#endif

}

void pmic_init(void)
{
	float vdd_arm, vdd_int, vdd_g3d;
	float vdd_mif;
#if defined(CONFIG_PM_VDD_LDO10)
	float vdd_apll;
#endif
#if defined(CONFIG_PM_VDD_LDO21)
	float vdd_ldo21;
#endif
	u8 read_data;

	vdd_arm = CONFIG_PM_VDD_ARM;
	vdd_int = CONFIG_PM_VDD_INT;
	vdd_g3d = CONFIG_PM_VDD_G3D;
	vdd_mif = CONFIG_PM_VDD_MIF;
#if defined(CONFIG_PM_VDD_LDO10)
	vdd_apll = CONFIG_PM_VDD_LDO10;
#endif
#if defined(CONFIG_PM_VDD_LDO21)
	vdd_ldo21 = CONFIG_PM_VDD_LDO21;
#endif

	IIC0_ESetport();
	IIC1_ESetport();
	/* read ID */
	IIC0_ERead(MAX8997_ADDR, 0, &read_data);
	if (read_data == 0x77) {
		I2C_MAX8997_VolSetting(PMIC_BUCK1, CALC_MAXIM_BUCK1245_VOLT(vdd_arm * 1000), 1);
		I2C_MAX8997_VolSetting(PMIC_BUCK2, CALC_MAXIM_BUCK1245_VOLT(vdd_int * 1000), 1);
		I2C_MAX8997_VolSetting(PMIC_BUCK3, CALC_MAXIM_BUCK37_VOLT(vdd_g3d * 1000), 1);
		I2C_MAX8997_VolSetting(PMIC_BUCK4, CALC_MAXIM_BUCK1245_VOLT(vdd_mif * 1000), 1);
#if defined(CONFIG_PM_VDD_LDO10)
		/* LDO10: APLL VDD */
		I2C_MAX8997_VolSetting(PMIC_LDO10, CALC_MAXIM_ALL_LDO(vdd_apll * 1000), 3);
#endif
#if defined(CONFIG_PM_VDD_LDO21)
		/* LDO21: VDDQ_M1_M, VDDQ_M2_M */
		I2C_MAX8997_VolSetting(PMIC_LDO21, CALC_MAXIM_ALL_LDO(vdd_ldo21 * 1000), 3);
#endif
	} else {
		/* ARM VDD */
		I2C_MAX77686_VolSetting(PMIC_BUCK2, CALC_MAXIM77686_BUCK234_VOLT(vdd_arm * 1000), 1);
		/* INT VDD */
		I2C_MAX77686_VolSetting(PMIC_BUCK3, CALC_MAXIM77686_BUCK234_VOLT(vdd_int * 1000), 1);
		/* G3D VDD */
		I2C_MAX77686_VolSetting(PMIC_BUCK4, CALC_MAXIM77686_BUCK234_VOLT(vdd_g3d * 1000), 1);
		/* MIF VDD */
		I2C_MAX77686_VolSetting(PMIC_BUCK1, CALC_MAXIM77686_BUCK156789_VOLT(vdd_mif * 1000), 3);
#if defined(CONFIG_PM_VDD_LDO10)
		/* LDO7: APLL VDD */
		I2C_MAX77686_VolSetting(0x08, CALC_MAXIM77686_LDO1267815_VOLT(vdd_apll * 1000), 3);
#endif
#if defined(CONFIG_PM_VDD_LDO21)
		/* LDO2: VDDQ_M1_M, VDDQ_M2_M */
		I2C_MAX77686_VolSetting(0x04, CALC_MAXIM77686_LDO1267815_VOLT(vdd_ldo21 * 1000), 3);
#endif
	}

}
