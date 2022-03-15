#pragma once


// Bit 0 - Enable TMU 0
// Bit 1 - Enable TMU 1
// Bit 2 - Enable TMU 2
#define REG_TMU_TSTR	(*((unsigned char*) 0xA4490004))

/*
   TCOR : Constant register
     32 bit value timer is reset to when it underflows
   TCNT : Counter register
	 Decremented when the timer is active based on Time prescaler value.
	 When it hits 0, optional interrupt occurs and reset to TCOR value
   TCR : Control Register
	 Bit 15-9 : Reserved, always 0
	 Bit 8    : Underflow flag, set when counter reaches 0 
	 Bit 7-6  : Reserved, always 0
	 Bit 5    : Interrupt control, 1 to enable when underflow occurs
	 Bit 3-4  : Reserved, always 0
	 Bit 2-0  : Time prescaler : 
				  000 - Clock rate / 4
				  001 - Clock rate / 16
				  010 - Clock rate / 64
				  011 - Clock rate / 256
				  100 - Clock rate / 1024
 */
#define REG_TMU_TCOR_0	(*((unsigned int*) 0xA4490008))
#define REG_TMU_TCNT_0	(*((unsigned int*) 0xA449000C))
#define REG_TMU_TCR_0	(*((unsigned short*) 0xA4490010))

#define REG_TMU_TCOR_1	(*((unsigned int*) 0xA4490014))
#define REG_TMU_TCNT_1	(*((unsigned int*) 0xA4490018))
#define REG_TMU_TCR_1	(*((unsigned short*) 0xA449001C))

#define REG_TMU_TCOR_2	(*((unsigned int*) 0xA4490020))
#define REG_TMU_TCNT_2	(*((unsigned int*) 0xA4490024))
#define REG_TMU_TCR_2	(*((unsigned short*) 0xA4490028))