/*
 * a2560_platform.h
 *
 *  Created on: Feb 19, 2022
 *      Author: micahbly
 */

#ifndef A2560_PLATFORM_H_
#define A2560_PLATFORM_H_


/* about this class
 *
 *
 *
 *** things this class needs to be able to do
 * nothing - this is not a functional class, but a set of useful headers providing structs and defines useful for developing on the A2560 platform
 *
 *** things objects of this class have
 *
 *
 */


/*****************************************************************************/
/*                                Includes                                   */
/*****************************************************************************/

#include <stdint.h>


/*****************************************************************************/
/*                            Macro Definitions                              */
/*****************************************************************************/

// 
// // eliminate __huge extension keyword unless we are building with Calypsi for 65816
// #if defined(__VBCC__)
// 	#define __huge  
// #endif

#define ID_CHANNEL_A				0	// for use in text() calls, etc. 
#define ID_CHANNEL_B				1	// for use in text() calls, etc.

// A2560 OTHER
#define EA_MCP						(char*)0x010000	// start of MCP kernel
#define EA_USER						(char*)0x020000	// start of user space. ie, put your program here.


// adapted from vinz67
#define R8(x)						*((volatile __far uint8_t* const)(x))			// make sure we read an 8 bit byte; for VICKY registers, etc.
#define P8(x)						(volatile __far uint8_t* const)(x)			// make sure we read an 8 bit byte; for VICKY registers, etc.
#define R16(x)						*((volatile __far uint16_t* const)(x))		// make sure we read an 16 bit byte; for RNG etc.
#define P16(x)						(volatile unsigned short* const)(x)		// make sure we read an 16 bit short; for VICKY registers, etc.
#define R32(x)						*((volatile __far uint32_t* const)(x))		// make sure we read an 32 bit byte;
#define P32(x)						(volatile unsigned long* const)(x)		// make sure we read a 32 bit long; for VICKY registers, etc.
// and near variants. keeping far variants as regular "R8", etc, for backwards compatibility with other 6502 code of mine
#define NR8(x)						*((volatile uint8_t* const)(x))			// make sure we read an 8 bit byte; for VICKY registers, etc.
#define NP8(x)						(volatile uint8_t* const)(x)			// make sure we read an 8 bit byte; for VICKY registers, etc.
#define NR16(x)						*((volatile uint16_t* const)(x))		// make sure we read an 16 bit byte; for RNG etc.

#define TEXT_COL_COUNT_FOR_PLOTTING_A2560K		100	// regardless of visible cols (between borders), VRAM seems to be fixed at 80 cols across.
#define TEXT_ROW_COUNT_FOR_PLOTTING_A2560K		75	// regardless of visible rows (between borders), VRAM seems to be fixed at 60 rows up/down.
#define TEXT_COL_COUNT_FOR_PLOTTING				TEXT_COL_COUNT_FOR_PLOTTING_A2560K	// regardless of visible cols (between borders), VRAM seems to be fixed at 80 cols across.
#define TEXT_ROW_COUNT_FOR_PLOTTING				TEXT_ROW_COUNT_FOR_PLOTTING_A2560K	// regardless of visible rows (between borders), VRAM seems to be fixed at 60 rows up/down.

#define TEXT_ROW_COUNT_60HZ			60
#define TEXT_ROW_COUNT_70HZ			50
#define TEXT_ROW_COUNT_DEBUG_SCREEN	75	// for A2560K only, the "debug" screen that runs text-only at 800x600

#define TEXT_FONT_WIDTH				8	// for text mode, the width of the fixed-sized font chars
#define TEXT_FONT_HEIGHT			8	// for text mode, the height of the fixed-sized font chars.
#define TEXT_FONT_BYTE_SIZE			(8*256)

// general
#define MAX_STRING_COMP_LEN		256		//!< 255 + terminator is max string size for compares

// word-wrap and string measurement related
#define GEN_NO_STRLEN_CAP		-1		//!< for the xxx_DrawString function's max_chars parameter, the value that corresponds to 'draw the entire string if it fits, do not cap it at n characters' 
#define WORD_WRAP_MAX_LEN		12800	//!< For the xxx_DrawStringInBox function, the strnlen char limit. 128*100 (1024x768 with 8x8 char grid). 

// file-related
#define FILE_MAX_PATHNAME_SIZE	260	// https://www.keil.com/pack/doc/mw/FileSystem/html/fat_fs.html


#define VIDEO_MODE_BYTE				0x01	//!> the byte offset from system control register that holds the video mode bits
#define VIDEO_MODE_BIT1				0x01	//!> the bits in the 2nd byte of the system control register that define video mode (resolution). if this bit is set, resolution is 800x600 (bit 2 not set) or 640x400 (bit 2 set) or 1024x768 (if bit 11 is set)
// from Stefany on 2022/03/06 discord:
// Channel B bit 8 and 9 are the bit that select the video mode. 640x480, 800x600 and 640x400. No 1024x768 
// Channel A bit 8 and 9 are ignored
// It is bit 11 that gives you the choice between 800x600 or 1024x768.
#define VIDEO_MODE_BIT2				0x02	//!> the bits in the 2nd byte of the system control register that define video mode (resolution). if set on Chan B, you get 640x400. (also requires bit1 to be set?)
#define GAMMA_MODE_ONOFF_BITS		0x03	//!>the bits in the 3rd byte of the system control register control gamma correction on/off

	#define BORDER_X_MASK				0xFFFF00FF	//!> the mask for the Border control register (0x0004) long, for the X border
	#define BORDER_Y_MASK				0xFF00FFFF	//!> the mask for the Border control register (0x0004) long, for the Y border
	#define BORDER_CTRL_OFFSET_L		0x01		//!> the (long) offset from the VICKY control register to the border control register		
	#define BORDER_COLOR_OFFSET_L		0x02		//!> the (long) offset from the VICKY control register to the border color register		
	#define BACKGROUND_COLOR_OFFSET_L	0x03		//!> the (long) offset from the VICKY control register to the background color register		
	#define CURSOR_CTRL_OFFSET_L		0x04		//!> the (long) offset from the VICKY control register to the cursor control register		
	#define CURSOR_POS_OFFSET_L			0x04		//!> the (long) offset from the VICKY control register to the cursor position register		
	#define LN_INTERRUPT_01_OFFSET_L	0x05		//!> the (long) offset from the VICKY control register to the line interrupts 0 and 1 registers		
	#define BITMAP_L0_CTRL_L			0x40		//!> the (long) offset from the VICKY control register to the bitmap layer0 control register (foreground layer)		
	#define BITMAP_L0_VRAM_ADDR_OFFSET_L	0x41		//!> the (long) offset from the VICKY control register to the bitmap layer0 VRAM address pointer)		
	#define BITMAP_L1_CTRL_L			0x42		//!> the (long) offset from the VICKY control register to the bitmap layer1 control register (background layer)		
	#define BITMAP_L1_VRAM_ADDR_OFFSET_L	0x43		//!> the (long) offset from the VICKY control register to the bitmap layer1 VRAM address pointer)		
	#define CLUT0_OFFSET_L				0x800		//!> the (long) offset from the VICKY control register to the first CLUT RAM space
	#define CLUT1_OFFSET_L				0x900		//!> the (long) offset from the VICKY control register to the 2nd CLUT RAM space
	#define CLUT2_OFFSET_L				0xA00		//!> the (long) offset from the VICKY control register to the 3rd CLUT RAM space
	#define CLUT3_OFFSET_L				0xB00		//!> the (long) offset from the VICKY control register to the 4th CLUT RAM space
	#define CLUT4_OFFSET_L				0xC00		//!> the (long) offset from the VICKY control register to the 5th CLUT RAM space
	#define CLUT5_OFFSET_L				0xD00		//!> the (long) offset from the VICKY control register to the 6th CLUT RAM space
	#define CLUT6_OFFSET_L				0xE00		//!> the (long) offset from the VICKY control register to the 7th CLUT RAM space
	#define CLUT7_OFFSET_L				0xF00		//!> the (long) offset from the VICKY control register to the 8th CLUT RAM space


	// ** C256 (VICKY II)
	
	// byte swapper: 68000 is big endian; 65816 is little endian
	#define BSWAP(word)	( (((word) >> 8) & 0x00FF) | (((word) << 8) & 0xFF00) )
	
	#define VICKY_C256					0x00af0000		// Vicky II offset/first register
	#define VICKY_II_MASTER_CTRL_REG_L	0x00af0000		// Vicky II Master Control Register - low - graphic mode/text mode/etc.
	#define VICKY_II_MASTER_CTRL_REG_H	0x00af0001		// Vicky II Master Control Register - high - screen res, etc.
	#define VICKY_II_GAMMA_CTRL_REG		0x00af0002		// Vicky II Gamma Control Register
	#define VICKY_II_BORDER_CTRL_REG	0x00af0004		// Vicky II Border Control Register
	#define VICKY_II_BORDER_COLOR_B		0x00af0005		// Vicky II Border Color Blue
	#define VICKY_II_BORDER_COLOR_G		0x00af0006		// Vicky II Border Color Green
	#define VICKY_II_BORDER_COLOR_R		0x00af0007		// Vicky II Border Color Red
	#define VICKY_II_BORDER_X_SIZE		0x00af0008		// Vicky II Border X size in pixels
	#define VICKY_II_BORDER_Y_SIZE		0x00af0009		// Vicky II Border Y size in pixels
	
	#define VICKY_II_BACKGROUND_COLOR_B		0x00af000D		// Vicky II background color Blue
	#define VICKY_II_BACKGROUND_COLOR_G		0x00af000E		// Vicky II background color Green
	#define VICKY_II_BACKGROUND_COLOR_R		0x00af000F		// Vicky II background color Red

	#define BITMAP_L0_CTRL				0x00af0100		//!> bitmap layer0 control register (background layer)		
	#define BITMAP_L0_VRAM_ADDR_L		0x00af0101		//!> bitmap layer0 VRAM address pointer)		
	#define BITMAP_L0_VRAM_ADDR_M		0x00af0102		//!> bitmap layer0 VRAM address pointer)		
	#define BITMAP_L0_VRAM_ADDR_H		0x00af0103		//!> bitmap layer0 VRAM address pointer)		
	#define BITMAP_L1_CTRL				0x00af0108		//!> bitmap layer1 control register (foreground layer)		
	#define BITMAP_L1_VRAM_ADDR_L		0x00af0109		//!> bitmap layer1 VRAM address pointer)		
	#define BITMAP_L1_VRAM_ADDR_M		0x00af010A		//!> bitmap layer1 VRAM address pointer)		
	#define BITMAP_L1_VRAM_ADDR_H		0x00af010B		//!> bitmap layer1 VRAM address pointer)		

	#define TEXT_RAM_C256			(char*)0xafa000			// CS_TEXT_MEM_PTR	Text Memory Block
	#define TEXT_ATTR_C256			(char*)0xafc000			// CS_COLOR_MEM_PTR	Color Text Memory Block
	#define TEXT_FORE_LUT_C256		(char*)0xaf1f40			// FG_CHAR_LUT_PTR	Text Foreground Look-Up Table
	#define TEXT_BACK_LUT_C256		(char*)0xaf1f80			// BG_CHAR_LUT_PTR	Text Background Look-Up Table
	#define FONT_MEMORY_BANK_C256	(char*)0xaf8000			// FONT_MEMORY_BANK0	FONT Character Graphic Mem
	#define VICKY_II_CLUT0				0xaf2000				// each addition LUT is 400 offset from here
	#define VICKY_II_CLUT1				(VICKY_II_CLUT0 + 0x400)	// each addition LUT is 400 offset from here
	#define VICKY_II_CLUT2				(VICKY_II_CLUT1 + 0x400)	// each addition LUT is 400 offset from here
	#define VICKY_II_CLUT3				(VICKY_II_CLUT2 + 0x400)	// each addition LUT is 400 offset from here
	#define VICKY_II_CLUT4				(VICKY_II_CLUT3 + 0x400)	// each addition LUT is 400 offset from here
	#define VICKY_II_CLUT5				(VICKY_II_CLUT4 + 0x400)	// each addition LUT is 400 offset from here
	#define VICKY_II_CLUT6				(VICKY_II_CLUT5 + 0x400)	// each addition LUT is 400 offset from here
	#define VICKY_II_CLUT7				(VICKY_II_CLUT6 + 0x400)	// each addition LUT is 400 offset from here


	#define GABE_SYS_STAT				0x00AFE887	//!> The gabe register holding the machine ID. Machine ID is stored in 4 least significant bits
	
	#define VIDEO_MODE_REG_OFFSET_B		0x01		//!> the (byte) offset from the VICKY control register to the Video mode reg (MASTER_CTRL_REG_H)
	#define GAMMA_CTRL_REG_OFFSET_B		0x02		//!> the (byte) offset from the VICKY control register to the Gamma control register
	#define VICKY_RESERVED_OFFSET_B		0x03		//!> the (byte) offset from the VICKY control register to the VKY_RESERVED_O1 byte
	#define BORDER_CTRL_OFFSET_B		0x04		//!> the (byte) offset from the VICKY control register to the border control register
	#define BORDER_COLOR_OFFSET_B_B		0x05		//!> the (byte) offset from the VICKY control register to the border color register	for Blue
	#define BORDER_COLOR_OFFSET_G_B		0x06		//!> the (byte) offset from the VICKY control register to the border color register	for Green
	#define BORDER_COLOR_OFFSET_R_B		0x07		//!> the (byte) offset from the VICKY control register to the border color register	for Red
	#define BORDER_X_SIZE_B				0x08		//!> the (byte) offset from the VICKY control register to the border X-size
	#define BORDER_Y_SIZE_B				0x09		//!> the (byte) offset from the VICKY control register to the border Y-size
	#define BACKGROUND_COLOR_OFFSET_B_B	0x0D		//!> the (byte) offset from the VICKY control register to the background color register	for Blue
	#define BACKGROUND_COLOR_OFFSET_G_B	0x0E		//!> the (byte) offset from the VICKY control register to the background color register	for Green
	#define BACKGROUND_COLOR_OFFSET_R_B	0x0F		//!> the (byte) offset from the VICKY control register to the background color register	for Red
	#define CURSOR_CTRL_OFFSET_B		0x10		//!> the (byte) offset from the VICKY control register to the cursor control register		
	#define START_ADDR_OFFSET_B			0x11		//!> the (byte) offset from the VICKY control register to change the starting address of the text mode buffer (in x)
	#define CURSOR_CHAR_OFFSET_B		0x12		//!> the (byte) offset from the VICKY control register to the Char used as the Cursor	
	#define CURSOR_COLOR_OFFSET_B		0x13		//!> the (byte) offset from the VICKY control register to the color used for the Cursor	
	#define LN_INTERRUPT_01_OFFSET_B	0x1B		//!> the (byte) offset from the VICKY control register to the line interrupts 0 and 1 registers		
	#define BITMAP_L0_CTRL_B			0x100		//!> the (byte) offset from the VICKY control register to the bitmap layer0 control register (foreground layer)		
	#define BITMAP_L0_VRAM_ADDR_L_B		0x101		//!> the (byte) offset from the VICKY control register to the bitmap layer0 VRAM address pointer)		
	#define BITMAP_L0_VRAM_ADDR_M_B		0x102		//!> the (byte) offset from the VICKY control register to the bitmap layer0 VRAM address pointer)		
	#define BITMAP_L0_VRAM_ADDR_H_B		0x103		//!> the (byte) offset from the VICKY control register to the bitmap layer0 VRAM address pointer)		
	#define BITMAP_L1_CTRL_B			0x108		//!> the (byte) offset from the VICKY control register to the bitmap layer1 control register (foreground layer)		
	#define BITMAP_L1_VRAM_ADDR_L_B		0x109		//!> the (byte) offset from the VICKY control register to the bitmap layer1 VRAM address pointer)		
	#define BITMAP_L1_VRAM_ADDR_M_B		0x10A		//!> the (byte) offset from the VICKY control register to the bitmap layer1 VRAM address pointer)		
	#define BITMAP_L1_VRAM_ADDR_H_B		0x10B		//!> the (byte) offset from the VICKY control register to the bitmap layer1 VRAM address pointer)		
	#define CLUT0_OFFSET_B				0x2000		//!> the (byte) offset from the VICKY control register to the first CLUT RAM space
	#define CLUT1_OFFSET_B				0x2400		//!> the (byte) offset from the VICKY control register to the 2nd CLUT RAM space
	#define CLUT2_OFFSET_B				0x2800		//!> the (byte) offset from the VICKY control register to the 3rd CLUT RAM space
	#define CLUT3_OFFSET_B				0x2C00		//!> the (byte) offset from the VICKY control register to the 4th CLUT RAM space
	#define CLUT4_OFFSET_B				0x3000		//!> the (byte) offset from the VICKY control register to the 5th CLUT RAM space
	#define CLUT5_OFFSET_B				0x3400		//!> the (byte) offset from the VICKY control register to the 6th CLUT RAM space
	#define CLUT6_OFFSET_B				0x3800		//!> the (byte) offset from the VICKY control register to the 7th CLUT RAM space
	#define CLUT7_OFFSET_B				0x3C00		//!> the (byte) offset from the VICKY control register to the 8th CLUT RAM space
	#define FONT_MEMORY_BANK0_OFFSET_B	0x8000		//!> the (byte) offset from the VICKY control register to the font memory for bank0



// VICKY RESOLUTION FLAGS Per A2560K_UM_Rev0.0.1.pdf and A2560U_UM_Rev0.0.2.pdf
// VICKY II / VICKY III Chan B
// 640x480  @ 60FPS > 0 0
// 800x600  @ 60FPS > 0 1
// reserved         > 1 0
// 640x400  @ 70FPS > 1 1

// VICKY III Chan A
// 800x600  @ 60FPS > 0 0
// 1024x768 @ 60FPS > 0 1
// reserved         > 1 0
// reserved         > 1 1

#define VICKY_II_RES_640X480_FLAGS		0x00	// 0b00000000
#define VICKY_II_RES_800X600_FLAGS		0x01	// 0b00000001
#define VICKY_II_PIX_DOUBLER_FLAGS		0x02	// 0b00000001
#define VICKY_II_RES_640X400_FLAGS		0x03	// 0b00000011

#define VICKY_IIIB_RES_640X480_FLAGS	0x00	// 0b00000000
#define VICKY_IIIB_RES_800X600_FLAGS	0x01	// 0b00000001
#define VICKY_IIIB_RES_640X400_FLAGS	0x03	// 0b00000011

#define VICKY_IIIA_RES_800X600_FLAGS	0x00	// 0b00000000
#define VICKY_IIIA_RES_1024X768_FLAGS	0x08	// 0b00000100



// ** A2560K GAVIN

#define GAVIN_CONTROL_REG			0xfec00000	// GAVIN Control Register - 4b - RW
	// flags for byte 0 (000000FF)
	#define FLAG_SYS0_REG_POWER_LED			0b00000001		// power LED on (1) or off (0)
	#define FLAG_SYS0_REG_SD_LED			0b00000010		// disk (SD) LED on (1) or off (0)
	#define FLAG_SYS0_REG_RES_2				0b00000100		// reserved
	#define FLAG_SYS0_REG_RES_3				0b00001000		// reserved
	#define FLAG_SYS0_REG_R_BUZZER			0b00010000		// allows programmer to generate buzzer sounds by toggling on/off.
	#define FLAG_SYS0_REG_RES_5				0b00100000		// reserved
	#define FLAG_SYS0_REG_RES_6				0b01000000		// reserved
	#define FLAG_SYS0_REG_RES_7				0b10000000		// reserved
	// flags for byte 1 (0000FF00)
	#define FLAG_SYS1_REG_RES_0				0b00000001		// reserved
	#define FLAG_SYS1_REG_RES_1				0b00000010		// reserved
	#define FLAG_SYS1_REG_RES_2				0b00000100		// reserved
	#define FLAG_SYS1_REG_RES_3				0b00001000		// reserved
	#define FLAG_SYS1_REG_RES_4				0b00010000		// reserved
	#define FLAG_SYS1_REG_RES_5				0b00100000		// reserved
	#define FLAG_SYS1_REG_RES_6				0b01000000		// reserved
	#define FLAG_SYS1_REG_W_RESET			0b10000000		// set, then unset to trigger. See also bytes 2 and 3 for more requirements.
	// bytes 2 and 3 enable system reset and protect against accidental reset. to reset, fill them with 0xDEAD, then set and unset FLAG_SYS1_REG_W_RESET

// note: A linear feedback shift register (LFSR) is a device that generates a variety of bit patterns, including pseudo-random sequences, by connecting some of its outputs back to its input. To enable the random number generator, set bit 1 of the first byte to enable the LFSR, write a Value to setup the LFSR Seed (bytes 2-3), then set bit#1 of LFSR Control Register. Then, clear the bit. This will latch the value of the Seed in the LFSR.
#define GAVIN_LFSR_CONTROL			0xfec00004	// LFSR Control Register - 4b - RW
	// flags for byte 0 (000000FF)
	#define FLAG_LFSR0_LFSR_ENABLE			0b00000001		// enables/disables LFSR
	#define FLAG_LFSR0_SEED_WRITE			0b00000010		// after putting in a seed value in bytes 2-3, set this and unset this bit to set the seed.
	#define FLAG_LFSR0_REG_RES_2			0b00000100		// reserved
	#define FLAG_LFSR0_REG_RES_3			0b00001000		// reserved
	#define FLAG_LFSR0_REG_RES_4			0b00010000		// reserved
	#define FLAG_LFSR0_REG_RES_5			0b00100000		// reserved
	#define FLAG_LFSR0_REG_RES_6			0b01000000		// reserved
	#define FLAG_LFSR0_REG_RES_7			0b10000000		// reserved
	// flags for byte 1 (0000FF00)
	#define FLAG_LFSR1_REG_RES_0			0b00000001		// reserved
	#define FLAG_LFSR1_REG_RES_1			0b00000010		// reserved
	#define FLAG_LFSR1_REG_RES_2			0b00000100		// reserved
	#define FLAG_LFSR1_REG_RES_3			0b00001000		// reserved
	#define FLAG_LFSR1_REG_RES_4			0b00010000		// reserved
	#define FLAG_LFSR1_REG_RES_5			0b00100000		// reserved
	#define FLAG_LFSR1_REG_RES_6			0b01000000		// reserved
	#define FLAG_LFSR1_REG_RES_7			0b10000000		// reserved
	// bytes 2 and 3 are the seed value for the random number generator. set this once, then set and unset FLAG_LFSR0_SEED_WRITE

#define RANDOM_NUM_GEN_ENABLE		(GAVIN_LFSR_CONTROL)	// for compatibility with other Foenix code
#define GAVIN_LFSR_OUTPUT			0xfec00008	// LFSR Output Value - 4b - RO
	// bytes 0 and 1 are the generated random number - regenerated every time you read the register
	// flags for byte 2 (00FF0000)
	#define FLAG_LFSR_STATUS_CTRL0			0b00000001		// status bit
	#define FLAG_LFSR_STATUS_CTRL1			0b00000010		// status bit
	#define FLAG_LFSR_STATUS_CTRL2			0b00000100		// status bit
	#define FLAG_LFSR_STATUS_CTRL3			0b00001000		// status bit
	#define FLAG_LFSR_STATUS_CTRL4			0b00010000		// status bit
	#define FLAG_LFSR_STATUS_CTRL5			0b00100000		// status bit
	#define FLAG_LFSR_STATUS_CTRL6			0b01000000		// status bit
	// byte 3 will always read 0
	
#define RANDOM_NUM_GEN				(GAVIN_LFSR_OUTPUT)		// for compatibility with other Foenix code

#define GAVIN_POWER_LED_RGB			0xfec00008	// RGB Power LED - 4b - WO
	// byte 0 is the blue
	// byte 1 is the green
	// byte 2 is the red
	// byte 3 is ignored

#define GAVIN_MACHINE_ID			0xfec0000c	// Machine ID - 4b - RO	
	// MB: not clear from 0.0.2 manual vs subsequent changes to other machines, if this will stay same in future. 
	// byte 0 appears to be a speed ID
	// byte 1 appears to be for CPU ID
#define MACHINE_ID_REGISTER			(GAVIN_MACHINE_ID)

#define GAVIN_KEYBOARD_LED_RGB		0xfec0000c	// Keyboard Status LEDs - 4b - WO
	// The Value of the status led are simply on or off, so only 7 basic colors can be programmed.
	// bits 0-2 are RGB for the top right LED
	// bits 3-5 are RGB for the middle right LED
	// bits 6-8 are RGB for the bottom right LED
	// bits 9-11 are RGB for the middle left LED

#define GAVIN_CHIP_VERSION			0xfec00010	// Chip version and number - 4b - RO	
	// bytes 0-1 are the chip version in hex
	// bytes 2-3 are the chip number in hex

#define GAVIN_FIRMWARE_DATE			0xfec00014	// Firmware date - 4b - RO	
	// byte 0 is the year in decimal
	// byte 1 is the month in decimal
	// byte 2 is the day in decimal
	// byte 3 is reserved

#define GAVIN_HARDWARE_DATE_A		0xfec00018	// Hardware date - 4b - RO	
	// byte 0 EOS
	// byte 1 Rev digit 2 in ASCII
	// byte 2 Rev digit 1 in ASCII
	// byte 3 Rev digit 0 in ASCII

#define GAVIN_HARDWARE_DATE_B		0xfec0001C	// Firmware date - 4b - RO	
	// byte 0 is the year in decimal
	// byte 1 is the month in decimal
	// byte 2 is the day in decimal
	// byte 3 is reserved



// ** A2560K Keyboard Register
// TODO

#define GAVIN_KEYBOARD_INPUT_REG	0xfec00040	// Keyboard input register - 4b - RO	


// ** A2560K Real Time Clock

#define GAVIN_RTC					0xfec00080	// start of RTC registers -- all are 1 byte RW	
#define RTC_SECONDS						(GAVIN_RTC)				//  654: second digit, 3210: 1st digit
#define RTC_SECONDS_ALARM				(RTC_SECONDS + 1)		//  654: second digit, 3210: 1st digit
#define RTC_MINUTES						(RTC_SECONDS_ALARM + 1)	//  654: second digit, 3210: 1st digit
#define RTC_MINUTES_ALARM				(RTC_MINUTES + 1)		//  654: second digit, 3210: 1st digit
#define RTC_HOURS						(RTC_MINUTES_ALARM + 1)	//   54: second digit, 3210: 1st digit
#define RTC_HOURS_ALARM					(RTC_HOURS + 1)			//   54: second digit, 3210: 1st digit
#define RTC_DAY							(RTC_HOURS_ALARM + 1)	//   54: second digit, 3210: 1st digit
#define RTC_DAY_ALARM					(RTC_DAY + 1)			//   54: second digit, 3210: 1st digit
#define RTC_DAY_OF_WEEK					(RTC_DAY_ALARM + 1)		//  210: day of week digit
#define RTC_MONTH						(RTC_DAY_OF_WEEK + 1)	//    4: second digit, 3210: 1st digit
#define RTC_YEAR						(RTC_MONTH + 1)			// 7654: second digit, 3210: 1st digit
#define RTC_RATES						(RTC_YEAR + 1)			//  654: WD (watchdog, not really relevant to F256); 3210: RS
	#define FLAG_RTC_RATE_NONE			0b00000000		// applies to bits 3210 of RTC_RATES
	#define FLAG_RTC_RATE_31NS			0b00000001		// applies to bits 3210 of RTC_RATES. See manual for values between 0001 and 1101
	#define FLAG_RTC_RATE_125MS			0b00001101		// applies to bits 3210 of RTC_RATES
	#define FLAG_RTC_RATE_63MS			0b00001100		// applies to bits 3210 of RTC_RATES - 62.5ms
	#define FLAG_RTC_RATE_250MS			0b00001110		// applies to bits 3210 of RTC_RATES
	#define FLAG_RTC_RATE_500MS			0b00001111		// applies to bits 3210 of RTC_RATES 
#define RTC_ENABLES						(RTC_RATES + 1)			// Controls various interrupt enables, only some of which apply to an F256
	#define FLAG_RTC_PERIODIC_INT_EN	0b00000100		// set PIE (bit 2) to raise interrupt based on RTC_RATES
	#define FLAG_RTC_ALARM_INT_EN		0b00001000		// Set AEI (bit 3) to raise interrupt based on RTC_SECONDS_ALARM, etc. 
#define RTC_FLAGS						(RTC_ENABLES + 1)		// check to see why an RTC interrupt was raised
	#define FLAG_RTC_PERIODIC_INT		0b00000100		// will be set if interrupt was raised based on RTC_RATES
	#define FLAG_RTC_ALARM_INT			0b00001000		// will be set if interrupt was raised based on alarm clock
#define RTC_CONTROL						(RTC_FLAGS + 1)			// set UTI (bit 3) to disable update of reg, to read secs. 
	#define MASK_RTC_CTRL_DSE			0b00000001		// if set (1), daylight savings is in effect.
	#define MASK_RTC_CTRL_12_24			0b00000010		// sets whether the RTC is using 12 or 24 hour accounting (1 = 24 Hr, 0 = 12 Hr)
	#define MASK_RTC_CTRL_STOP			0b00000100		// If it is clear (0) before the system is powered down, it will avoid draining the battery and may stop tracking the time. If it is set (1), it will keep using the battery as long as possible.
	#define MASK_RTC_CTRL_UTI			0b00001000		// if set (1), the update of the externally facing registers by the internal timers is inhibited. In order to read or write those registers, the program must first set UTI and then clear it when done.
	#define MASK_RTC_CTRL_UNUSED		0b11110000		// the upper 4 bits are not used.
#define RTC_CENTURY						(RTC_CONTROL + 1)		// 7654: century 10s digit, 3210: centurys 1s digit


// ** A2560K Interrupt control registers
// TODO

#define GAVIN_INTERRUPT_CONTROL		0xfec00100	// start of interrupt control registers -- all are 2 byte RW	


// ** A2560K Timer control registers
// TODO

#define GAVIN_TIMER_CONTROL			0xfec00200	// start of interrupt control registers -- all are 4 byte RW	


// ** A2560K SD card control registers
// TODO

#define GAVIN_SD_CONTROL			0xfec00300	// start of SD card control registers -- all are 1 byte, mix of RW and RO	


// ** A2560K IDE control registers
// TODO

#define GAVIN_IDE_CONTROL			0xfec00400	// start of IDE drive control registers -- mix of 1-2 bytes, all are RW	


// ** A2560K Joystick control registers
// TODO

#define GAVIN_JOY_CONTROL			0xfec00500	// start of joystick control registers -- all are 2 byte, mix of RW and RO


// ** A2560K Ethernet control registers
// TODO

#define GAVIN_ETHERNET_CONTROL		0xfec00500	// start of Ethernet control registers -- all are 4 byte, mix of RW and RO


// ** A2560K Serial control registers

// common to both serial ports -- from LPC47M107 datasheet
#define UART_BAUD_DIV_300		384	// divisor for 300 baud
#define UART_BAUD_DIV_600		192	// divisor for 600 baud
#define UART_BAUD_DIV_1200		96	// divisor for 1200 baud
#define UART_BAUD_DIV_1800		64		// divisor for 1800 baud
#define UART_BAUD_DIV_2000		58		// divisor for 2000 baud
#define UART_BAUD_DIV_2400		48		// divisor for 2400 baud
#define UART_BAUD_DIV_3600		32		// divisor for 3600 baud
#define UART_BAUD_DIV_4800		24		// divisor for 4800 baud
#define UART_BAUD_DIV_9600		12		// divisor for 9600 baud
#define UART_BAUD_DIV_19200		6		// divisor for 19200 baud
#define UART_BAUD_DIV_38400		3		// divisor for 38400 baud
#define UART_BAUD_DIV_57600		2		// divisor for 57600 baud
#define UART_BAUD_DIV_115200	1		// divisor for 115200 baud

// common to LCR register for both ports
#define UART_DATA_BITS			0b00000011	// 0/0: 5 bit word len. 0/1=6b. 1/0=7b. 1/1=8b.
#define UART_STOP_BITS			0b00000100	// stop bits. clear to have stop bits at 1. 
#define UART_PARITY_ENABLE		0b00001000	// when 1 a parity bit is generated (transmit) or checked (receive)
#define UART_PARITY_EVEN		0b00010000	// even parity: this and b3 are 1s. odd parity: this is 1, b3 is 0.
#define UART_PARITY_STICK		0b00100000	// clear this if doing no parity
#define UART_BRK_SIG			0b01000000	// when 1, TXD is forced to Spacing. don't use.
#define UART_DLAB_MASK			0b10000000	// Divisor Latch Access bit DLAB. set to 1 to access baud rate generator. must be at 0 to access receiver buffer reg, transmitter reg, and IER.

// common to LSR register for both ports
#define UART_DATA_AVAILABLE		0b00000001	// Data Ready (DR). set to 1 whenever an incoming char is available. reset to 0 by reading all of the data in the receive buffer register or the FIFO.
#define UART_OVERRUN_ERROR		0b00000010	// Overrun Error (OE). When set, indicates the receiver buffer register was not read before the next char was transferred. In FIFO mode an overrun error will occur only when the FIFO is full. 
#define UART_PARITY_ERROR		0b00000100	// Parity Error (PE). When set, indicates the received data char does not have the correct even or odd parity.
#define UART_FRAMING_ERROR		0b00001000	// Framing Error (FE). When set, indicates received char did not have a valid stop bit
#define UART_BREAK_INT			0b00010000	// Break Interrupt (BI). see datasheet.
#define UART_THR_IS_EMPTY		0b00100000	// Transmitter Holding Register Empty (THRE). When 1, serial port is ready to accept a new char for transmission. RO.
#define UART_THR_EMPTY_IDLE		0b01000000	// Transmitter Empty (TEMT). When 1, serial both THR and TSR are empty. RO.
#define UART_ERROR				0b10000000	// In non-FIFO mode, will be 0. In FIFO mode, will be 1 when there is any of the following: parity error, framing error, or break indication. Cleared when LSR is read.
#define UART_ERROR_MASK			0b10011110

// MCP has these #s:
// #define UART1_BASE              0xFEC023F8  /* Base address for UART 1 (COM1) */
// #define UART2_BASE              0xFEC022F8  /* Base address for UART 2 (COM2) */
// Manual version 0.0.2 has fec02278 and 0xfec023f8

// serial port 2
// #define GAVIN_SERIAL_2_BASE			0xfec02278	// start of Serial Port 2 registers -- all are 1 byte, RW except as noted
#define GAVIN_SERIAL_2_BASE			0xfec022F8	// start of Serial Port 2 registers -- all are 1 byte, RW except as noted
#define UART2_BASE					(GAVIN_SERIAL_2_BASE)
#define UART2_RBR					(UART2_BASE + 0)	// F256 naming compatibility
#define UART2_RHR					(UART2_BASE + 0)	// When DLAB=0 (RHR) Receiver Holding Register (R)
#define UART2_THR					(UART2_BASE + 0)	// When DLAB=0 (THR) Transmitter Holding Register (W)
#define UART2_DLL					(UART2_BASE + 0)	// When DLAB=1 (DLL) Baud rate Divisor’s Constant LSB
#define UART2_IER					(UART2_BASE + 1)	// When DLAB=0 (IER) Interrupt Enable Register
#define UART2_DLM					(UART2_BASE + 1)	// When DLAB=1 (DLM) Baud rate Divisor’s Constant MSB
#define UART2_IIR					(UART2_BASE + 2)	// F256 naming compatibility
#define UART2_ISR					(UART2_BASE + 2)	// When DLAB=0 (ISR) Interrupt Status Register (R)
#define UART2_FCR					(UART2_BASE + 2)	// When DLAB=1 (FCR) FIFO Control Register (FIFO is 16 Bytes Deep) (W)
#define UART2_LCR					(UART2_BASE + 3)	// (LCR) Line Control Register
#define UART2_MCR					(UART2_BASE + 4)	// (MCR) Modem Control Register
#define UART2_LSR					(UART2_BASE + 5)	// (LSR) Line Status Register
#define UART2_MSR					(UART2_BASE + 6)	// (MSR) Modem Status Register
#define UART2_SCR					(UART2_BASE + 7)	// F256 naming compatibility
#define UART2_SCR					(UART2_BASE + 7)	// (SPR) Scratch Pad Register

// serial port 1
#define GAVIN_SERIAL_1_BASE			0xfec023F8	// start of Serial Port 1 registers -- all are 1 byte, RW except as noted
#define UART_BASE					(GAVIN_SERIAL_1_BASE)
#define UART_RBR					(UART_BASE + 0)		// F256 naming compatibility
#define UART_RHR					(UART_BASE + 0)		// When DLAB=0 (RHR) Receiver Holding Register (R)
#define UART_THR					(UART_BASE + 0)		// When DLAB=0 (THR) Transmitter Holding Register (W)
#define UART_DLL					(UART_BASE + 0)		// When DLAB=1 (DLL) Baud rate Divisor’s Constant LSB
#define UART_IER					(UART_BASE + 1)		// When DLAB=0 (IER) Interrupt Enable Register
#define UART_DLM					(UART_BASE + 1)		// When DLAB=1 (DLM) Baud rate Divisor’s Constant MSB
#define UART_IIR					(UART_BASE + 2)		// F256 naming compatibility
#define UART_ISR					(UART_BASE + 2)		// When DLAB=0 (ISR) Interrupt Status Register (R)
#define UART_FCR					(UART_BASE + 2)		// When DLAB=1 (FCR) FIFO Control Register (FIFO is 16 Bytes Deep) (W)
#define UART_LCR					(UART_BASE + 3)		// (LCR) Line Control Register
#define UART_MCR					(UART_BASE + 4)		// (MCR) Modem Control Register
#define UART_LSR					(UART_BASE + 5)		// (LSR) Line Status Register
#define UART_MSR					(UART_BASE + 6)		// (MSR) Modem Status Register
#define UART_SCR					(UART_BASE + 7)		// F256 naming compatibility
#define UART_SCR					(UART_BASE + 7)		// (SPR) Scratch Pad Register


// ** A2560K VICKY - COMMON
// NOTE: A2560K has 2 VICKY controls, and 2 outputs. Channel A is text only and higher res. 

// Cursor control flags
#define CURSOR_ONOFF_BITS				0b00000001		//!> bit 0 controls whether cursor is displayed or not
#define CURSOR_FLASH_RATE_BITS			0b00000110		//!> bits 1-2 control rate of cursor flashing (if visible)
#define CURSOR_FLASH_RATE_12S			0b00000010		//!> bits 1 on = 1 blink per 1/2 second
#define CURSOR_FLASH_RATE_14S			0b00000100		//!> bits 2 on = 1 blink per 1/4 second
#define CURSOR_FLASH_RATE_15S			0b00000110		//!> bits 1&2 on = 1 blink per 1/5 second

// VICKY FONT FLAGS - maybe only apply to later machines like F256??
#define VICKY_RES_FON_OVLY			0x10	// 0b00010000 -- if clear(0), only the text foreground color will be displayed when text overlays graphics (all background colors will be completely transparent). If set (1), both foreground and background colors will be displayed, except that background color 0 will be transparent.
#define VICKY_RES_FON_SET			0x20	// 0b00100000 -- if set (1), the text font displayed will be font set 1. If clear (0), the text font displayed will be font set 0.


// ** A2560K VICKY - CHANNEL A
// NOTE: channel A is text only, no graphic modes

#define VICKY_A_BASE_ADDRESS			0xfec40000		// start of VICKY channel A registers -- all are 4 byte, RW (some bits RO)
#define VICKY_A_MASTER_CONTROL			(VICKY_A_BASE_ADDRESS)	// VICKY Channel A master control register
	// VICKY A byte 0 bits (reserved except as noted below)
	#define VICKY_AB_TEXT_MODE_EN_FLAG	0x00000001	// 0b00000001	Enable the Text Mode
	#define VICKY_AB_DISABLE_VID_OUTPUT	0x00000080	// 0b10000000	This will disable the Scanning of the Video information in the 4Meg of Video RAM hence giving 100% bandwidth to the CPU
	// VICKY A byte 1 bits (reserved except as noted below)
	#define VICKY_A_MC_RES_FLAG			0x00000800	// 0b00001000 - 1=1024x768, 0=800x600
	// VICKY A byte 2 bits (reserved except as noted below)
	#define VICKY_A_MC_GAMMA_SRC_FLAG	0x00010000	// 0b00000001	0: respect dip switch. 1: respect bit 1 of this byte
	#define VICKY_A_MC_GAMMA_SW_CHOICE	0x00020000	// 0b00000010	0: gamma off. 1: gamma on (only effective if bit 0 set)
	#define VICKY_A_MC_SYNC_DISABLE		0x00040000	// 0b00000100	0: normal. 1: sync off (put display to sleep)
	// VICKY A byte 3 bits (reserved except as noted below)
	#define VICKY_A_MC_GAMMA_DIP		0x00200000	// 0b00100000 - RO - position of gamma dipswitch
	#define VICKY_A_MC_HIRES_DIP		0x00400000	// 0b01000000 - RO - position of hi-res dispswitch
	#define VICKY_A_MC_CLK_SPD_FLAG		0x00800000	// 0b10000000 - RO - PLL clock speed - 0=40mhz, 1=65mhz

#define VICKY_A_BORDER_CONTROL			(VICKY_A_MASTER_CONTROL + 4)	// VICKY Channel A border control register

#define VICKYA_CURSOR_CTRL_A2560K	(VICKY_A_BASE_ADDRESS + 0x10)	// vicky III channel A cursor control register
#define VICKYA_CURSOR_POS_A2560K	(VICKY_A_BASE_ADDRESS + 0x14)	// vicky III channel A cursor position register (x pos is lower word, y pos is upper word)

// VICKY III - Channel B - Text and Graphics
#define VICKY_BITMAP_MAX_H_RES		800		// VICKY in A2560K supports a max resolution of 800x600 for graphics
#define VICKY_BITMAP_MAX_V_RES		600		// VICKY in A2560K supports a max resolution of 800x600 for graphics
#define GRAPHICS_MODE_MASK			0xFFFFFF00	//!> the mask for the system control register that holds the graphics/bitmap/text/sprite mode bits
#define VIDEO_MODE_MASK				0xFFFF00FF	//!> the mask for the system control register that holds the video mode bits
#define GAMMA_MODE_MASK				0xFF00FFFF	//!> the mask for the system control register that controls gamma (in some way not clear to me)

#define VICKY_B_BASE_ADDRESS		0xfec80000		// start of VICKY channel B registers -- all are 4 byte, RW (some bits RO)
#define VICKY_B_MASTER_CONTROL		(VICKY_B_BASE_ADDRESS)	// vicky III channel B control register

	// VICKY B byte 0 bits (reserved except as noted below)
	#define VICKY_AB_TEXT_MODE_EN_FLAG	0x00000001	// 0b00000001	Enable the Text Mode
	#define VICKY_AB_DISABLE_VID_OUTPUT	0x00000080	// 0b10000000	This will disable the Scanning of the Video information in the 4Meg of Video RAM hence giving 100% bandwidth to the CPU

	#define GRAPHICS_MODE_TEXT		0x01	// 0b00000001	Enable the Text Mode
	#define GRAPHICS_MODE_TEXT_OVER	0x02	// 0b00000010	Enable the Overlay of the text mode on top of Graphic Mode (the Background Color is ignored)
	#define GRAPHICS_MODE_GRAPHICS	0x04	// 0b00000100	Enable the Graphic Mode
	#define GRAPHICS_MODE_EN_BITMAP	0x08	// 0b00001000	Enable the Bitmap Module In Vicky
	#define GRAPHICS_MODE_EN_TILE	0x10	// 0b00010000	Enable the Tile Module in Vicky
	#define GRAPHICS_MODE_EN_SPRITE	0x20	// 0b00100000	Enable the Sprite Module in Vicky
	#define GRAPHICS_MODE_RESERVED	0x40	// 0b01000000	Reserved on A2560K
	#define GRAPHICS_MODE_DIS_VIDEO	0x80	// 0b10000000	This will disable the Scanning of the Video information in the 4Meg of VideoRAM hence giving 100% bandwidth to the CPU


	// VICKY B byte 1 bits
	#define VICKY_B_MC_RES_FLAG_1		0x00000100	// 0b00000001
	#define VICKY_B_MC_RES_FLAG_2		0x00000200	// 0b00000010
												// note: apparently 640x400 is not yet configured on A2560K
												// bits 0-1 @ 0/0: 640x480 @ 60FPS
												// bits 0-1 @ 0/1: 800x600 @ 60FPS
												// bits 0-1 @ 1/1: 640x400 @ 70FPS
	#define VICKY_B_MC_PIX_DBL_FLAG		0x00000400	// 0b00000100 - doubles pixel size in both X and Y directions. 
	#define VICKY_B_MC_RESERVED_1		0x00000800	// 0b00001000 - reserved
	#define VICKY_B_MC_RESERVED_2		0x00001000	// 0b00010000 - reserved
	#define VICKY_B_MC_GAMMA_DIP		0x00002000	// 0b00100000 - RO - position of gamma dipswitch
	#define VICKY_B_MC_HIRES_DIP		0x00004000	// 0b01000000 - RO - position of hi-res dispswitch
	#define VICKY_B_MC_CLK_SPD_FLAG		0x00008000	// 0b10000000 - RO - clock speed - 0=25mhz, 1=40mhz
	// VICKY B byte 2 bits (reserved except as noted below)
	#define VICKY_B_MC_GAMMA_SRC_FLAG	0x00010000	// 0b00000001	0: respect dip switch. 1: respect bit 1 of this byte
	#define VICKY_B_MC_GAMMA_SW_CHOICE	0x00020000	// 0b00000010	0: gamma off. 1: gamma on (only effective if bit 0 set)
	#define VICKY_B_MC_SYNC_DISABLE		0x00040000	// 0b00000100	0: normal. 1: sync off (put display to sleep)
	// VICKY B byte 3 bits are all reserved

#define VICKYB_BORDER_CTRL_A2560K	(VICKY_B_BASE_ADDRESS + 0x04)	// vicky III channel B border control register
#define VICKYB_BORDER_COLOR_A2560K	0xfec80008				// vicky III channel B border color register
#define VICKYB_BACK_COLOR_A2560K	0xfec8000C				// vicky III channel B background color register
#define VICKYB_CURSOR_CTRL_A2560K	(VICKY_B_BASE_ADDRESS + 0x10)	// vicky III channel B cursor control register
#define VICKYB_CURSOR_POS_A2560K	(VICKY_B_BASE_ADDRESS + 0x14)	// vicky III channel B cursor position register
#define VICKYB_BITMAP_L0_CTRL		0xfec80100				// vicky III channel B bitmap layer 0 control register (1=enable, +2=LUT0, +4=LUT1, +8=LUT2
#define VICKYB_MOUSE_GRAPHIC_A2560K	0xfec80400				// vicky III channel B mouse pointer graphic stored here (16x16)
#define VICKYB_MOUSE_CTRL_A2560K	0xfec80c00				// vicky III channel B mouse pointer control register. set to 1 to enable mouse. +2 to do whatever "pointer choice" does.
#define VICKYB_MOUSE_PTR_POS_A2560K	0xfec80c04				// vicky III channel B mouse pointer position (Y pos in upper 16 bits, x in lower)
#define TEXTA_RAM_A2560K			(char*)0xfec60000		// channel A text
#define TEXTA_ATTR_A2560K			(char*)0xfec68000		// channel A attr
#define TEXTA_FORE_LUT_A2560K		(char*)0xfec6c400		// FG_CHAR_LUT_PTR	Text Foreground Look-Up Table
#define TEXTA_BACK_LUT_A2560K		(char*)0xfec6c440		// BG_CHAR_LUT_PTR	Text Background Look-Up Table
#define TEXTB_RAM_A2560K			(char*)0xfeca0000		// channel B text
#define TEXTB_ATTR_A2560K			(char*)0xfeca8000		// channel B attr
#define TEXTB_FORE_LUT_A2560K		(char*)0xfecac400		// FG_CHAR_LUT_PTR	Text Foreground Look-Up Table
#define TEXTB_BACK_LUT_A2560K		(char*)0xfecac440		// BG_CHAR_LUT_PTR	Text Background Look-Up Table
#define FONT_MEMORY_BANKA_A2560K	(char*)0xfec48000		// chan A
#define FONT_MEMORY_BANKB_A2560K	(char*)0xfec88000		// chan B
#define VICKY_IIIB_CLUT0			0xfec82000				// each addition LUT is 400 offset from here
#define VICKY_IIIB_CLUT1			(VICKY_IIIB_CLUT0 + 0x400)	// each addition LUT is 400 offset from here
#define VICKY_IIIB_CLUT2			(VICKY_IIIB_CLUT1 + 0x400)	// each addition LUT is 400 offset from here
#define VICKY_IIIB_CLUT3			(VICKY_IIIB_CLUT2 + 0x400)	// each addition LUT is 400 offset from here
#define VICKY_IIIB_CLUT4			(VICKY_IIIB_CLUT3 + 0x400)	// each addition LUT is 400 offset from here
#define VICKY_IIIB_CLUT5			(VICKY_IIIB_CLUT4 + 0x400)	// each addition LUT is 400 offset from here
#define VICKY_IIIB_CLUT6			(VICKY_IIIB_CLUT5 + 0x400)	// each addition LUT is 400 offset from here
#define VICKY_IIIB_CLUT7			(VICKY_IIIB_CLUT6 + 0x400)	// each addition LUT is 400 offset from here

#define default_start_a2560k_vram	0x00011000	// offset against vicky I think though. add to VICKY_B_BASE_ADDRESS? based on doing peek32 in f68. 
#define BITMAP_CTRL_REG_A2560_0		0xfec80100	//! Bitmap Layer0 Control Register (Foreground Layer)
#define BITMAP_VRAM_ADDR_A2560_0	0xfec80104	//! Bitmap Layer0 VRAM Address Pointer. Offset within the VRAM memory from VICKY’s perspective. VRAM Address begins @ $00:0000 and ends @ $1FFFFF
#define BITMAP_CTRL_REG_A2560_1		0xfec80108	//! Bitmap Layer1 Control Register (Background Layer)
#define BITMAP_VRAM_ADDR_A2560_1	0xfec8010C	//! Bitmap Layer0 VRAM Address Pointer. Offset within the VRAM memory from VICKY’s perspective. VRAM Address begins @ $00:0000 and ends @ $1FFFFF


// ** A2560U and A2560U+
#define VICKY_A2560U				0xb40000				// Vicky II offset/first register
#define VICKY_CURSOR_CTRL_A2560U	(VICKY_A2560U + 0x10)		// vicky II channel A cursor control register
#define VICKY_CURSOR_POS_A2560U		(VICKY_A2560U + 0x14)		// vicky II channel A cursor position register (x pos is lower word, y pos is upper word)
#define TEXT_RAM_A2560U				(char*)0xb60000			// text (A2560U only has one video channel)
#define TEXT_ATTR_A2560U			(char*)0xb68000			// attr (A2560U only has one video channel)
#define TEXT_FORE_LUT_A2560U		(char*)0xb6c400			// FG_CHAR_LUT_PTR	Text Foreground Look-Up Table
#define TEXT_BACK_LUT_A2560U		(char*)0xb6c440			// BG_CHAR_LUT_PTR	Text Background Look-Up Table
#define FONT_MEMORY_BANK_A2560U		(char*)0xb48000			// only 1 channel


// ** VRAM locations per machine

// NOTE: VRAM is currently only available in VRAM Buffer A on A2560K, Buffer B is not implemented (2022/03/21)

#if defined _C256_FMX_
	#define VRAM_START				0x00B00000
	#define VRAM_LEN				0x00200000
#elif defined _A2560U_
	#define VRAM_START				0x00C00000
	#define VRAM_LEN				0x00200000
#elif defined _A2560K_
	#define VRAM_START				0x00800000
	#define VRAM_LEN				0x00400000
#endif

#define VRAM_OFFSET_TO_NEXT_SCREEN	0x75300		// 800x600 - 480,000 -- number of bytes needed to cover maximum screen resolution for one bitmap layer


// subtract 0xfe000000 from the UM map for Vicky (to get the old/morfe addresses)
// size of some areas changed too:
//   channel-A text went 0xc6:0000 -> 0xFEC6:0000, but channel-A color went 0xc6:8000 -> 0xFEC6:4000
//   channel-B text went 0xca:0000-> 0xFECA:0000, channel-B color went 0xca:8000 -> 0xFECA:4000
// btw, one thing to keep in mind is device-mem access granularity -- while in morfe you can do 8-32bit accesses, on the actual hw you will need to adhere to the area access granularity
//  see user manual, the "SIZE" columns

// c256foenix on 2/27:
// I will have to look into that. Suffice to say that Channel A, has 2 Video Modes, 800x600 and 1024x768 with no doubling. Channel B has 6 modes, 640x480@60, 800x600@60 and 640x400@70 and with the equivalent Pixel Doubling, 320x240, 400x300 and 320x200. 
// Now, in the K, for Channel A, to not be confusing (although I might have created what I was trying to avoid) and to not have competing Regiters bit with different function (from Channel B and A), I moved the Resolution selection bit to bit# (Something I have to check), but it is farther down the Control Register.
// the Video mode bit for Channel A (that I call Auxiliary) would be bit 11 of the Control Register
// assign Mstr_Ctrl_Video_Mode_PLL_Aux_o = VICKY_MASTER_REG1_RESYNC_AUX[2][11];
// So, that is the bit that selects either 800x600 or 1024x768 and bit[9:8] are ignored.

//    so: 1024x768 = 128x96
//    800x600 = 100x75
//    640x480 = 80x60
//    640x400 = 80x50
//    400x300 = 50x37.5
//    320x240 = 40x30
//    320x200 = 40x25
//    (these are all maximums, as borders can be configured, which reduces the number of usable rows/cols.)
//
// c256 foenix on 2/27:
// To answer, in the traditional Text Mode (Channel B). When you are double pixel mode, everything is reajusted automatically. The Channel A doesn't have a text doubling mode (anymore). And the text matrix and FONT dimension are all manual (needs to be programmed). This is to allow the usage of different sizes of FONT.

// fonts
// gadget:
// If it's the same as on the C256 line, each character consists of 8 bytes.  Upper left hand corner is the high-bit of byte zero, upper right is the low bit of byte zero, lower left is the high bit of byte 7, lower right is the low bit of byte 7.  The bytes are placed in memory from character zero to character 255, 0..7, 0..7, 0..7, etc.

// pjw:
// Yeah, C256 and A2560U fonts are the same layout. The A2560K is a little different. The mostly-text-mode screen supports an 8x16 font layout, where the structure is essentially the same, but each character is 16 bytes rather than 8.

// beethead:
// The 2nd font was removed when the U line came in since it was not being used.  Atleast on the C256's.

// PJW — 2022/03/06 at 9:10 AM
// The A2560K has two DVI ports: one is channel A (which is a text only channel), and channel B is a text and graphics channel equivalent to the main screen on the A2560U. The boot up image on the K shows up on channel B. Currently, the MCP uses channel A as the main interaction channel, but I think I'm going to change that soon, since some people may have just the one screen. The "CTX Switch" key on the A2560K was originally intended as a way to switch which screen you were using for text input, and I may finally implement that.

#define SYS_TICKS_PER_SEC		60	// per syscalls.h in MCP, "a jiffie is 1/60 of a second."

// C256 FPGA Version. not the machine model.
//C256F_MODEL_MAJOR - $AF:070B
//C256F_MODEL_MINOR - $AF:070C

// machine model numbers - for decoding s_sys_info.model - value read from MACHINE_ID_REGISTER (see above)
#define MACHINE_C256FMX			0x00	///< for s_sys_info.model
#define MACHINE_C256U			0x01	///< for s_sys_info.model
#define MACHINE_F256JR			0x02	///< for s_sys_info.model
#define MACHINE_F256JRE			0x03	///< for s_sys_info.model
#define MACHINE_GENX			0x04	///< for s_sys_info.model
#define MACHINE_C256_UPLUS		0x05	///< for s_sys_info.model
#define MACHINE_UNDEFINED_1		0x06	///< for s_sys_info.model
#define MACHINE_UNDEFINED_2		0x07	///< for s_sys_info.model
#define MACHINE_A2560X			0x08	///< for s_sys_info.model. (GenX 32Bits Side)
#define MACHINE_A2560U_PLUS		0x09	///< for s_sys_info.model. there is no A2560U only in the field
#define MACHINE_A2560M			0x0a	///< for s_sys_info.model. 
#define MACHINE_A2560K			0x0b	///< for s_sys_info.model. "classic" A2560K
#define MACHINE_A2560K40		0x0c	///< for s_sys_info.model
#define MACHINE_A2560K60		0x0d	///< for s_sys_info.model
#define MACHINE_UNDEFINED_3		0x0e	///< for s_sys_info.model
#define MACHINE_UNDEFINED_4		0x0f	///< for s_sys_info.model
#define MACHINE_F256P			0x10	///< for s_sys_info.model
#define MACHINE_F256K2			0x11	///< for s_sys_info.model
#define MACHINE_F256K			0x12	///< for s_sys_info.model
#define MACHINE_F256KE			0x13	///< for s_sys_info.model
#define MACHINE_F256K2E			0x14	///< for s_sys_info.model

#define MACHINE_MODEL_MASK		0x1F		

// GABE_SYS_STAT       = $AFE887 ;
// GABE_SYS_STAT_MID0  = $01     ; Machine ID -- LSB
// GABE_SYS_STAT_MID1  = $02     ; Machine ID -- 
// GABE_SYS_STAT_MID2  = $04     ; Machine ID -- MSB
// Bit 3, Bit 2, Bit 1, Bit 0
// 0000: C256 FMX
// 0001: C256 U 2Meg
// 0010: TBD (Reserved)
// 0011: A2560 Dev
// 0100: C256 GenX
// 0101: C256 U+ 4Meg
// 0110: A2560U+
// 0111: A2560X
// 1001: A2560U
// 1101: A2560K
// ergo, $0F is the mask for the bits we care about (the lower 4). 
//#define MACHINE_MODEL_MASK	0x0F
		


// amiga rawkey codes
#define KEYCODE_CURSOR_LEFT		79
#define KEYCODE_CURSOR_RIGHT	78
#define KEYCODE_CURSOR_UP		76
#define KEYCODE_CURSOR_DOWN		77

#define KEYCODE_RETURN			0x44					// rawkey code
#define KEYCODE_RETURN_UP		KEY_RAW_RETURN & 0x80
#define KEY_RETURN			13	// vanilla key
#define KEY_ESC				27	// vanilla key

#define CHAR_UMLAUT			0xA8	// ¨ char -- will use in place of ellipsis when truncating strings
#define CHAR_ELLIPSIS		CHAR_UMLAUT

#define KEY_BUFFER_SIZE		16	// unlikely anyone would ever want to type 15 chars to select a file

#define ALERT_MAX_MESSAGE_LEN		256	// 255 chars + terminator. seems long, but with formatting chars, not crazy.

#define ALERT_DIALOG_SHOW_AS_ERROR	true	// parameter for General_ShowAlert()
#define ALERT_DIALOG_SHOW_AS_INFO	false	// parameter for General_ShowAlert()

#define ALERT_DIALOG_INCLUDE_CANCEL	true	// parameter for General_ShowAlert()
#define ALERT_DIALOG_NO_CANCEL_BTN	false	// parameter for General_ShowAlert()

#define ALERT_DIALOG_1ST_BUTTON	0	// return value for General_ShowAlert()
#define ALERT_DIALOG_2ND_BUTTON	1	// return value for General_ShowAlert()
#define ALERT_DIALOG_3RD_BUTTON	2	// return value for General_ShowAlert()
// NOTE: can't define "OK" as 1 or 0, because it depends on passing ok, then cancel (eg). Buttons appear to be numbered from RIGHT to LEFT!


// System Default Colors - correspond to the CLUT loaded by sys lib

#define SYS_COLOR_WHITE		244						// = 0xF4
#define SYS_COLOR_GRAY1		(SYS_COLOR_WHITE + 1)
#define SYS_COLOR_GRAY2		(SYS_COLOR_WHITE + 2)
#define SYS_COLOR_GRAY3		(SYS_COLOR_WHITE + 3)
#define SYS_COLOR_GRAY4		(SYS_COLOR_WHITE + 4)
#define SYS_COLOR_GRAY5		(SYS_COLOR_WHITE + 5)
#define SYS_COLOR_GRAY6		(SYS_COLOR_WHITE + 6)
#define SYS_COLOR_GRAY7		(SYS_COLOR_WHITE + 7)
#define SYS_COLOR_GRAY8		(SYS_COLOR_WHITE + 8)
#define SYS_COLOR_GRAY9		(SYS_COLOR_WHITE + 9)
#define SYS_COLOR_GRAY10	(SYS_COLOR_WHITE + 10)
#define SYS_COLOR_BLACK		255

#define SYS_COLOR_RED1		35 // = 0x23
#define SYS_COLOR_RED2		(SYS_COLOR_RED1 + 36*5+1)	// 3rd lightest red
#define SYS_COLOR_RED3		(SYS_COLOR_RED2 + 3)

#define SYS_COLOR_GREEN1	23*8+1
#define SYS_COLOR_GREEN2	(SYS_COLOR_GREEN1 + 2)
#define SYS_COLOR_GREEN3	(SYS_COLOR_GREEN2 + 2)

#define SYS_COLOR_BLUE1		26*8+3	// = 211 = 0xd3
#define SYS_COLOR_BLUE2		(SYS_COLOR_BLUE1 + 2)
#define SYS_COLOR_BLUE3		(SYS_COLOR_BLUE2 + 2)

#define SYS_COLOR_PURPLEBLUE		(15*8+7)	// purplish blue = 127 = 0x7F
#define SYS_COLOR_PURPLEBLUEINACT	SYS_COLOR_GRAY3	// light gray inactive accent color for purplish blue = 85 = 0x55
#define SYS_COLOR_PURPLEBLUEHL		(6*7+1)	// light active accent color for purplish blue = 37 = 0x2B
#define SYS_COLOR_TETRA_1			(14 * 4 + 0) // wine color (CC6699) that is tetradic for SYS_COLOR_PURPLEBLUE = 56 = 0x38
#define SYS_COLOR_TETRA_2			(11 * 4 + 1) // yellowish (CCCC66) color that is tetradic for SYS_COLOR_PURPLEBLUE = 44 = 0x2C
#define SYS_COLOR_TETRA_3			(29 * 4 + 0) // teal/greenish color (66CC99) that is tetradic for SYS_COLOR_PURPLEBLUE = 116 = 0x74

#define SYS_DEF_COLOR_WINFRAME			SYS_COLOR_BLACK
#define SYS_DEF_COLOR_WINTITLE_BACK		SYS_COLOR_PURPLEBLUE
#define SYS_DEF_COLOR_WINTITLE_TEXT		SYS_COLOR_WHITE
#define SYS_DEF_COLOR_ICONBAR_BACK		SYS_COLOR_PURPLEBLUE
#define SYS_DEF_COLOR_CONTENT_BACK		SYS_COLOR_WHITE
#define SYS_DEF_COLOR_BUTTON_BACK		SYS_COLOR_GRAY7
#define SYS_DEF_COLOR_BUTTON_PUSH		SYS_COLOR_PURPLEBLUE
#define SYS_DEF_COLOR_BUTTON_TEXT		SYS_COLOR_WHITE
#define SYS_DEF_COLOR_BUTTON_TEXT_DIS	SYS_COLOR_GRAY3
#define SYS_DEF_COLOR_DESKTOP			SYS_COLOR_GRAY4



typedef uint8_t	ColorIdx;


// "character" definitions for each key. Might be how it gets rendered, but can also just be an ID#

#define CH_ALT_OFFSET		96	// this is amount that will be added to key press if ALT is held down.
// #define CH_LSHIFT     	0x00	
// #define CH_RSHIFT     	0x01	
// #define CH_LCTRL     	0x02	
// #define CH_RCTRL     	0x03	
// #define CH_LALT     	0x04	
// #define CH_RALT     	0x05	
// #define CH_LMETA     	0x06	
// #define CH_RMETA     	0x07	
// #define CH_CAPS     	0x08	
// 
// #define CH_CURS_UP      0x10
// #define CH_CURS_DOWN    0x0e
// #define CH_CURS_LEFT    0x02
// #define CH_CURS_RIGHT   0x06
// #define CH_DEL          0x04
#define CH_LF    	    10
#define CH_FF			12
#define CH_ENTER        13
#define CH_ESC          27
#define CH_TAB          9
#define CH_COPYRIGHT	215
#define CH_BKSP   		0x08	
#define CH_DEL   		0x7F	// 127	
#define CH_MENU			0xFF	// no particular reason for selecting FF.

#define CH_K0      		'0'	
#define CH_K1      		'1'	
#define CH_K2      		'2'		
#define CH_K3      		'3'		
#define CH_K4      		'4'		
#define CH_K5      		'5'		
#define CH_K6     		'6'		
#define CH_K7  			'7'		
#define CH_K8  			'8'		
#define CH_K9    		'9'		
#define CH_KENTER   	(CH_ENTER)	
#define CH_KPLUS    	'+'	
#define CH_KMINUS    	'-'	
#define CH_KTIMES   	'*'	
#define CH_KDIV   		'/'	
#define CH_KPOINT  		'.'	
// #define CH_NUM    		0x81	
// #define CH_SCROLL    	0x81	
// #define CH_SYSREQ    	0x81	
// #define CH_HOME    		0x01	
// #define CH_END    		0x05
// #define CH_INS			KEY_INS
// #define CH_PGUP    		0x1A	
// #define CH_PGDN    		0x16	


/*****************************************************************************/
/*                      Standard Foenix Character Points                     */
/*****************************************************************************/

// Standard alpha-numeric characters
#define CH_SPACE			32
#define CH_BANG				33
#define CH_DQUOTE			34
#define CH_HASH				35
#define CH_DOLLAR			36
#define CH_PERCENT			37
#define CH_AMP				38
#define CH_SQUOTE			39
#define CH_LPAREN			40
#define CH_RPAREN			41
#define CH_AST				42
#define CH_PLUS				43
#define CH_COMMA			44
#define CH_MINUS			45
#define CH_PERIOD			46
#define CH_FSLASH			47
#define CH_0				48
#define CH_1				49
#define CH_2				50
#define CH_3				51
#define CH_4				52
#define CH_5				53
#define CH_6				54
#define CH_7				55
#define CH_8				56
#define CH_9				57
#define CH_COLON			58
#define CH_SEMIC			59
#define CH_LESS				60
#define CH_EQUAL			61
#define CH_GREATER			62
#define CH_QUESTION			63
#define CH_AT				64
#define CH_UC_A				65
#define CH_UC_B				(CH_UC_A + 1)
#define CH_UC_C				(CH_UC_B + 1)
#define CH_UC_D				(CH_UC_C + 1)
#define CH_UC_E				(CH_UC_D + 1)
#define CH_UC_F				(CH_UC_E + 1)
#define CH_UC_G				(CH_UC_F + 1)
#define CH_UC_H				(CH_UC_G + 1)
#define CH_UC_I				(CH_UC_H + 1)
#define CH_UC_J				(CH_UC_I + 1)
#define CH_UC_K				(CH_UC_J + 1)
#define CH_UC_L				(CH_UC_K + 1)
#define CH_UC_M				(CH_UC_L + 1)
#define CH_UC_N				(CH_UC_M + 1)
#define CH_UC_O				(CH_UC_N + 1)
#define CH_UC_P				(CH_UC_O + 1)
#define CH_UC_Q				(CH_UC_P + 1)
#define CH_UC_R				(CH_UC_Q + 1)
#define CH_UC_S				(CH_UC_R + 1)
#define CH_UC_T				(CH_UC_S + 1)
#define CH_UC_U				(CH_UC_T + 1)
#define CH_UC_V				(CH_UC_U + 1)
#define CH_UC_W				(CH_UC_V + 1)
#define CH_UC_X				(CH_UC_W + 1)
#define CH_UC_Y				(CH_UC_X + 1)
#define CH_UC_Z				(CH_UC_Y + 1)
#define CH_LBRACKET			91
#define CH_BSLASH			92
#define CH_RBRACKET			93
#define CH_CARET			94
#define CH_UNDER			95
#define CH_LSQUOTE			96
#define CH_LC_A				(CH_UC_A + 32)
#define CH_LC_B				(CH_LC_A + 1)
#define CH_LC_C				(CH_LC_B + 1)
#define CH_LC_D				(CH_LC_C + 1)
#define CH_LC_E				(CH_LC_D + 1)
#define CH_LC_F				(CH_LC_E + 1)
#define CH_LC_G				(CH_LC_F + 1)
#define CH_LC_H				(CH_LC_G + 1)
#define CH_LC_I				(CH_LC_H + 1)
#define CH_LC_J				(CH_LC_I + 1)
#define CH_LC_K				(CH_LC_J + 1)
#define CH_LC_L				(CH_LC_K + 1)
#define CH_LC_M				(CH_LC_L + 1)
#define CH_LC_N				(CH_LC_M + 1)
#define CH_LC_O				(CH_LC_N + 1)
#define CH_LC_P				(CH_LC_O + 1)
#define CH_LC_Q				(CH_LC_P + 1)
#define CH_LC_R				(CH_LC_Q + 1)
#define CH_LC_S				(CH_LC_R + 1)
#define CH_LC_T				(CH_LC_S + 1)
#define CH_LC_U				(CH_LC_T + 1)
#define CH_LC_V				(CH_LC_U + 1)
#define CH_LC_W				(CH_LC_V + 1)
#define CH_LC_X				(CH_LC_W + 1)
#define CH_LC_Y				(CH_LC_X + 1)
#define CH_LC_Z				(CH_LC_Y + 1)
#define CH_LCBRACKET		123
#define CH_PIPE				124
#define CH_RCBRACKET		125
#define CH_TILDE			126


// FOENSCII graphic characters
#define CH_VFILL_UP_1		95	// underscore
#define CH_VFILL_UP_2		1
#define CH_VFILL_UP_3		2
#define CH_VFILL_UP_4		3
#define CH_VFILL_UP_5		4
#define CH_VFILL_UP_6		5
#define CH_VFILL_UP_7		6
#define CH_VFILL_UP_8		7	// solid inverse space
#define CH_VFILL_DN_8		7	// solid inverse space
#define CH_VFILL_DN_7		8
#define CH_VFILL_DN_6		9
#define CH_VFILL_DN_5		10
#define CH_VFILL_DN_4		11
#define CH_VFILL_DN_3		12
#define CH_VFILL_DN_2		13
#define CH_VFILL_DN_1		14

#define CH_VFILLC_UP_1		192	// same as solid fill up/down, but checkered
#define CH_VFILLC_UP_2		193
#define CH_VFILLC_UP_3		194
#define CH_VFILLC_UP_4		195
#define CH_VFILLC_UP_5		196
#define CH_VFILLC_UP_6		197
#define CH_VFILLC_UP_7		198
#define CH_VFILLC_UP_8		199	// full checkered block
#define CH_VFILLC_DN_8		199	// full checkered block
#define CH_VFILLC_DN_7		200
#define CH_VFILLC_DN_6		201
#define CH_VFILLC_DN_5		202
#define CH_VFILLC_DN_4		203
#define CH_VFILLC_DN_3		204
#define CH_VFILLC_DN_2		205
#define CH_VFILLC_DN_1		206

#define CH_HFILL_UP_1		134
#define CH_HFILL_UP_2		135
#define CH_HFILL_UP_3		136
#define CH_HFILL_UP_4		137
#define CH_HFILL_UP_5		138
#define CH_HFILL_UP_6		139
#define CH_HFILL_UP_7		140
#define CH_HFILL_UP_8		7	// solid inverse space
#define CH_HFILL_DN_8		7	// solid inverse space
#define CH_HFILL_DN_7		141
#define CH_HFILL_DN_6		142
#define CH_HFILL_DN_5		143
#define CH_HFILL_DN_4		144
#define CH_HFILL_DN_3		145
#define CH_HFILL_DN_2		146
#define CH_HFILL_DN_1		147

#define CH_HFILLC_UP_1		207
#define CH_HFILLC_UP_2		208
#define CH_HFILLC_UP_3		209
#define CH_HFILLC_UP_4		210
#define CH_HFILLC_UP_5		211
#define CH_HFILLC_UP_6		212
#define CH_HFILLC_UP_7		213
#define CH_HFILLC_UP_8		199	// full checkered block
#define CH_HFILLC_DN_8		199	// full checkered block
#define CH_HFILLC_DN_7		214
#define CH_HFILLC_DN_6		216
#define CH_HFILLC_DN_5		217
#define CH_HFILLC_DN_4		218
#define CH_HFILLC_DN_3		219
#define CH_HFILLC_DN_2		220
#define CH_HFILLC_DN_1		221

#define CH_HDITH_1			15	// horizontal dither patterns...
#define CH_HDITH_2			16
#define CH_HDITH_3			17
#define CH_HDITH_4			18
#define CH_HDITH_5			19
#define CH_HDITH_6			20
#define CH_HDITH_7			21
#define CH_HDITH_8			22
#define CH_HDITH_9			23
#define CH_HDITH_10			24

#define CH_DITH_L1			16	// full-block dither patterns
#define CH_DITH_L2			18
#define CH_DITH_L3			199
#define CH_DITH_L4			21
#define CH_DITH_L5			23

#define CH_VDITH_1			25	// vertical dither patterns...
#define CH_VDITH_2			26
#define CH_VDITH_3			27
#define CH_VDITH_4			28
#define CH_VDITH_5			29

#define CH_DIAG_R1			186	// diagonal patterns and lines...
#define CH_DIAG_R2			30

#define CH_DIAG_R3			184
#define CH_DIAG_R4			230
#define CH_DIAG_R5			234
#define CH_DIAG_R6			238

#define CH_DIAG_R7			229
#define CH_DIAG_R8			233
#define CH_DIAG_R9			237
#define CH_DIAG_R10			241

#define CH_DIAG_L1			187
#define CH_DIAG_L2			31

#define CH_DIAG_L3			228
#define CH_DIAG_L4			232
#define CH_DIAG_L5			236
#define CH_DIAG_L6			240

#define CH_DIAG_L7			185
#define CH_DIAG_L8			231
#define CH_DIAG_L9			235
#define CH_DIAG_L10			239

#define CH_DIAG_X			159

#define CH_HLINE_UP_1		95	// underscore
#define CH_HLINE_UP_2		148
#define CH_HLINE_UP_3		149
#define CH_HLINE_UP_4		150
#define CH_HLINE_UP_5		151
#define CH_HLINE_UP_6		152
#define CH_HLINE_UP_7		153
#define CH_HLINE_UP_8		14
#define CH_HLINE_DN_8		CH_HLINE_UP_8
#define CH_HLINE_DN_7		CH_HLINE_UP_7
#define CH_HLINE_DN_6		CH_HLINE_UP_6
#define CH_HLINE_DN_5		CH_HLINE_UP_5
#define CH_HLINE_DN_4		CH_HLINE_UP_4
#define CH_HLINE_DN_3		CH_HLINE_UP_3
#define CH_HLINE_DN_2		CH_HLINE_UP_2
#define CH_HLINE_DN_1		CH_HLINE_UP_1

#define CH_VLINE_UP_1		134
#define CH_VLINE_UP_2		133
#define CH_VLINE_UP_3		132
#define CH_VLINE_UP_4		131
#define CH_VLINE_UP_5		130
#define CH_VLINE_UP_6		129
#define CH_VLINE_UP_7		128
#define CH_VLINE_UP_8		147
#define CH_VLINE_DN_8		CH_VLINE_UP_8
#define CH_VLINE_DN_7		CH_VLINE_UP_7
#define CH_VLINE_DN_6		CH_VLINE_UP_6
#define CH_VLINE_DN_5		CH_VLINE_UP_5
#define CH_VLINE_DN_4		CH_VLINE_UP_4
#define CH_VLINE_DN_3		CH_VLINE_UP_3
#define CH_VLINE_DN_2		CH_VLINE_UP_2
#define CH_VLINE_DN_1		CH_VLINE_UP_1

#define CH_LINE_WE			150	// box-drawing lines. read clockwise from west. N=up-facing line, E=right-facing line, etc.
#define CH_LINE_NS			130
#define CH_LINE_NES			154
#define CH_LINE_WES			155
#define CH_LINE_WNES		156
#define CH_LINE_WNE			157
#define CH_LINE_WNS			158
#define CH_LINE_ES			160	// square edge corners
#define CH_LINE_WS			161
#define CH_LINE_NE			162
#define CH_LINE_WN			163
#define CH_LINE_RND_ES		188	// rounded edge corners
#define CH_LINE_RND_WS		189
#define CH_LINE_RND_NE		190
#define CH_LINE_RND_WN		191

#define CH_LINE_BLD_WE		173	// thick box-drawing lines. read clockwise from west. N=up-facing line, E=right-facing line, etc.
#define CH_LINE_BLD_NS		174
#define CH_LINE_BLD_NES		164
#define CH_LINE_BLD_WES		165
#define CH_LINE_BLD_WNES	166
#define CH_LINE_BLD_WNE		167
#define CH_LINE_BLD_WNS		168
#define CH_LINE_BLD_ES		169	// square edge corners
#define CH_LINE_BLD_WS		170
#define CH_LINE_BLD_NE		171
#define CH_LINE_BLD_WN		172
#define CH_LINE_BLD_RND_ES	175	// rounded edge corners
#define CH_LINE_BLD_RND_WS	176
#define CH_LINE_BLD_RND_NE	177
#define CH_LINE_BLD_RND_WN	178

#define CH_BLOCK_N			11	// half-width/height block characters. read clockwise from west. N=upper, E=right-side, etc.
#define CH_BLOCK_S			3
#define CH_BLOCK_W			137
#define CH_BLOCK_E			144
#define CH_BLOCK_SE			242
#define CH_BLOCK_SW			243
#define CH_BLOCK_NE			244
#define CH_BLOCK_NW			245
#define CH_BLOCK_NWSE		246
#define CH_BLOCK_SWNE		247

#define CH_MISC_GBP			0
#define CH_MISC_VTILDE		127
#define CH_MISC_COPY		215
#define CH_MISC_FOENIX		223
#define CH_MISC_CHECKMARK	222
#define CH_MISC_HEART		252
#define CH_MISC_DIA			253
#define CH_MISC_SPADE		254
#define CH_MISC_CLUB		255

#define CH_ARROW_DN			248
#define CH_ARROW_LEFT		249
#define CH_ARROW_RIGHT		250
#define CH_ARROW_UP			251
// circular shapes, from larger to smaller
#define CH_CIRCLE_1			180	// full-size filled circle
#define CH_CIRCLE_2			225	// full-size selected radio button circle
#define CH_CIRCLE_3			179	// full-size empty circle
#define CH_CIRCLE_4			226 // medium-size filled circle
#define CH_CIRCLE_5			182 // small circle (square tho)
#define CH_CIRCLE_6			183 // tiny circle (1 pixel)
// square shapes, from larger to smaller
#define CH_RECT_1			7	// full-size filled square
#define CH_RECT_2			227	// full-size empty square
#define CH_RECT_3			181 // almost-full size filled square
#define CH_RECT_4			224 // medium-size filled square
#define CH_RECT_5			182 // small square
#define CH_RECT_6			183 // tiny square (1 pixel)


// Japanese JIS characters plus 6 custom from PC-8001
#define CH_JA_HOUR			24
#define CH_JA_MIN			25
#define CH_JA_SEC			26
#define CH_JA_YEAR			28
#define CH_JA_MONTH			29
#define CH_JA_DAY			192

#define CH_JIS_FIRST		193
#define CH_JIS_KUTEN		193
#define CH_JIS_OPEN			194
#define CH_JIS_CLOSE		195
#define CH_JIS_DOKUTEN		196
#define CH_JIS_MID			197
#define CH_JIS_WO			198
#define CH_JIS_L_A			199
#define CH_JIS_L_I			200
#define CH_JIS_L_U			201
#define CH_JIS_L_E			202
#define CH_JIS_L_O			203
#define CH_JIS_L_YA			204
#define CH_JIS_L_YU			205
#define CH_JIS_L_YO			206
#define CH_JIS_L_TU			207
#define CH_JIS_BOU			208
#define CH_JIS_A			209
#define CH_JIS_I			210
#define CH_JIS_U			211
#define CH_JIS_E			212
#define CH_JIS_O			213
#define CH_JIS_KA			214
#define CH_JIS_KI			215
#define CH_JIS_KU			216
#define CH_JIS_KE			217
#define CH_JIS_KO			218
#define CH_JIS_SA			219
#define CH_JIS_SHI			220
#define CH_JIS_SU			221
#define CH_JIS_SE			222
#define CH_JIS_SO			223
#define CH_JIS_TA			224
#define CH_JIS_TI			225
#define CH_JIS_TSU			226
#define CH_JIS_TE			227
#define CH_JIS_TO			228
#define CH_JIS_NA			229
#define CH_JIS_NI			230
#define CH_JIS_NU			231
#define CH_JIS_NE			232
#define CH_JIS_NO			233
#define CH_JIS_HA			234
#define CH_JIS_HI			235
#define CH_JIS_HU			236
#define CH_JIS_HE			237
#define CH_JIS_HO			238
#define CH_JIS_MA			239
#define CH_JIS_MI			240
#define CH_JIS_MU			241
#define CH_JIS_ME			242
#define CH_JIS_MO			243
#define CH_JIS_YA			244
#define CH_JIS_YU			245
#define CH_JIS_YO			246
#define CH_JIS_RA			247
#define CH_JIS_RI			248
#define CH_JIS_RU			249
#define CH_JIS_RE			250
#define CH_JIS_RO			251
#define CH_JIS_WA			252
#define CH_JIS_N			253
#define CH_JIS_B			254
#define CH_JIS_P			255
#define CH_JIS_LAST			255


/*****************************************************************************/
/*                      Named Characters for Box Drawing                     */
/*****************************************************************************/

#define SC_HLINE        150
#define SC_VLINE        130
#define SC_ULCORNER     160
#define SC_URCORNER     161
#define SC_LLCORNER     162
#define SC_LRCORNER     163
#define SC_ULCORNER_RND 188
#define SC_URCORNER_RND 189
#define SC_LLCORNER_RND 190
#define SC_LRCORNER_RND 191
#define SC_CHECKERED	199
#define SC_T_DOWN		155 // T-shape pointing down
#define SC_T_UP			157 // T-shape pointing up
#define SC_T_LEFT		158 // T-shape pointing left
#define SC_T_RIGHT		154 // T-shape pointing right
#define SC_T_JUNCTION	156 // + shape meeting of 4 lines

/*****************************************************************************/
/*                               Enumerations                                */
/*****************************************************************************/

typedef enum
{
	back_layer = 0,
	fore_layer = 1,
} bitmap_layer;

// device-independent resolution flags
typedef enum
{
	RES_640X400 = 0,
	RES_640X480,
	RES_800X600,
	RES_1024X768,
} screen_resolution;

/*****************************************************************************/
/*                                 Structs                                   */
/*****************************************************************************/

// forward declarations
typedef struct Font Font;						// defined in font.h
typedef struct Window Window;					// defined in window.h
typedef struct ClipRect ClipRect;				// defined in window.h
typedef struct NewWinTemplate NewWinTemplate;	// defined in window.h
typedef struct Theme Theme;						// defined in theme.h
typedef struct ControlBackdrop ControlBackdrop;	// defined in theme.h
typedef struct Control Control;					// defined in control.h
typedef struct ControlTemplate ControlTemplate;	// defined in control.h
typedef struct System System;					// defined in lib_sys.h
typedef struct Bitmap Bitmap;					// defined in bitmap.h
typedef struct List List;						// defined in list.h
typedef struct EventRecord EventRecord;			// defined in event.h
typedef struct EventManager EventManager;		// defined in event.h
typedef struct MouseTracker MouseTracker;		// defined in mouse.h
typedef struct MenuItem MenuItem;				// defined in menu.h
typedef struct MenuGroup MenuGroup;				// defined in menu.h
typedef struct Menu Menu;						// defined in menu.h

//typedef enum event_modifiers event_modifiers;	// defined in event.h


// typedef struct Coordinate
// {
//     int16_t x;
//     int16_t y;
// } Coordinate;

typedef struct Rectangle
{
	int16_t		MinX, MinY;
	int16_t		MaxX, MaxY;
} Rectangle;

typedef struct Screen
{
	int16_t			id_;				// 0 for channel A, 1 for channel B. not all foenix's have 2 channels.
	volatile unsigned long*	vicky_;		// VICKY primary register RAM loc. See VICKY_A_BASE_ADDRESS, VICKY_B_BASE_ADDRESS, VICKY_A2560U, etc.
	Rectangle		rect_;				// the x1, y1, > x2, y2 coordinates of the screen, taking into account any borders. 
	int16_t			width_;				// for the current resolution, the max horizontal pixel count 
	int16_t			height_;			// for the current resolution, the max vertical pixel count 
	int16_t			text_cols_vis_;		// accounting for borders, the number of visible columns on screen
	int16_t			text_rows_vis_;		// accounting for borders, the number of visible rows on screen
	int16_t			text_mem_cols_;		// for the current resolution, the total number of columns per row in VRAM. Use for plotting x,y 
	int16_t			text_mem_rows_;		// for the current resolution, the total number of rows per row in VRAM. Use for plotting x,y 
	char*			text_ram_;
	char*			text_attr_ram_;
	char*			text_font_ram_;		// 2K of memory holding font definitions.
	char*			text_color_fore_ram_;	// 64b of memory holding foreground color LUTs for text mode, in BGRA order
	char*			text_color_back_ram_;	// 64b of memory holding background color LUTs for text mode, in BGRA order
	int16_t			text_font_height_;	// in text mode, the height in pixels for the fixed width font. Should be either 8 or 16, depending on which Foenix. used for calculating text fit.
	int16_t			text_font_width_;	// in text mode, the width in pixels for the fixed width font. Unlikely to be other than '8' with Foenix machines. used for calculating text fit.
// 	char			text_temp_buffer_1_[TEXT_COL_COUNT_FOR_PLOTTING_A2560K * TEXT_ROW_COUNT_FOR_PLOTTING_A2560K + 1];	// todo: replace with pointer, and allocate space on resolution switch. general use temp buffer - do NOT use for real storage - any utility function clobber it
// 	char			text_temp_buffer_2_[TEXT_COL_COUNT_FOR_PLOTTING_A2560K * TEXT_ROW_COUNT_FOR_PLOTTING_A2560K + 1];	// todo: replace with pointer, and allocate space on resolution switch. general use temp buffer - do NOT use for real storage - any utility function clobber it
	Bitmap*			bitmap_[2];			//! The foreground (layer0=0) and background (layer1=1) bitmaps associated with this screen, if any. (Text only screens do not have bitmaps available)
} Screen;



/*****************************************************************************/
/*                             Global Variables                              */
/*****************************************************************************/


/*****************************************************************************/
/*                       Public Function Prototypes                          */
/*****************************************************************************/





#endif /* A2560_PLATFORM_H_ */
