/*
 * sys.c
 *
 *  Created on: Mar 22, 2022
 *      Author: micahbly
 */





/*****************************************************************************/
/*                                Includes                                   */
/*****************************************************************************/

// project includes
#include "bitmap.h"
#include "debug.h"
#include "event.h"
#include "font.h"
#include "general.h"
#include "list.h"
#include "menu.h"
#include "sys.h"
#include "theme.h"
#include "window.h"
#include "mcp_code/ps2.h"

// C includes
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// A2560 includes
#include "a2560k.h"
#include <mcp/syscalls.h>
#include <mcp/interrupt.h>


/*****************************************************************************/
/*                               Definitions                                 */
/*****************************************************************************/



/*****************************************************************************/
/*                               Enumerations                                */
/*****************************************************************************/



/*****************************************************************************/
/*                          File-scoped Variables                            */
/*****************************************************************************/

	static uint8_t system_std_font[2048] = {  
0x06,0x09,0x10,0x3C, 0x10,0x21,0x7E,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0xFF,0xFF, 0x00,0x00,0x00,0x00, 0x00,0xFF,0xFF,0xFF, 0x00,0x00,0x00,0x00, 0xFF,0xFF,0xFF,0xFF, 0x00,0x00,0x00,0xFF, 0xFF,0xFF,0xFF,0xFF, 0x00,0x00,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0x00,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0x00, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0x00,0x00, 0xFF,0xFF,0xFF,0xFF, 0xFF,0x00,0x00,0x00, 0xFF,0xFF,0xFF,0xFF, 0x00,0x00,0x00,0x00, 0xFF,0xFF,0xFF,0x00, 0x00,0x00,0x00,0x00, 0xFF,0xFF,0x00,0x00, 0x00,0x00,0x00,0x00, 0xFF,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x08,0x00,0x22,0x00, 0x08,0x00,0x02,0x00, 0x88,0x00,0x22,0x00, 0x88,0x00,0x22,0x00, 0x8A,0x00,0x2A,0x00, 0x8A,0x00,0x2A,0x00, 0xAA,0x00,0xAA,0x00, 0xAA,0x00,0xAA,0x00, 0xAA,0x05,0xAA,0x11, 0xAA,0x05,0xAA,0x11, 0xAA,0x5F,0xAA,0x77, 0xAA,0x5F,0xAA,0x77, 0xAA,0xFF,0xAA,0xFF, 0xAA,0xFF,0xAA,0xFF, 0xAF,0xFF,0xBB,0xFF, 0xAF,0xFF,0xBB,0xFF, 0x77,0xFF,0xDD,0xFF, 0x77,0xFF,0xDD,0xFF, 0x7F,0xFF,0xDF,0xFF, 0x77,0xFF,0xDF,0xFF, 0xFF,0xFF,0xDF,0xFF, 0x77,0xFF,0xDD,0xFF, 0xBB,0xFF,0xEE,0xFF, 0xAA,0xFF,0xAA,0xFF, 0xAA,0xFF,0xAA,0x77, 0xAA,0xDD,0xAA,0x55, 0xAA,0x55,0x22,0x55, 0x88,0x55,0x00,0x55, 0xAA,0x00,0xAA,0x00, 0x88,0x00,0x22,0x00, 0x33,0x99,0xCC,0x66, 0x33,0x99,0xCC,0x66, 0xCC,0x99,0x33,0x66, 0xCC,0x99,0x33,0x66, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x08,0x08,0x08,0x08, 0x00,0x00,0x08,0x00, 0x24,0x24,0x24,0x00, 0x00,0x00,0x00,0x00, 0x24,0x24,0x7E,0x24, 0x7E,0x24,0x24,0x00, 0x08,0x1E,0x28,0x1C, 0x0A,0x3C,0x08,0x00, 0x00,0x62,0x64,0x08, 0x10,0x26,0x46,0x00, 0x30,0x48,0x48,0x30, 0x4A,0x44,0x3A,0x00, 0x08,0x08,0x08,0x00, 0x00,0x00,0x00,0x00, 0x04,0x08,0x10,0x10, 0x10,0x08,0x04,0x00, 0x20,0x10,0x08,0x08, 0x08,0x10,0x20,0x00, 0x00,0x2A,0x1C,0x3E, 0x1C,0x2A,0x00,0x00, 0x00,0x08,0x08,0x3E, 0x08,0x08,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x08,0x08,0x10, 0x00,0x00,0x00,0x7E, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x18,0x18,0x00, 0x00,0x02,0x04,0x08, 0x10,0x20,0x40,0x00, 0x3C,0x42,0x46,0x5A, 0x62,0x42,0x3C,0x00, 0x08,0x18,0x08,0x08, 0x08,0x08,0x1C,0x00, 0x3C,0x42,0x02,0x3C, 0x40,0x40,0x7E,0x00, 0x3C,0x42,0x02,0x1C, 0x02,0x42,0x3C,0x00, 0x04,0x44,0x44,0x44, 0x7E,0x04,0x04,0x00, 0x7E,0x40,0x40,0x7C, 0x02,0x02,0x7C,0x00, 0x3C,0x40,0x40,0x7C, 0x42,0x42,0x3C,0x00, 0x7E,0x42,0x04,0x08, 0x10,0x10,0x10,0x00, 0x3C,0x42,0x42,0x3C, 0x42,0x42,0x3C,0x00, 0x3C,0x42,0x42,0x3E, 0x02,0x02,0x3C,0x00, 0x00,0x00,0x08,0x00, 0x00,0x08,0x00,0x00, 0x00,0x00,0x08,0x00, 0x00,0x08,0x08,0x10, 0x08,0x10,0x20,0x40, 0x20,0x10,0x08,0x00, 0x00,0x00,0x7E,0x00, 0x7E,0x00,0x00,0x00, 0x10,0x08,0x04,0x02, 0x04,0x08,0x10,0x00, 0x3C,0x42,0x02,0x0C, 0x10,0x00,0x10,0x00, 0x3C,0x42,0x4E,0x52, 0x4E,0x40,0x3C,0x00, 0x3C,0x42,0x42,0x7E, 0x42,0x42,0x42,0x00, 0x7C,0x42,0x42,0x7C, 0x42,0x42,0x7C,0x00, 0x3C,0x42,0x40,0x40, 0x40,0x42,0x3C,0x00, 0x7C,0x42,0x42,0x42, 0x42,0x42,0x7C,0x00, 0x7E,0x40,0x40,0x78, 0x40,0x40,0x7E,0x00, 0x7E,0x40,0x40,0x78, 0x40,0x40,0x40,0x00, 0x3C,0x42,0x40,0x4E, 0x42,0x42,0x3C,0x00, 0x42,0x42,0x42,0x7E, 0x42,0x42,0x42,0x00, 0x1C,0x08,0x08,0x08, 0x08,0x08,0x1C,0x00, 0x0E,0x04,0x04,0x04, 0x04,0x44,0x38,0x00, 0x42,0x44,0x48,0x70, 0x48,0x44,0x42,0x00, 0x40,0x40,0x40,0x40, 0x40,0x40,0x7E,0x00, 0x41,0x63,0x55,0x49, 0x41,0x41,0x41,0x00, 0x42,0x62,0x52,0x4A, 0x46,0x42,0x42,0x00, 0x3C,0x42,0x42,0x42, 0x42,0x42,0x3C,0x00, 0x7C,0x42,0x42,0x7C, 0x40,0x40,0x40,0x00, 0x3C,0x42,0x42,0x42, 0x4A,0x44,0x3A,0x00, 0x7C,0x42,0x42,0x7C, 0x48,0x44,0x42,0x00, 0x3C,0x42,0x40,0x3C, 0x02,0x42,0x3C,0x00, 0x3E,0x08,0x08,0x08, 0x08,0x08,0x08,0x00, 0x42,0x42,0x42,0x42, 0x42,0x42,0x3C,0x00, 0x41,0x41,0x41,0x22, 0x22,0x14,0x08,0x00, 0x41,0x41,0x41,0x49, 0x55,0x63,0x41,0x00, 0x42,0x42,0x24,0x18, 0x24,0x42,0x42,0x00, 0x41,0x22,0x14,0x08, 0x08,0x08,0x08,0x00, 0x7F,0x02,0x04,0x08, 0x10,0x20,0x7F,0x00, 0x3C,0x20,0x20,0x20, 0x20,0x20,0x3C,0x00, 0x00,0x40,0x20,0x10, 0x08,0x04,0x02,0x00, 0x3C,0x04,0x04,0x04, 0x04,0x04,0x3C,0x00, 0x00,0x08,0x14,0x22, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0xFF, 0x10,0x08,0x04,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x3C,0x02, 0x3E,0x42,0x3E,0x00, 0x40,0x40,0x7C,0x42, 0x42,0x42,0x7C,0x00, 0x00,0x00,0x3C,0x40, 0x40,0x40,0x3C,0x00, 0x02,0x02,0x3E,0x42, 0x42,0x42,0x3E,0x00, 0x00,0x00,0x3C,0x42, 0x7E,0x40,0x3C,0x00, 0x0C,0x10,0x10,0x7C, 0x10,0x10,0x10,0x00, 0x00,0x00,0x3E,0x42, 0x42,0x3E,0x02,0x3C, 0x40,0x40,0x7C,0x42, 0x42,0x42,0x42,0x00, 0x08,0x00,0x18,0x08, 0x08,0x08,0x08,0x00, 0x04,0x00,0x0C,0x04, 0x04,0x04,0x04,0x38, 0x40,0x40,0x44,0x48, 0x50,0x68,0x44,0x00, 0x18,0x08,0x08,0x08, 0x08,0x08,0x1C,0x00, 0x00,0x00,0x76,0x49, 0x49,0x49,0x49,0x00, 0x00,0x00,0x7C,0x42, 0x42,0x42,0x42,0x00, 0x00,0x00,0x3C,0x42, 0x42,0x42,0x3C,0x00, 0x00,0x00,0x7C,0x42, 0x42,0x7C,0x40,0x40, 0x00,0x00,0x3E,0x42, 0x42,0x3E,0x02,0x02, 0x00,0x00,0x5C,0x60, 0x40,0x40,0x40,0x00, 0x00,0x00,0x3E,0x40, 0x3C,0x02,0x7C,0x00, 0x10,0x10,0x7C,0x10, 0x10,0x10,0x0C,0x00, 0x00,0x00,0x42,0x42, 0x42,0x42,0x3E,0x00, 0x00,0x00,0x42,0x42, 0x42,0x24,0x18,0x00, 0x00,0x00,0x41,0x49, 0x49,0x49,0x36,0x00, 0x00,0x00,0x42,0x24, 0x18,0x24,0x42,0x00, 0x00,0x00,0x42,0x42, 0x42,0x3E,0x02,0x3C, 0x00,0x00,0x7E,0x04, 0x18,0x20,0x7E,0x00, 0x0C,0x10,0x10,0x20, 0x10,0x10,0x0C,0x00, 0x10,0x10,0x10,0x10, 0x10,0x10,0x10,0x00, 0x30,0x08,0x08,0x04, 0x08,0x08,0x30,0x00, 0x00,0x00,0x30,0x49, 0x06,0x00,0x00,0x00, 0x08,0x04,0x04,0x08, 0x10,0x10,0x08,0x00, 0x02,0x02,0x02,0x02, 0x02,0x02,0x02,0x02, 0x04,0x04,0x04,0x04, 0x04,0x04,0x04,0x04, 0x08,0x08,0x08,0x08, 0x08,0x08,0x08,0x08, 0x10,0x10,0x10,0x10, 0x10,0x10,0x10,0x10, 0x20,0x20,0x20,0x20, 0x20,0x20,0x20,0x20, 0x40,0x40,0x40,0x40, 0x40,0x40,0x40,0x40, 0x80,0x80,0x80,0x80, 0x80,0x80,0x80,0x80, 0xC0,0xC0,0xC0,0xC0, 0xC0,0xC0,0xC0,0xC0, 0xE0,0xE0,0xE0,0xE0, 0xE0,0xE0,0xE0,0xE0, 0xF0,0xF0,0xF0,0xF0, 0xF0,0xF0,0xF0,0xF0, 0xF8,0xF8,0xF8,0xF8, 0xF8,0xF8,0xF8,0xF8, 0xFC,0xFC,0xFC,0xFC, 0xFC,0xFC,0xFC,0xFC, 0xFE,0xFE,0xFE,0xFE, 0xFE,0xFE,0xFE,0xFE, 0x7F,0x7F,0x7F,0x7F, 0x7F,0x7F,0x7F,0x7F, 0x3F,0x3F,0x3F,0x3F, 0x3F,0x3F,0x3F,0x3F, 0x1F,0x1F,0x1F,0x1F, 0x1F,0x1F,0x1F,0x1F, 0x0F,0x0F,0x0F,0x0F, 0x0F,0x0F,0x0F,0x0F, 0x07,0x07,0x07,0x07, 0x07,0x07,0x07,0x07, 0x03,0x03,0x03,0x03, 0x03,0x03,0x03,0x03, 0x01,0x01,0x01,0x01, 0x01,0x01,0x01,0x01, 0x00,0x00,0x00,0x00, 0x00,0x00,0xFF,0x00, 0x00,0x00,0x00,0x00, 0x00,0xFF,0x00,0x00, 0x00,0x00,0x00,0x00, 0xFF,0x00,0x00,0x00, 0x00,0x00,0x00,0xFF, 0x00,0x00,0x00,0x00, 0x00,0x00,0xFF,0x00, 0x00,0x00,0x00,0x00, 0x00,0xFF,0x00,0x00, 0x00,0x00,0x00,0x00, 0x08,0x08,0x08,0x08, 0x0F,0x08,0x08,0x08, 0x00,0x00,0x00,0x00, 0xFF,0x08,0x08,0x08, 0x08,0x08,0x08,0x08, 0xFF,0x08,0x08,0x08, 0x08,0x08,0x08,0x08, 0xFF,0x00,0x00,0x00, 0x08,0x08,0x08,0x08, 0xF8,0x08,0x08,0x08, 0x81,0x42,0x24,0x18, 0x18,0x24,0x42,0x81, 0x00,0x00,0x00,0x00, 0x0F,0x08,0x08,0x08, 0x00,0x00,0x00,0x00, 0xF8,0x08,0x08,0x08, 0x08,0x08,0x08,0x08, 0x0F,0x00,0x00,0x00, 0x08,0x08,0x08,0x08, 0xF8,0x00,0x00,0x00, 0x18,0x18,0x18,0x1F, 0x1F,0x18,0x18,0x18, 0x00,0x00,0x00,0xFF, 0xFF,0x18,0x18,0x18, 0x18,0x18,0x18,0xFF, 0xFF,0x18,0x18,0x18, 0x18,0x18,0x18,0xFF, 0xFF,0x00,0x00,0x00, 0x18,0x18,0x18,0xF8, 0xF8,0x18,0x18,0x18, 0x00,0x00,0x00,0x1F, 0x1F,0x18,0x18,0x18, 0x00,0x00,0x00,0xF8, 0xF8,0x18,0x18,0x18, 0x18,0x18,0x18,0x1F, 0x1F,0x00,0x00,0x00, 0x18,0x18,0x18,0xF8, 0xF8,0x00,0x00,0x00, 0x00,0x00,0x00,0xFF, 0xFF,0x00,0x00,0x00, 0x18,0x18,0x18,0x18, 0x18,0x18,0x18,0x18, 0x00,0x00,0x00,0x00, 0x03,0x07,0x0F,0x0F, 0x00,0x00,0x00,0x00, 0xC0,0xE0,0xF0,0xF0, 0x0F,0x0F,0x07,0x03, 0x00,0x00,0x00,0x00, 0xF0,0xF0,0xE0,0xC0, 0x00,0x00,0x00,0x00, 0x00,0x3C,0x42,0x42, 0x42,0x42,0x3C,0x00, 0x00,0x3C,0x7E,0x7E, 0x7E,0x7E,0x3C,0x00, 0x00,0x7E,0x7E,0x7E, 0x7E,0x7E,0x7E,0x00, 0x00,0x00,0x00,0x18, 0x18,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x08,0x00,0x00,0x00, 0xFF,0x7F,0x3F,0x1F, 0x0F,0x07,0x03,0x01, 0xFF,0xFE,0xFC,0xF8, 0xF0,0xE0,0xC0,0x80, 0x80,0x40,0x20,0x10, 0x08,0x04,0x02,0x01, 0x01,0x02,0x04,0x08, 0x10,0x20,0x40,0x80, 0x00,0x00,0x00,0x00, 0x03,0x04,0x08,0x08, 0x00,0x00,0x00,0x00, 0xE0,0x10,0x08,0x08, 0x08,0x08,0x08,0x04, 0x03,0x00,0x00,0x00, 0x08,0x08,0x08,0x10, 0xE0,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x55, 0x00,0x00,0x00,0x00, 0x00,0x00,0xAA,0x55, 0x00,0x00,0x00,0x00, 0x00,0x55,0xAA,0x55, 0x00,0x00,0x00,0x00, 0xAA,0x55,0xAA,0x55, 0x00,0x00,0x00,0x55, 0xAA,0x55,0xAA,0x55, 0x00,0x00,0xAA,0x55, 0xAA,0x55,0xAA,0x55, 0x00,0x55,0xAA,0x55, 0xAA,0x55,0xAA,0x55, 0xAA,0x55,0xAA,0x55, 0xAA,0x55,0xAA,0x55, 0xAA,0x55,0xAA,0x55, 0xAA,0x55,0xAA,0x00, 0xAA,0x55,0xAA,0x55, 0xAA,0x55,0x00,0x00, 0xAA,0x55,0xAA,0x55, 0xAA,0x00,0x00,0x00, 0xAA,0x55,0xAA,0x55, 0x00,0x00,0x00,0x00, 0xAA,0x55,0xAA,0x00, 0x00,0x00,0x00,0x00, 0xAA,0x55,0x00,0x00, 0x00,0x00,0x00,0x00, 0xAA,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x80,0x00,0x80,0x00, 0x80,0x00,0x80,0x00, 0x80,0x40,0x80,0x40, 0x80,0x40,0x80,0x40, 0xA0,0x40,0xA0,0x40, 0xA0,0x40,0xA0,0x40, 0xA0,0x50,0xA0,0x50, 0xA0,0x50,0xA0,0x50, 0xA8,0x50,0xA8,0x50, 0xA8,0x50,0xA8,0x50, 0xA8,0x54,0xA8,0x54, 0xA8,0x54,0xA8,0x54, 0xAA,0x54,0xAA,0x54, 0xAA,0x54,0xAA,0x54, 0x2A,0x55,0x2A,0x55, 0x2A,0x55,0x2A,0x55, 0x7E,0x81,0x9D,0xA1, 0xA1,0x9D,0x81,0x7E, 0x2A,0x15,0x2A,0x15, 0x2A,0x15,0x2A,0x15, 0x0A,0x15,0x0A,0x15, 0x0A,0x15,0x0A,0x15, 0x0A,0x05,0x0A,0x05, 0x0A,0x05,0x0A,0x05, 0x02,0x05,0x02,0x05, 0x02,0x05,0x02,0x05, 0x02,0x01,0x02,0x01, 0x02,0x01,0x02,0x01, 0x00,0x01,0x00,0x01, 0x00,0x01,0x00,0x01, 0x00,0x00,0x03,0x06, 0x6C,0x38,0x10,0x00, 0x7E,0x81,0xBD,0xA1, 0xB9,0xA1,0xA1,0x7E, 0x00,0x00,0x3C,0x3C, 0x3C,0x3C,0x00,0x00, 0x00,0x3C,0x42,0x5A, 0x5A,0x42,0x3C,0x00, 0x00,0x00,0x18,0x3C, 0x3C,0x18,0x00,0x00, 0xFF,0x81,0x81,0x81, 0x81,0x81,0x81,0xFF, 0x01,0x03,0x07,0x0F, 0x1F,0x3F,0x7F,0xFF, 0x80,0xC0,0xE0,0xF0, 0xF8,0xFC,0xFE,0xFF, 0x3F,0x1F,0x0F,0x07, 0x03,0x01,0x00,0x00, 0xFC,0xF8,0xF0,0xE0, 0xC0,0x80,0x00,0x00, 0x00,0x00,0x01,0x03, 0x07,0x0F,0x1F,0x3F, 0x00,0x00,0x80,0xC0, 0xE0,0xF0,0xF8,0xFC, 0x0F,0x07,0x03,0x01, 0x00,0x00,0x00,0x00, 0xF0,0xE0,0xC0,0x80, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x01,0x03,0x07,0x0F, 0x00,0x00,0x00,0x00, 0x80,0xC0,0xE0,0xF0, 0x03,0x01,0x00,0x00, 0x00,0x00,0x00,0x00, 0xC0,0x80,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x01,0x03, 0x00,0x00,0x00,0x00, 0x00,0x00,0x80,0xC0, 0x00,0x00,0x00,0x00, 0x0F,0x0F,0x0F,0x0F, 0x00,0x00,0x00,0x00, 0xF0,0xF0,0xF0,0xF0, 0x0F,0x0F,0x0F,0x0F, 0x00,0x00,0x00,0x00, 0xF0,0xF0,0xF0,0xF0, 0x00,0x00,0x00,0x00, 0xF0,0xF0,0xF0,0xF0, 0x0F,0x0F,0x0F,0x0F, 0x0F,0x0F,0x0F,0x0F, 0xF0,0xF0,0xF0,0xF0, 0x00,0x00,0x00,0x3E, 0x1C,0x08,0x00,0x00, 0x00,0x00,0x08,0x18, 0x38,0x18,0x08,0x00, 0x00,0x00,0x10,0x18, 0x1C,0x18,0x10,0x00, 0x00,0x00,0x08,0x1C, 0x3E,0x00,0x00,0x00, 0x36,0x7F,0x7F,0x7F, 0x3E,0x1C,0x08,0x00, 0x08,0x1C,0x3E,0x7F, 0x3E,0x1C,0x08,0x00, 0x08,0x1C,0x3E,0x7F, 0x7F,0x1C,0x3E,0x00, 0x08,0x1C,0x2A,0x77, 0x2A,0x08,0x1C,0x00,
};

/*****************************************************************************/
/*                             Global Variables                              */
/*****************************************************************************/

extern System*			global_system;

// MCP / previous interrupt handler functions for restore on exit
p_int_handler	global_old_keyboard_interrupt;
p_int_handler	global_old_mouse_interrupt;
// p_int_handler is defined in mcp/interrupt.h as typedef void (*p_int_handler)();

// VGA colors, used for both fore- and background colors in Text mode
// in C256, these are 8 bit values; in A2560s, they are 32 bit values, and endianness matters
static uint32_t standard_text_color_lut_buffer[16] = 
{
	0x00000000,
	0x00AA0000,
	0x0000AA00,
	0x00AA5500,
	0x000000AA,
	0x00AA00AA,
	0x0000AAAA,
	0x00AAAAAA,
	0x00555555,
	0x00FF5555,
	0x0055FF55,
	0x00FFFF55,
	0x005555FF,
	0x00FF55FF,
	0x0055FFFF,
	0x00FFFFFF,			
};
static uint32_t*	standard_text_color_lut = standard_text_color_lut_buffer;

/*****************************************************************************/
/*                       Private Function Prototypes                         */
/*****************************************************************************/


// Instruct all windows to close / clean themselves up
void Sys_DestroyAllWindows(System* the_system);

//! Instruct every window to update itself and its controls to match the system's current theme
//! This is called as part of Sys_SetTheme().
void Sys_UpdateWindowTheme(System* the_system);

void Sys_RenumberWindows(System* the_system);

//! Event handler for the backdrop window
void Window_BackdropWinEventHandler(EventRecord* the_event);

// enable or disable the gamma correction 
bool Sys_SetGammaMode(System* the_system, Screen* the_screen, bool enable_it);

//! Find out what kind of machine the software is running on, and determine # of screens available
//! @param	the_system -- valid pointer to system object
//! @return	Returns false if the machine is known to be incompatible with this software. 
bool Sys_AutoDetectMachine(System* the_system);

//! Find out what kind of machine the software is running on, and configure the passed screen accordingly
//! Configures screen settings, RAM addresses, etc. based on known info about machine types
//! Configures screen width, height, total text rows and cols, and visible text rows and cols by checking hardware
//! @param	the_system -- valid pointer to system object
//! @return	Returns false if the machine is known to be incompatible with this software. 
bool Sys_AutoConfigure(System* the_system);

//! Detect the current screen mode/resolution, and set # of columns, rows, H pixels, V pixels, accordingly
//! @param	the_screen -- valid pointer to the target screen to operate on
//! @return	returns false on any error/invalid input.
bool Sys_DetectScreenSize(Screen* the_screen);

//! Change the cursor character to the passed character
//! @param	the_char -- the character point to be set as the cursor graphic
//! FUTURE: pass and set foreground and background cursor color; pass and set blink speed.
void Sys_ChangeCursor(uint8_t the_char);

// Interrupt handler for PS/2 keyboard
void Sys_InterruptKeyboard(void);

// Interrupt handler for PS/2 mouse
void Sys_InterruptMouse(void);




/*****************************************************************************/
/*                       Private Function Definitions                        */
/*****************************************************************************/




//! Instruct all windows to close / clean themselves up
//! @param	the_system -- valid pointer to system object
void Sys_DestroyAllWindows(System* the_system)
{
	int16_t		num_nodes = 0;
	List*		the_item;
	
 	if (the_system == NULL)
 	{
		LOG_ERR(("%s %d: passed class object was null", __func__ , __LINE__));
		goto error;
 	}
	
	if (the_system->list_windows_ == NULL)
	{
		DEBUG_OUT(("%s %d: the window list was NULL", __func__ , __LINE__));
		goto error;
	}
	
	the_item = *(the_system->list_windows_);

	while (the_item != NULL)
	{
		Window*		this_window = (Window*)(the_item->payload_);
		
		Window_Destroy(&this_window);
		++num_nodes;
		--the_system->window_count_;

		the_item = the_item->next_item_;
	}

	// now free up the list items themselves
	List_Destroy(the_system->list_windows_);

	DEBUG_OUT(("%s %d: %i windows closed", __func__ , __LINE__, num_nodes));
	
	return;
	
error:
	Sys_Exit(&global_system, PARAM_EXIT_ON_ERROR);	// crash early, crash often
	return;
}


//! Instruct every window to update itself and its controls to match the system's current theme
//! This is called as part of Sys_SetTheme().
void Sys_UpdateWindowTheme(System* the_system)
{
 	List*	the_item;
 	
 	if (the_system == NULL)
 	{
		LOG_ERR(("%s %d: passed class object was null", __func__ , __LINE__));
		goto error;
 	}
	
	the_item = *(the_system->list_windows_);

	while (the_item != NULL)
	{
		Window*		this_window = (Window*)(the_item->payload_);
		
		Window_UpdateTheme(this_window);
		
		the_item = the_item->next_item_;
	}
	
	return;
	
error:
	Sys_Exit(&global_system, PARAM_EXIT_ON_ERROR);	// crash early, crash often
	return;
}


void Sys_RenumberWindows(System* the_system)
{
 	List*	the_item;
 	int8_t	win_num = 1;
 	int8_t	this_display_order;
 	
 	if (the_system == NULL)
 	{
		LOG_ERR(("%s %d: passed class object was null", __func__ , __LINE__));
		return;
 	}
	
// 	DEBUG_OUT(("%s %d: win count=%i", __func__ , __LINE__, the_system->window_count_));
// 	List_Print(the_system->list_windows_, (void*)&Window_PrintBrief);
	
	the_item = *(the_system->list_windows_);

	while (the_item != NULL)
	{
		Window*		this_window = (Window*)(the_item->payload_);
		
		if (this_window == NULL)
		{
			LOG_ERR(("%s %d: this_window was null, the_item=%p, win_num=%i, win count=%i", __func__ , __LINE__, the_item, win_num, the_system->window_count_));
			goto error;
		}
		
		if (this_window->is_backdrop_)
		{
			this_display_order = SYS_WIN_Z_ORDER_BACKDROP;
		}
		else
		{
			this_display_order = SYS_MAX_WINDOWS - win_num++;
		}
		
		Window_SetDisplayOrder(this_window, this_display_order);
		
		the_item = the_item->next_item_;
	}
	
	return;
	
error:
	Sys_Exit(&global_system, PARAM_EXIT_ON_ERROR);	// crash early, crash often
	return;
}


//! Event handler for the backdrop window
void Window_BackdropWinEventHandler(EventRecord* the_event)
{
	return;
}


//! Find out what kind of machine the software is running on, and determine # of screens available
//! @param	the_system -- valid pointer to system object
//! @return	Returns false if the machine is known to be incompatible with this software. 
bool Sys_AutoDetectMachine(System* the_system)
{
	struct s_sys_info	sys_info;
	struct s_sys_info*	the_sys_info = &sys_info; // doing this convoluted thing so that C256 macro can fake having sys_get_info

	// MB: this is another way to get it, without depending on MCP:
	// 	uint8_t	the_machine_id;	
	// 	the_machine_id = (R8(MACHINE_ID_REGISTER) & MACHINE_MODEL_MASK);	

	sys_get_info(the_sys_info);
	
	the_system->model_number_ = the_sys_info->model;
	DEBUG_OUT(("%s %d: the_system->model_number_=%u", __func__, __LINE__, the_system->model_number_));
	
	// temp until Calypsi fix for switch on 65816
	if (the_system->model_number_ == MACHINE_A2560M || the_system->model_number_ == MACHINE_A2560U_PLUS)
	{
			DEBUG_OUT(("%s %d: I think this is a A2560U or A2560U+...", __func__, __LINE__));
			the_system->num_screens_ = 1;
	}
	else if (the_system->model_number_ == MACHINE_A2560X || 
			the_system->model_number_ == MACHINE_A2560K ||
			the_system->model_number_ == MACHINE_A2560K40 ||
			the_system->model_number_ == MACHINE_A2560K60 ||
			the_system->model_number_ == MACHINE_GENX)
	{
			DEBUG_OUT(("%s %d: I think this is a A2560K or A2560X or GenX...", __func__, __LINE__));
			the_system->num_screens_ = 2;
	}
	else
	{
			DEBUG_OUT(("%s %d: I can't recognize this machine (id=%u). Application will now quit.", __func__, __LINE__, the_system->model_number_));
			return false;
	}
	
	return true;
}


//! Find out what kind of machine the software is running on, and configure the passed screen accordingly
//! Configures screen settings, RAM addresses, etc. based on known info about machine types
//! Configures screen width, height, total text rows and cols, and visible text rows and cols by checking hardware
//! @param	the_system -- valid pointer to system object
//! @return	Returns false if the machine is known to be incompatible with this software. 
bool Sys_AutoConfigure(System* the_system)
{
	int16_t				i;

	// TEMP until bug fix for calypsi on switch below
	if (the_system->model_number_ == MACHINE_A2560U_PLUS || the_system->model_number_ == MACHINE_A2560M)
	{
		the_system->screen_[ID_CHANNEL_A]->vicky_ = P32(VICKY_A2560U);
		the_system->screen_[ID_CHANNEL_A]->text_ram_ = TEXT_RAM_A2560U;
		the_system->screen_[ID_CHANNEL_A]->text_attr_ram_ = TEXT_ATTR_A2560U;
		the_system->screen_[ID_CHANNEL_A]->text_font_ram_ = FONT_MEMORY_BANK_A2560U;
		the_system->screen_[ID_CHANNEL_A]->text_color_fore_ram_ = (uint32_t*)TEXT_FORE_LUT_A2560U;
		the_system->screen_[ID_CHANNEL_A]->text_color_back_ram_ = (uint32_t*)TEXT_BACK_LUT_A2560U;

		the_system->screen_[ID_CHANNEL_B]->vicky_ = P32(VICKY_A2560U);
		the_system->screen_[ID_CHANNEL_B]->text_ram_ = TEXT_RAM_A2560U;
		the_system->screen_[ID_CHANNEL_B]->text_attr_ram_ = TEXT_ATTR_A2560U;
		the_system->screen_[ID_CHANNEL_B]->text_font_ram_ = FONT_MEMORY_BANK_A2560U;
		the_system->screen_[ID_CHANNEL_B]->text_color_fore_ram_ = (uint32_t*)TEXT_FORE_LUT_A2560U;
		the_system->screen_[ID_CHANNEL_B]->text_color_back_ram_ = (uint32_t*)TEXT_BACK_LUT_A2560U;
	}
	else if (the_system->model_number_ == MACHINE_A2560X || 
			the_system->model_number_ == MACHINE_A2560K ||
			the_system->model_number_ == MACHINE_A2560K40 ||
			the_system->model_number_ == MACHINE_A2560K60 ||
			the_system->model_number_ == MACHINE_GENX
			)
	{
		the_system->screen_[ID_CHANNEL_A]->vicky_ = P32(VICKY_A_BASE_ADDRESS);
		the_system->screen_[ID_CHANNEL_A]->text_ram_ = TEXTA_RAM_A2560K;
		the_system->screen_[ID_CHANNEL_A]->text_attr_ram_ = TEXTA_ATTR_A2560K;
		the_system->screen_[ID_CHANNEL_A]->text_font_ram_ = FONT_MEMORY_BANKA_A2560K;
		the_system->screen_[ID_CHANNEL_A]->text_color_fore_ram_ = (uint32_t*)TEXTA_FORE_LUT_A2560K;
		the_system->screen_[ID_CHANNEL_A]->text_color_back_ram_ = (uint32_t*)TEXTA_BACK_LUT_A2560K;

		the_system->screen_[ID_CHANNEL_B]->vicky_ = P32(VICKY_B_BASE_ADDRESS);
		the_system->screen_[ID_CHANNEL_B]->text_ram_ = TEXTB_RAM_A2560K;
		the_system->screen_[ID_CHANNEL_B]->text_attr_ram_ = TEXTB_ATTR_A2560K;
		the_system->screen_[ID_CHANNEL_B]->text_font_ram_ = FONT_MEMORY_BANKB_A2560K;
		the_system->screen_[ID_CHANNEL_B]->text_color_fore_ram_ = (uint32_t*)TEXTB_FORE_LUT_A2560K;
		the_system->screen_[ID_CHANNEL_B]->text_color_back_ram_ = (uint32_t*)TEXTB_BACK_LUT_A2560K;
	}
	else
	{
		DEBUG_OUT(("%s %d: this system %i not supported!", __func__, __LINE__, the_system->model_number_));
	}


	// Teach each screen about its size, visible columns, resolution, etc.
	// Ok to do for 1 screen only if system has only 1 physical screen, because sys_new() points second screen at first anyway in 1-screen systems
	for (i = 0; i < the_system->num_screens_; i++)
	{
		the_system->screen_[i]->rect_.MinX = 0;
		the_system->screen_[i]->rect_.MinY = 0;	
		the_system->screen_[i]->text_font_width_ = TEXT_FONT_WIDTH;
		the_system->screen_[i]->text_font_height_ = TEXT_FONT_HEIGHT;

		// use auto configure to set resolution, text cols, margins, etc
		if (Sys_DetectScreenSize(the_system->screen_[i]) == false)
		{
			LOG_ERR(("%s %d: Unable to auto-configure screen resolution", __func__, __LINE__));
			return false;
		}

		// note: can't use memcpy with the VICKY 32 bit values, as memcpy writes byte by byte, and VICKY only accepts 32 bit writes (for given locations anyway)
		for (uint8_t j=0; j < 16; j++)
		{
			the_system->screen_[i]->text_color_fore_ram_[j] = standard_text_color_lut[j];
			the_system->screen_[i]->text_color_back_ram_[j] = standard_text_color_lut[j];
		}

		DEBUG_OUT(("%s %d: This screen (id=%i: %i x %i, with %i x %i text (%i x %i visible)", __func__, __LINE__, 
			the_system->screen_[i]->id_,
			the_system->screen_[i]->width_, 
			the_system->screen_[i]->height_, 
			the_system->screen_[i]->text_mem_cols_, 
			the_system->screen_[i]->text_mem_rows_, 
			the_system->screen_[i]->text_cols_vis_, 
			the_system->screen_[i]->text_rows_vis_
			));
	}
	
	// always enable gamma correction
	Sys_SetGammaMode(the_system, the_system->screen_[0], true);
	
	return true;
}


//! Detect the current screen mode/resolution, and set # of columns, rows, H pixels, V pixels, accordingly
bool Sys_DetectScreenSize(Screen* the_screen)
{
	screen_resolution	new_mode;
	uint32_t			the_vicky_value;
	uint8_t				the_video_mode_bits;
	uint32_t			the_border_control_value;
	int16_t				border_x_cols;
	int16_t				border_y_cols;
	int16_t				border_x_pixels;
	int16_t				border_y_pixels;
	
	if (the_screen == NULL)
	{
		LOG_ERR(("%s %d: passed screen was NULL", __func__, __LINE__));
		goto error;
	}

	// detect the video mode and set resolution based on it
	
	//v = (unsigned char*)the_screen->vicky_;
	the_vicky_value = *the_screen->vicky_;
	the_video_mode_bits = (the_vicky_value >> 8) & 0xff;
	DEBUG_OUT(("%s %d: 32bit vicky value=%x, video mode bits before vid flag=%x", __func__, __LINE__, the_vicky_value, the_video_mode_bits));
	the_video_mode_bits &= VICKY_AB_RES_FLAG_MASK_8BIT;	// further mask down to just the 2 bits relevant for detecting screen resolution
	DEBUG_OUT(("%s %d: 32bit vicky value=%x, video mode bits=%x", __func__, __LINE__, the_vicky_value, the_video_mode_bits));
	
	if (the_screen->vicky_ == P32(VICKY_A_BASE_ADDRESS))
	{
		// LOGIC: A2560K channel A only has 2 video modes, 1024x768 and 800x600. If bit 11 is set, it's 1024. 
		
		//DEBUG_OUT(("%s %d: vicky identified as VICKY_A_BASE_ADDRESS", __func__, __LINE__));

		if (the_video_mode_bits & VICKY_IIIA_RES_1024X768_FLAGS)
		{
			new_mode = RES_1024X768;
		}
		else
		{
			new_mode = RES_800X600;
		}
	}
	else if (the_screen->vicky_ == P32(VICKY_B_BASE_ADDRESS))
	{
		// LOGIC: 
		//   A2560K channel B only has 3 video modes, 800x600, 640x480, and 640x400 (currently non-functional)
		//   if bit 8 is set, it's 800x600, if bits 8/9 both set, it's 640x400. 

		DEBUG_OUT(("%s %d: vicky identified as VICKY_B_BASE_ADDRESS; bits=%x, 800x600=%x, 640x480=%x, vicky val=%x", __func__, __LINE__, the_video_mode_bits, VICKY_IIIB_RES_800X600_FLAGS, VICKY_IIIB_RES_640X480_FLAGS, the_vicky_value));
		//printf("detectscreen: bits=%x, 800x600=%x, 640x480=%x, vicky val=%x \n", the_video_mode_bits, VICKY_IIIB_RES_800X600_FLAGS, VICKY_IIIB_RES_640X480_FLAGS, the_vicky_value);

		if (the_video_mode_bits & VICKY_IIIB_RES_800X600_FLAGS || the_video_mode_bits & VICKY_IIIB_RES_RESERVED_FLAGS)	// MB: this shouldn't be according to manual, but the reserved value is what I observer when MCp sets 800x600 mode.
		{
			new_mode = RES_800X600;
		}
		else if (the_video_mode_bits & VICKY_IIIB_RES_640X400_FLAGS)
		{
			new_mode = RES_640X400;
		}
		else
		{
			new_mode = RES_640X480;
		}
	}
	else if (the_screen->vicky_ == P32(VICKY_A2560U))
	{
 		//   A2560U has 1 channel with 3 video modes, 800x600, 640x480, and 640x400 (currently non-functional)
		//   if bit 8 is set, it's 800x600, if bits 8/9 both set, it's 640x400. 

		//DEBUG_OUT(("%s %d: vicky identified as VICKY_A2560U", __func__, __LINE__));

		if (the_video_mode_bits & VICKY_II_RES_800X600_FLAGS)
		{
			new_mode = RES_800X600;
		}
		else if (the_video_mode_bits & VICKY_II_RES_640X400_FLAGS)
		{
			new_mode = RES_640X400;
		}
		else
		{
			new_mode = RES_640X480;
		}
	}
	else
	{
		LOG_ERR(("%s %d: The VICKY register on this machine (%p) doesn't match one I know of/care about. I won't be able to figure out what the screen size is.", __func__, __LINE__, the_screen->vicky_));
		return false;
	}

	switch (new_mode)
	{
		case RES_640X400:
			the_screen->width_ = 640;	
			the_screen->height_ = 400;
			DEBUG_OUT(("%s %d: set to RES_640X400", __func__, __LINE__));
			break;
			
		case RES_640X480:
			the_screen->width_ = 640;	
			the_screen->height_ = 480;
			DEBUG_OUT(("%s %d: set to RES_640X480", __func__, __LINE__));
			break;
			
		case RES_800X600:
			the_screen->width_ = 800;	
			the_screen->height_ = 600;
			DEBUG_OUT(("%s %d: set to RES_800X600", __func__, __LINE__));
			break;
			
		case RES_1024X768:
			the_screen->width_ = 1024;	
			the_screen->height_ = 768;
			DEBUG_OUT(("%s %d: set to 1024x768", __func__, __LINE__));
			break;		
	}
	
	// detect borders, and set text cols/rows based on resolution modified by borders (if any)
	the_border_control_value = R32(the_screen->vicky_ + BORDER_CTRL_OFFSET_L);
	border_x_pixels = (the_border_control_value >> 8) & 0xFF & 0x3F;
	border_y_pixels = (the_border_control_value >> 16) & 0xFF & 0x3F;
	
	border_x_cols = border_x_pixels * 2 / the_screen->text_font_width_;
	border_y_cols = border_y_pixels * 2 / the_screen->text_font_height_;
	the_screen->text_mem_cols_ = the_screen->width_ / the_screen->text_font_width_;
	the_screen->text_mem_rows_ = the_screen->height_ / the_screen->text_font_height_;
	the_screen->text_cols_vis_ = the_screen->text_mem_cols_ - border_x_cols;
	the_screen->text_rows_vis_ = the_screen->text_mem_rows_ - border_y_cols;
	the_screen->rect_.MaxX = the_screen->width_;
	the_screen->rect_.MaxY = the_screen->height_;	
	Sys_PrintScreen(the_screen);
	
	DEBUG_OUT(("%s %d: screen=%u, vis cols=%u, vis rows=%u, brdx=%u, brdy=%un", __func__, __LINE__, the_screen->id_, the_screen->text_cols_vis_, the_screen->text_rows_vis_, border_x_cols, border_y_cols));

	return true;
	
error:
	Sys_Exit(&global_system, PARAM_EXIT_ON_ERROR);	// crash early, crash often
	return false;
}


//! Change the cursor character to the passed character
//! @param	the_char -- the character point to be set as the cursor graphic
//! FUTURE: pass and set foreground and background cursor color; pass and set blink speed.
void Sys_ChangeCursor(uint8_t the_char)
{
	uint32_t	the_old_value1;
	uint32_t	the_old_value2;
	uint32_t	the_new_value1;
	uint32_t	the_new_value2;
	uint32_t	the_old_masked_value1;
	uint32_t	the_old_masked_value2;

	the_old_value1 = R32(VICKYA_CURSOR_CTRL_A2560K);
	the_old_masked_value1 = the_old_value1 & VICKY_AB_CCR_CHAR_MASK;
	the_new_value1 = the_old_masked_value1 | (CH_HFILL_UP_8 << 16);
	//printf("A: old=%x, old masked=%x, new val=%x \n", the_old_value1, the_old_masked_value1, the_new_value1);
	R32(VICKYA_CURSOR_CTRL_A2560K) = the_new_value1;
	
	the_old_value2 = R32(VICKYB_CURSOR_CTRL_A2560K);
	the_old_masked_value2 = the_old_value2 & VICKY_AB_CCR_CHAR_MASK;
	the_new_value2 = the_old_masked_value2 | (CH_HFILL_UP_8 << 16);
	//printf("B: old=%x, old masked=%x, new val=%x \n", the_old_value2, the_old_masked_value2, the_new_value2);
	R32(VICKYB_CURSOR_CTRL_A2560K) = the_new_value2;
}




// **** Debug functions *****

void Sys_Print(System* the_system)
{
	DEBUG_OUT(("System print out:"));
	DEBUG_OUT(("  address: %p", 			the_system));
	DEBUG_OUT(("  event_manager_: %p", 		the_system->event_manager_));
	DEBUG_OUT(("  menu_manager_: %p", 		the_system->menu_manager_));
	DEBUG_OUT(("  system_font_: %p", 		the_system->system_font_));
	DEBUG_OUT(("  app_font_: %p",			the_system->app_font_));
	DEBUG_OUT(("  theme_: %p",				the_system->theme_));
	DEBUG_OUT(("  num_screens_: %i",		the_system->num_screens_));
	DEBUG_OUT(("  window_count_: %i",		the_system->window_count_));
	DEBUG_OUT(("  active_window_: %p",		the_system->active_window_));
	DEBUG_OUT(("  model_number_: %i",		the_system->model_number_));
}


void Sys_PrintScreen(Screen* the_screen)
{
	DEBUG_OUT(("Screen print out:"));
	DEBUG_OUT(("  address: %p", 			the_screen));
	DEBUG_OUT(("  id_: %i", 				the_screen->id_));
	DEBUG_OUT(("  vicky_: %p", 				the_screen->vicky_));
	DEBUG_OUT(("  width_: %i", 				the_screen->width_));
	DEBUG_OUT(("  height_: %i", 			the_screen->height_));
	DEBUG_OUT(("  text_cols_vis_: %i", 		the_screen->text_cols_vis_));
	DEBUG_OUT(("  text_rows_vis_: %i", 		the_screen->text_rows_vis_));
	DEBUG_OUT(("  text_mem_cols_: %i", 		the_screen->text_mem_cols_));
	DEBUG_OUT(("  text_mem_rows_: %i", 		the_screen->text_mem_rows_));
	DEBUG_OUT(("  text_ram_: %p", 			the_screen->text_ram_));
	DEBUG_OUT(("  text_attr_ram_: %p", 		the_screen->text_attr_ram_));
	DEBUG_OUT(("  text_font_ram_: %p", 		the_screen->text_font_ram_));
	DEBUG_OUT(("  bitmap_[0]: %p", 			the_screen->bitmap_[0]));
	DEBUG_OUT(("  bitmap_[1]: %p", 			the_screen->bitmap_[1]));
	DEBUG_OUT(("  text_font_height_: %i",	the_screen->text_font_height_));
	DEBUG_OUT(("  text_font_width_: %i",	the_screen->text_font_width_));
}



/*****************************************************************************/
/*                        Public Function Definitions                        */
/*****************************************************************************/

// **** CONSTRUCTOR AND DESTRUCTOR *****

// constructor
System* Sys_New(void)
{
	System*			the_system;
	
	if ( (the_system = (System*)calloc(1, sizeof(System)) ) == NULL)
	{
		LOG_ERR(("%s %d: could not allocate memory to create new system", __func__ , __LINE__));
		goto error;
	}
	LOG_ALLOC(("%s %d:	__ALLOC__	the_system	%p	size	%i", __func__ , __LINE__, the_system, sizeof(System)));
	
	DEBUG_OUT(("%s %d: System object created ok...", __func__ , __LINE__));
	
	return the_system;
	
error:
	if (the_system)					Sys_Exit(&the_system, PARAM_EXIT_ON_ERROR);
	return NULL;
}


// destructor
// frees all allocated memory associated with the passed object, and the object itself
//! @param	the_system -- valid pointer to system object
bool Sys_Destroy(System** the_system)
{
	int16_t	i;
	
	if (*the_system == NULL)
	{
		LOG_ERR(("%s %d: passed class object was null", __func__ , __LINE__));
		return false;
	}

	for (i = 0; i < 2; i++)
	{
		if ((*the_system)->screen_[i])
		{
			LOG_ALLOC(("%s %d:	__FREE__	(*the_system)->screen_[i]	%p	size	%i", __func__ , __LINE__, (*the_system)->screen_[i], sizeof(Screen)));
			free((*the_system)->screen_[i]);
			(*the_system)->screen_[i] = NULL;
		}
	}

	if ((*the_system)->system_font_)
	{
		Font_Destroy(&(*the_system)->system_font_);
	}

	if ((*the_system)->app_font_)
	{
		Font_Destroy(&(*the_system)->app_font_);
	}

	if ((*the_system)->menu_manager_)
	{
		Menu_Destroy(&(*the_system)->menu_manager_);
	}

	if ((*the_system)->event_manager_)
	{
		EventManager_Destroy(&(*the_system)->event_manager_);
	}

	if ((*the_system)->list_windows_)
	{
		Sys_DestroyAllWindows(*the_system);
	}


	LOG_ALLOC(("%s %d:	__FREE__	*the_system	%p	size	%i", __func__ , __LINE__, *the_system, sizeof(System)));
	free(*the_system);
	*the_system = NULL;
	
	return true;
}


//! Exit to MCP
//! Destroys the system on the way out
//! @param	the_system -- valid pointer to system object
//! @param	error_condition -- true if error, false if a normal exit. Use PARAM_EXIT_ON_ERROR/PARAM_EXIT_NO_ERROR
void Sys_Exit(System** the_system, bool error_condition)
{
	// clean up system objects
	Sys_Destroy(the_system);
	
	// call MCP exit to return control to it
	if (error_condition == PARAM_EXIT_ON_ERROR)
	{
		DEBUG_OUT(("%s %d: **** SYSTEM EXIT ON ERROR! ****", __func__, __LINE__));		
		sys_exit(-1);
	}
	else
	{
		DEBUG_OUT(("%s %d: **** SYSTEM EXIT ****", __func__, __LINE__));		
		sys_exit(0);
	}
}



// **** System Initialization functions *****

//! Initialize the system (primary entry point for all system initialization activity)
//! Starts up the memory manager, creates the global system object, runs autoconfigure to check the system hardware, loads system and application fonts, allocates a bitmap for the screen.
bool Sys_InitSystem(System* the_system)
{
	Theme*		the_theme;
	int16_t		i;
	
	// initialize logging
	#if defined LOG_LEVEL_1 || defined LOG_LEVEL_2 || defined LOG_LEVEL_3 || defined LOG_LEVEL_4 || defined LOG_LEVEL_5
		General_LogInitialize();
		printf("logging started \n");
	#endif

	// temp text buffer
	if ( (the_system->text_temp_buffer_ = (char*)calloc(WORD_WRAP_MAX_LEN + 1, sizeof(char)) ) == NULL)
	{
		LOG_ERR(("%s %d: could not allocate memory to create the temp text buffer", __func__ , __LINE__));
		goto error;
	}

	DEBUG_OUT(("%s %d: Sys temp text buffer created ok...", __func__ , __LINE__));

	// event manager
	if ( (the_system->event_manager_ = EventManager_New() ) == NULL)
	{
		LOG_ERR(("%s %d: could not allocate memory to create the event manager", __func__ , __LINE__));
		goto error;
	}

	DEBUG_OUT(("%s %d: EventManager created ok. Detecting hardware...", __func__ , __LINE__));
	
	// check what kind of hardware the system is running on
	// LOGIC: we need to know how many screens it has before allocating screen objects
	if (Sys_AutoDetectMachine(the_system) == false)
	{
		LOG_ERR(("%s %d: Detected machine hardware is incompatible with this software", __func__ , __LINE__));
		goto error;
	}
	
	DEBUG_OUT(("%s %d: Hardware detected (%u screens). Creating screens...", __func__ , __LINE__, the_system->num_screens_));

	// screens
	for (i = 0; i < the_system->num_screens_; i++)
	{
		if ( (the_system->screen_[i] = (Screen*)calloc(1, sizeof(Screen)) ) == NULL)
		{
			LOG_ERR(("%s %d: could not allocate memory to create screen object", __func__ , __LINE__));
			goto error;
		}
		LOG_ALLOC(("%s %d:	__ALLOC__	the_system->screen_[%i]	%p	size	%i", __func__ , __LINE__, i, the_system->screen_[i], sizeof(Screen)));
		
		the_system->screen_[i]->id_ = i;
	}

	DEBUG_OUT(("%s %d: Screen(s) created ok.", __func__ , __LINE__, i));
	
	// for systems with only one screen, we will point the 2nd screen to the first, so that any call to 2nd screen works as if it was a call to the first. 
	if (the_system->num_screens_ == 1)
	{
		the_system->screen_[1] = the_system->screen_[0];
	}

	DEBUG_OUT(("%s %d: returning to SysInit()...", __func__ , __LINE__, i));
	
	// LOGIC: we don't have font info yet; just want to make it clear these are not set and not rely on compiler behavior
	the_system->system_font_ = NULL;
	the_system->app_font_ = NULL;
	the_system->theme_ = NULL;
	the_system->active_window_ = NULL;
	the_system->window_count_ = 0;

// 	// set the global variable that other classes/libraries need access to.
// 	global_system = the_system;

	DEBUG_OUT(("%s %d: Running Autoconfigure...", __func__, __LINE__));
	
	if (Sys_AutoConfigure(global_system) == false)
	{
		LOG_ERR(("%s %d: Auto configure failed", __func__, __LINE__));
		goto error;
	}

	// LOGIC:
	//   load default theme so that fonts are available
	//   having system fonts in lib sys so they are guaranteed is good, but once a theme is loaded it replaces theme
	//   so default theme needs to know how to reload them in case user switches back
	//   better to have it consistent: theme is responsible for loading and setting system fonts
	
	DEBUG_OUT(("%s %d: loading default theme...", __func__, __LINE__));
	
	if ( (the_theme = Theme_CreateDefaultTheme(THEME_PARAM_FULL_RESOURCES) ) == NULL)
	{
		LOG_ERR(("%s %d: Failed to create default system theme", __func__, __LINE__));
		goto error;
	}
	
	Theme_Activate(the_theme);
	
	DEBUG_OUT(("%s %d: Default theme loaded ok. Creating menu manager...", __func__ , __LINE__));
	
	// menu manager
	if ( (global_system->menu_manager_ = Menu_New() ) == NULL)
	{
		LOG_ERR(("%s %d: could not allocate memory to create the menu manager", __func__ , __LINE__));
		goto error;
	}	

	DEBUG_OUT(("%s %d: allocating screen bitmap...", __func__, __LINE__));
	
	// allocate the foreground and background bitmaps, then assign them fixed locations in VRAM
	
	// LOGIC: 
	//   The only bitmaps we want pointing to VRAM locations are the system's layer0 and layer1 bitmaps for the screen
	//   Only 1 screen has bitmapped graphics
	//   We assign them fixed spaces in VRAM, 800*600 apart, so that the addresses are good even on a screen resolution change. 
	
	for (i = 0; i < 2; i++)
	{
		Bitmap*		the_bitmap;

		if ( (the_bitmap = Bitmap_New(global_system->screen_[ID_CHANNEL_B]->width_, global_system->screen_[ID_CHANNEL_B]->height_, Sys_GetSystemFont(global_system), PARAM_IN_VRAM)) == NULL)
		{
			LOG_ERR(("%s %d: Failed to create bitmap #%i", __func__, __LINE__, i));
			goto error;
		}

		the_bitmap->addr_int_ = (uint32_t)VRAM_START + (uint32_t)i * (uint32_t)VRAM_OFFSET_TO_NEXT_SCREEN;
		the_bitmap->addr_ = (unsigned char*)the_bitmap->addr_int_;

		//DEBUG_OUT(("%s %d: bitmap addr_int_=%lx, addr_=%p", __func__, __LINE__, the_bitmap->addr_int_, the_bitmap->addr_));
		
		Sys_SetScreenBitmap(global_system, the_bitmap, i);
		
		// clear the bitmap
		Bitmap_FillMemory(the_bitmap, 0x00);
	}
	
	// load the standard text font
	Text_UpdateFontData(global_system->screen_[ID_CHANNEL_A], (char*)system_std_font);
	Text_UpdateFontData(global_system->screen_[ID_CHANNEL_B], (char*)system_std_font);
	
	// switch cursor to a full block. 
	Sys_ChangeCursor(CH_HFILL_UP_8);

// 	// load the splash screen and progress bar
// 	if (Startup_ShowSplash() == false)
// 	{
// 		LOG_ERR(("%s %d: Failed to load splash screen. Oh, no!", __func__, __LINE__));
// 		// but who cares, just continue on... 
// 	}

	// create the backdrop window and add it to the list of the windows the system tracks
	
	// LOGIC:
	//   Every app will use (or at least have access to) the backdrop window
	//   The backdrop window shares the same bitmap as the Screen
	//   The backdrop window will catch events that drop through the windows in the foreground
	
	if ( Sys_CreateBackdropWindow(global_system) == false)
	{
		LOG_ERR(("%s %d: Failed to create a backdrop window. Fatal error.", __func__, __LINE__));
		goto error;
	}
	
	// Enable mouse pointer -- no idea if this works, f68 emulator doesn't support mouse yet. 
	//R32(VICKYB_MOUSE_CTRL_A2560K) = 1;
	
	// set interrupt handlers
	ps2_init();
//	global_old_keyboard_interrupt = sys_int_register(INT_KBD_PS2, &Sys_InterruptKeyboard);
// 	global_old_keyboard_interrupt = sys_int_register(INT_KBD_A2560K, &Sys_InterruptKeyboard);
// #define INT_MOUSE           0x12    /* SuperIO - PS/2 Mouse */
// 
// 	global_old_mouse_interrupt = sys_int_register(INT_MOUSE, &mouse_handle_irq);
// 	DEBUG_OUT(("%s %d: osf mouse interrupt (%p) installed, replacing MCP %p", __func__, __LINE__, &mouse_handle_irq, global_old_mouse_interrupt));
// 	global_old_mouse_interrupt = sys_int_register(INT_MOUSE, &Sys_InterruptMouse);
// 	DEBUG_OUT(("%s %d: osf mouse interrupt (%p) installed, replacing MCP %p", __func__, __LINE__, &Sys_InterruptMouse, global_old_mouse_interrupt));


	DEBUG_OUT(("%s %d: System initialization complete.", __func__, __LINE__));

	return true;
	
error:
	Sys_Exit(&global_system, PARAM_EXIT_ON_ERROR);	// crash early, crash often
	return false;
}





// **** Event-handling functions *****

// see MCP's ps2.c for real examples once real machine available

// interrupt 1 is PS2 keyboard, interrupt 2 is A2560K keyboard
void Sys_InterruptKeyboard(void)
{
	printf("keyboard!\n");
	kbd_handle_irq();
	return;
}

// Interrupt handler for PS/2 mouse
void Sys_InterruptMouse(void)
{
	printf("mouse!\n");
	mouse_handle_irq();
	return;
}




// **** Screen mode/resolution/size functions *****


//! Switch machine into graphics mode, text mode, sprite mode, etc.
//! @param	the_system -- valid pointer to system object
//! Use PARAM_SPRITES_ON/OFF, PARAM_BITMAP_ON/OFF, PARAM_TILES_ON/OFF, PARAM_TEXT_OVERLAY_ON/OFF, PARAM_TEXT_ON/OFF
bool Sys_SetGraphicMode(System* the_system, bool enable_sprites, bool enable_bitmaps, bool enable_tiles, bool enable_text_overlay, bool enable_text)
{	
	uint8_t		the_bits;
	uint32_t	the_old_value;
	uint32_t	the_old_masked_value;
	uint32_t	the_new_value;
	
	if (the_system == NULL)
	{
		LOG_ERR(("%s %d: passed class object was NULL", __func__, __LINE__));
		goto error;
	}
	
	// LOGIC:
	//   On an A2560K or X, the only screen that has a text/graphics mode is the Channel B screen
	//   clears everything from first byte of master control but the reserved bit (bit 6)
	//   then re-enables only those modes specified

	the_old_value = R32(the_system->screen_[ID_CHANNEL_B]->vicky_);
	the_old_masked_value = the_old_value & GRAPHICS_MODE_MASK;
	
	the_bits = 0;
	//the_bits = (uint8_t)the_old_masked_value;
	//the_bits = R8(VICKY_MASTER_CTRL_REG_L) & GRAPHICS_MODE_EN_GAMMA;	

	if (enable_sprites | enable_bitmaps | enable_tiles | enable_text_overlay)
	{
		the_bits |= GRAPHICS_MODE_GRAPHICS;
	}
	
	if (enable_sprites)
	{
		the_bits |= GRAPHICS_MODE_EN_SPRITE;
	}

	if (enable_bitmaps)
	{
		the_bits |= GRAPHICS_MODE_EN_BITMAP;
	}

	if (enable_tiles)
	{
		the_bits |= GRAPHICS_MODE_EN_TILE;
	}

	if (enable_text)
	{
		the_bits |= GRAPHICS_MODE_TEXT;
	}

	if (enable_text_overlay)
	{
		the_bits |= GRAPHICS_MODE_TEXT;
		the_bits |= GRAPHICS_MODE_TEXT_OVER;
		the_bits |= GRAPHICS_MODE_GRAPHICS;
	}

	// turn bitmaps on or off, as needed
	if (enable_bitmaps || enable_tiles)
	{
			// enable bitmap layers 0, 1; disable layer 2
		R32(the_system->screen_[ID_CHANNEL_B]->vicky_ + BITMAP_L0_CTRL_L) = 0x01;
		R32(the_system->screen_[ID_CHANNEL_B]->vicky_ + BITMAP_L1_CTRL_L) = 0x01;	
	}
	else
	{
		// disable bitmap layers 0, 1, 2
		R32(the_system->screen_[ID_CHANNEL_B]->vicky_ + BITMAP_L0_CTRL_L) = 0x00;
		R32(the_system->screen_[ID_CHANNEL_B]->vicky_ + BITMAP_L1_CTRL_L) = 0x00;	
	}

	// switch to graphics mode by setting graphics mode bit, and setting bitmap engine enable bit
	//the_new_value = (the_old_value & ~GRAPHICS_MODE_MASK) | (uint32_t)the_bits;
	the_new_value = (the_old_masked_value) | (uint32_t)the_bits;
	DEBUG_OUT(("%s %d: graphics: old masked=%ul, old=%ul, new=%ul", __func__, __LINE__, the_old_masked_value, the_old_value, the_new_value));
// old masked = 280000001
// old=			28000000B
// new = 		280000047 0100 0111
	R32(the_system->screen_[ID_CHANNEL_B]->vicky_) = the_new_value;
	
	return true;
	
error:
	return false;
}


// //! Switch machine into graphics mode
// //! @param	the_system -- valid pointer to system object
// bool Sys_SetModeGraphics(System* the_system)
// {	
// 	if (the_system == NULL)
// 	{
// 		LOG_ERR(("%s %d: passed class object was NULL", __func__, __LINE__));
// 		goto error;
// 	}
// 	
// 	// LOGIC:
// 	//   On an A2560K or X, the only screen that has a text/graphics mode is the Channel B screen
// 	
// 	// switch to graphics mode by setting graphics mode bit, and setting bitmap engine enable bit
// 
// 	// enable bitmap layers 0 and 1
// 		//*the_screen->vicky_ = (*the_screen->vicky_ & GRAPHICS_MODE_MASK | (GRAPHICS_MODE_GRAPHICS) | GRAPHICS_MODE_EN_BITMAP);
// 	R32(the_system->screen_[ID_CHANNEL_B]->vicky_) = (*the_system->screen_[ID_CHANNEL_B]->vicky_ & GRAPHICS_MODE_MASK | GRAPHICS_MODE_GRAPHICS | GRAPHICS_MODE_EN_BITMAP);
// 
// 	R32(the_system->screen_[ID_CHANNEL_B]->vicky_ + BITMAP_L0_CTRL_L) = 0x01;
// 	R32(the_system->screen_[ID_CHANNEL_B]->vicky_ + BITMAP_L1_CTRL_L) = 0x01;	
// 	
// 	return true;
// 	
// error:
// 	Sys_Exit(&global_system, PARAM_EXIT_ON_ERROR);	// crash early, crash often
// 	return false;
// }


//! Switch machine into text mode
//! @param	the_system -- valid pointer to system object
//! @param	as_overlay -- If true, sets text overlay mode (text over graphics). If false, sets full text mode (no graphics);
bool Sys_SetModeText(System* the_system, bool as_overlay)
{	
	if (the_system == NULL)
	{
		LOG_ERR(("%s %d: passed class object was NULL", __func__, __LINE__));
		goto error;
	}
	
	// LOGIC:
	//   On an A2560K or X, the only screen that has a text/graphics mode is the Channel B screen
	
	if (as_overlay)
	{
		// switch to text mode with overlay by setting graphics mode bit, setting bitmap engine enable bit, and setting graphics mode overlay		
		R32(the_system->screen_[ID_CHANNEL_B]->vicky_) = ((*the_system->screen_[ID_CHANNEL_B]->vicky_ & GRAPHICS_MODE_MASK) | GRAPHICS_MODE_TEXT | GRAPHICS_MODE_TEXT_OVER | GRAPHICS_MODE_GRAPHICS | GRAPHICS_MODE_EN_BITMAP);
		
		// c256foenix, discord 2022/03/10
		// Normally, for example, if you setup everything to be in bitmap mode, and you download an image in VRAM and you can see it properly... If you turn on overlay, then you will see on top of that same image, your text that you had before.
		// Mstr_Ctrl_Text_Mode_En  = $01       ; Enable the Text Mode
		// Mstr_Ctrl_Text_Overlay  = $02       ; Enable the Overlay of the text mode on top of Graphic Mode (the Background Color is ignored)
		// Mstr_Ctrl_Graph_Mode_En = $04       ; Enable the Graphic Mode
		// Mstr_Ctrl_Bitmap_En     = $08       ; Enable the Bitmap Module In Vicky
		// all of these should be ON
	}
	else
	{
		R32(the_system->screen_[ID_CHANNEL_B]->vicky_) = ((*the_system->screen_[ID_CHANNEL_B]->vicky_ & GRAPHICS_MODE_MASK) | GRAPHICS_MODE_TEXT);
		// disable bitmap layers 0 and 1
		R32(the_system->screen_[ID_CHANNEL_B]->vicky_ + BITMAP_L0_CTRL_L) = 0x00;
		R32(the_system->screen_[ID_CHANNEL_B]->vicky_ + BITMAP_L1_CTRL_L) = 0x00;
	}
	
	return true;
	
error:
	Sys_Exit(&global_system, PARAM_EXIT_ON_ERROR);	// crash early, crash often
	return false;
}


//! Change video mode to the one passed.
//! @param	the_screen -- valid pointer to the target screen to operate on
//! @param	new_mode -- One of the enumerated screen_resolution values. Must correspond to a valid VICKY video mode for the host machine. See VICKY_IIIA_RES_800X600_FLAGS, etc. defined in a2560_platform.h
//! @return	returns false on any error/invalid input.
bool Sys_SetVideoMode(Screen* the_screen, screen_resolution new_mode)
{
// 	uint8_t		the_bits;
// 	uint32_t	the_old_value;
// 	uint32_t	the_old_masked_value;
// 	uint32_t	the_new_value;
	uint8_t		the_mcp_screen_id;
	
	if (the_screen == NULL)
	{
		LOG_ERR(("%s %d: passed class object was NULL", __func__, __LINE__));
		goto error;
	}
	
	// LOGIC:
	//   On an A2560K or X, the only screen that has a text/graphics mode is the Channel B screen
	//   clears everything from first byte of master control but the reserved bit (bit 6)
	//   then re-enables only those modes specified

// 	the_old_value = R32(the_screen->vicky_);	// R32(VICKY_B_MASTER_CONTROL) etc also works.
// 	the_old_masked_value = the_old_value & VIDEO_MODE_MASK;	
// 	the_bits = 0;
// 
// 	// TODO: figure out smarter way of knowing which video modes are legal for the machine being run on
// 	if (the_screen->vicky_ == P32(VICKY_A_BASE_ADDRESS))
// 	{
// 		//DEBUG_OUT(("%s %d: vicky identified as VICKY_A_BASE_ADDRESS", __func__, __LINE__));
// 		
// 		if (new_mode == RES_800X600)
// 		{
// 			the_bits = VICKY_IIIA_RES_800X600_FLAGS;
// 		}
// 		else if (new_mode == RES_1024X768)
// 		{
// 			the_bits = VICKY_IIIA_RES_1024X768_FLAGS;
// 		}
// 	}
// 	else if (the_screen->vicky_ == P32(VICKY_B_BASE_ADDRESS))
// 	{
// 		//DEBUG_OUT(("%s %d: vicky identified as VICKY_B_BASE_ADDRESS", __func__, __LINE__));
// 		// Video Mode [1] [0]
// 		// 640x480 @ 60FPS 0 0
// 		// 800x600 @ 60FPS 0 1
// 		// Reserved 1 0
// 		// 640x400 @ 70FPS 1 1		
// 		
// 		if (new_mode == RES_640X400)
// 		{
// 			LOG_WARN(("%s %d: 640x400 mode is not yet available in hardware", __func__, __LINE__));
// 			//the_bits = VICKY_IIIB_RES_640X400_FLAGS;
// 		}
// 		else if (new_mode == RES_640X480)
// 		{
// 			the_bits = VICKY_IIIB_RES_640X480_FLAGS;
// 		}
// 		else if (new_mode == RES_800X600)
// 		{
// 			the_bits = VICKY_IIIB_RES_800X600_FLAGS;
// 		}
// 	}
// 	else if (the_screen->vicky_ == P32(VICKY_A2560U))
// 	{
//  		//DEBUG_OUT(("%s %d: vicky identified as VICKY_A2560U", __func__, __LINE__));
// 		if (new_mode == RES_640X400)
// 		{
// 			the_bits = VICKY_II_RES_640X400_FLAGS;
// 		}
// 		else if (new_mode == RES_640X480)
// 		{
// 			the_bits = VICKY_II_RES_640X480_FLAGS;
// 		}
// 		else if (new_mode == RES_800X600)
// 		{
// 			the_bits = VICKY_II_RES_800X600_FLAGS;
// 		}
// 	}
 	//DEBUG_OUT(("%s %d: specified video mode = %u, flag=%u", __func__, __LINE__, new_mode, the_bits));
	
	// switch to graphics mode by setting graphics mode bit, and setting bitmap engine enable bit
	//the_new_value = the_old_masked_value | (the_bits << 8);	// this is a 1-byte flag
		//the_new_value = the_old_masked_value | VICKY_B_MC_RES_FLAG_1;	// this is a 4-byte flag, will also work if above logic tweaked to use 4 byte vals
	//DEBUG_OUT(("%s %d: video mode: old masked=%ul, old=%ul, new=%ul", __func__, __LINE__, the_old_masked_value, the_old_value, the_new_value));
	//R32(the_screen->vicky_) = the_new_value;

	// the above code produces the correct values, but does not work by itself:
		// looking at MCP source, it seems the PLL frequency has to be jiggled before changing resolution. 
		// just going to use MCP technique for now.
		// it also looks as if there is a high res DIP switch that can be used to indicate user preference, but isn't necessarily mechanically driving resolution (requires MCP or user program action)
		// https://github.com/pweingar/FoenixMCP/blob/32af2939aff8b219a34fc18374785e43eb3a5e30/src/dev/txt_a2560k_b.c#L167
		
	// use MCP calls to set resolution
	
	// MCP uses 0 for B, 1 for A, we are opposite
	if (the_screen->id_ == ID_CHANNEL_A)
	{
		the_mcp_screen_id = 1;
	}
	else
	{
		the_mcp_screen_id = 0;
	}
	
	if (new_mode == RES_640X400)
	{
		sys_txt_set_resolution(the_mcp_screen_id, 640, 400);
	}
	else if (new_mode == RES_640X480)
	{
		sys_txt_set_resolution(the_mcp_screen_id, 640, 480);
	}
	else if (new_mode == RES_800X600)
	{
		sys_txt_set_resolution(the_mcp_screen_id, 800, 600);
	}
	else if (new_mode == RES_1024X768)
	{
		sys_txt_set_resolution(the_mcp_screen_id, 1024, 768);
	}

	// teach screen about the new settings
	if (Sys_DetectScreenSize(the_screen) == false)
	{
		LOG_ERR(("%s %d: Changed screen resolution, but the selected resolution could not be handled", __func__, __LINE__, new_mode));
		return false;
	}

	// tell the MCP that we changed res so it can update it's internal col sizes, etc. this function is not exposed in MCP headers yet
	//sys_text_setsizes();
	
	return true;
	
error:
	Sys_Exit(&global_system, PARAM_EXIT_ON_ERROR);	// crash early, crash often
	return false;
}


// enable or disable the gamma correction 
bool Sys_SetGammaMode(System* the_system, Screen* the_screen, bool enable_it)
{
	uint8_t		the_gamma_mode_bits;
	uint8_t		new_mode_flag;
	uint32_t	the_vicky_value = R32(the_screen->vicky_);

	// LOGIC:
	//   both C256s and A2560s have a gamma correction mode
	//   It needs to be hardware enabled by turning DIP switch 7 on the motherboard to ON (I believe)
	//     bit 5 (0x20) of the video mode byte in vicky master control reflects the DIP switch setting, but doesn't change anything if you write to it 
	//   byte 3 of the vicky master control appears to be dedicated to Gamma correction, but not all bits are documented. Stay away from all but the first 2!
	//     gamma correction can be activated by setting the first and 2nd bits of byte 3
	
	if (the_screen == NULL)
	{
		LOG_ERR(("%s %d: passed screen was NULL", __func__, __LINE__));
		goto error;
	}

	if (enable_it)
	{
		new_mode_flag = 0xFF;
	}
	else
	{
		new_mode_flag = 0x00;
	}

	the_gamma_mode_bits = (the_vicky_value >> 24) & 0xff;
	//DEBUG_OUT(("%s %d: before: 32bit vicky value=%x, gamma mode bits=%x", __func__, __LINE__, the_vicky_value, the_gamma_mode_bits));
	the_gamma_mode_bits &= (GAMMA_MODE_ONOFF_BITS & new_mode_flag);
	the_vicky_value &= (GAMMA_MODE_MASK | the_gamma_mode_bits << 24);
	//DEBUG_OUT(("%s %d: after: 32bit vicky value=%x, gamma mode bits=%x", __func__, __LINE__, the_vicky_value, the_gamma_mode_bits));
	R32(the_screen->vicky_) = the_vicky_value;
	
	return true; 
	
error:
	Sys_Exit(&global_system, PARAM_EXIT_ON_ERROR);	// crash early, crash often
	return false;
}



//! Enable or disable the hardware cursor in text mode, for the specified screen
//! @param	the_system -- valid pointer to system object
//! @param	the_screen -- valid pointer to the target screen to operate on
//! @param	enable_it -- If true, turns the hardware blinking cursor on. If false, hides the hardware cursor;
bool Sys_EnableTextModeCursor(System* the_system, Screen* the_screen, bool enable_it)
{
	if (the_screen == NULL)
	{
		LOG_ERR(("%s %d: passed screen was NULL", __func__, __LINE__));
		goto error;
	}

	if (enable_it)
	{
		R32(the_screen->vicky_ + CURSOR_CTRL_OFFSET_L) = 1;
	}
	else
	{
		R32(the_screen->vicky_ + CURSOR_CTRL_OFFSET_L) = 0;
	}
	
	return true;
	
error:
	Sys_Exit(&global_system, PARAM_EXIT_ON_ERROR);	// crash early, crash often
	return false;
}


//! Set the left/right and top/bottom borders
//! This will reset the visible text columns as a side effect
//! Grotesquely large values will be accepted as is: use at your own risk!
//! @param	border_width -- width in pixels of the border on left and right side of the screen. Total border used with be the double of this.
//! @param	border_height -- height in pixels of the border on top and bottom of the screen. Total border used with be the double of this.
//! @return	returns false on any error/invalid input.
bool Sys_SetBorderSize(System* the_system, Screen* the_screen, uint8_t border_width, uint8_t border_height)
{
	uint32_t the_border_control_value;

	if (the_screen == NULL)
	{
		LOG_ERR(("%s %d: passed screen was NULL", __func__, __LINE__));
		goto error;
	}
	
	// set borders
	the_border_control_value = R32(the_screen->vicky_ + BORDER_CTRL_OFFSET_L);
	the_border_control_value = (the_border_control_value & 0xFFFF00FF) | ((uint32_t)border_width <<  8);
	the_border_control_value = (the_border_control_value & 0xFF00FFFF) | ((uint32_t)border_height <<  16);
	R32(the_screen->vicky_ + BORDER_CTRL_OFFSET_L) = the_border_control_value;
	
	// now we need to recalculate how many text cols/rows are visible, because it might have changed
	the_screen->text_cols_vis_ = the_screen->text_mem_cols_ - (border_width * 2);
	the_screen->text_rows_vis_ = the_screen->text_mem_rows_ - (border_height * 2);
		
	return true;
	
error:
	Sys_Exit(&global_system, PARAM_EXIT_ON_ERROR);	// crash early, crash often
	return false;
}



// **** Window management functions *****


//! Add this window to the list of windows and make it the currently active window
//! @param	the_system -- valid pointer to system object
//! @return	Returns false if adding this window would exceed the system's hard cap on the number of available windows
bool Sys_AddToWindowList(System* the_system, Window* the_new_window)
{
	List*	the_new_item;
	int8_t	new_display_order;
	
	if (the_system == NULL)
 	{
		LOG_ERR(("%s %d: passed class object was null", __func__ , __LINE__));
		goto error;
 	}
	
 	if (the_new_window == NULL)
 	{
		LOG_ERR(("%s %d: passed window object was null", __func__ , __LINE__));
		goto error;
 	}

	// are there too many windows already? 
	if (the_system->window_count_ >= SYS_MAX_WINDOWS)
	{
		LOG_ERR(("%s %d: No more windows can be opened!", __func__ , __LINE__));
		return false;
	}

	the_new_item = List_NewItem((void*)the_new_window);
	
	// have any windows been created yet? If not, need to allocate the pointer to the list
	if (the_system->window_count_ < 1)
	{
		// initiate the list of windows
		if ( (the_system->list_windows_ = (List**)calloc(1, sizeof(List*)) ) == NULL)
		{
			LOG_ERR(("%s %d: could not allocate memory to create new list of windows", __func__ , __LINE__));
			goto error;
		}
		LOG_ALLOC(("%s %d:	__ALLOC__	the_system->list_windows_	%p	size	%i", __func__ , __LINE__, the_system->list_windows_, sizeof(List*)));
		
		*the_system->list_windows_ = the_new_item;
	}
	else
	{
		List_AddItem(the_system->list_windows_, the_new_item);
	}
	
	new_display_order = SYS_MAX_WINDOWS;
	Window_SetDisplayOrder(the_new_window, new_display_order);
	
	++the_system->window_count_;
	
	Sys_SetActiveWindow(the_system, the_new_window);

//  	DEBUG_OUT(("%s %d: window count after=%i", __func__, __LINE__, the_system->window_count_));
// 	List_Print(the_system->list_windows_, (void*)&Window_PrintBrief);
	
	return true;
	
error:
	Sys_Exit(&global_system, PARAM_EXIT_ON_ERROR);	// crash early, crash often
	return false;
}


// create the backdrop window for the system
//! @param	the_system -- valid pointer to system object
bool Sys_CreateBackdropWindow(System* the_system)
{
	Screen*				the_screen;
	Window*				the_window;
	NewWinTemplate*		the_win_template;
	static char*		the_win_title = "_backdrop";

 	if (the_system == NULL)
 	{
		LOG_ERR(("%s %d: passed class object was null", __func__ , __LINE__));
		goto error;
 	}

	if ( (the_win_template = Window_GetNewWinTemplate(the_win_title)) == NULL)
	{
		LOG_ERR(("%s %d: Could not get a new window template", __func__ , __LINE__));
		goto error;
	}
		
	the_screen = the_system->screen_[ID_CHANNEL_B];
	
	the_win_template->x_ = 0;
	the_win_template->y_ = 0;
	the_win_template->width_ = the_screen->width_;
	the_win_template->height_ = the_screen->height_;
	the_win_template->min_width_ = the_screen->width_;
	the_win_template->min_height_ = the_screen->height_;
	the_win_template->max_width_ = the_screen->width_;
	the_win_template->max_height_ = the_screen->height_;

	the_win_template->type_ = WIN_BACKDROP;
	the_win_template->is_backdrop_ = true;
	the_win_template->can_resize_ = false;
	
	if ( (the_window = Window_New(the_win_template, &Window_BackdropWinEventHandler)) == NULL)
	{
		DEBUG_OUT(("%s %d: Couldn't instantiate the backdrop window", __func__, __LINE__));
		goto error;
	}

	// make the window visible (Window_New always sets windows to invisible so they don't render before you need them to)
	Window_SetVisible(the_window, true);
	
	return true;
	
error:
	Sys_Exit(&global_system, PARAM_EXIT_ON_ERROR);	// crash early, crash often
	return false;
}


// return the active window
//! @param	the_system -- valid pointer to system object
Window* Sys_GetActiveWindow(System* the_system)
{
 	if (the_system == NULL)
 	{
		LOG_ERR(("%s %d: passed class object was null", __func__ , __LINE__));
		goto error;
 	}
	
	return the_system->active_window_;
	
error:
	Sys_Exit(&global_system, PARAM_EXIT_ON_ERROR);	// crash early, crash often
	return NULL;
}


// return the backdrop window
//! @param	the_system -- valid pointer to system object
Window* Sys_GetBackdropWindow(System* the_system)
{
 	List*	the_item;
 	
 	if (the_system == NULL)
 	{
		LOG_ERR(("%s %d: passed class object was null", __func__ , __LINE__));
		goto error;
 	}
	
	the_item = *(the_system->list_windows_);

	while (the_item != NULL)
	{
		Window*		this_window = (Window*)(the_item->payload_);
		
		if (Window_IsBackdrop(this_window))
		{
			return this_window;
		}

		the_item = the_item->next_item_;
	}
	
	return NULL;
	
error:
	Sys_Exit(&global_system, PARAM_EXIT_ON_ERROR);	// crash early, crash often
	return NULL;
}


// return a reference to the next window in the system's list, excluding backdrop windows
//! @param	the_system -- valid pointer to system object
Window* Sys_GetNextWindow(System* the_system)
{
	Window*		current_window;
	Window*		next_window;
	List*		current_window_item;
	List*		next_window_item;
	bool		ok = false;
	bool		looped = false;

 	if (the_system == NULL)
 	{
		LOG_ERR(("%s %d: passed class object was null", __func__ , __LINE__));
		goto error;
 	}
	
	if (the_system->window_count_ < 1)
	{
		return NULL;
	}
	
	current_window = Sys_GetActiveWindow(the_system);

	DEBUG_OUT(("%s %d: window list:", __func__, __LINE__));
	List_Print(the_system->list_windows_, (void*)&Window_PrintBrief);

	// if no active window (possible on a window close), then use the first window as starting point
	// otherwise, use active window as starting point
	if (current_window == NULL)
	{
		current_window_item = List_GetFirst(the_system->list_windows_);
		LOG_ERR(("%s %d: no active window, first item=%p", __func__ , __LINE__, current_window_item));
		looped = true;
	}
	else
	{
		current_window_item = List_FindThisObject(the_system->list_windows_, (void*)current_window);
	}	

	if (current_window_item == NULL)
	{
		LOG_ERR(("%s %d: couldn't find current window in the list of windows", __func__ , __LINE__));
		goto error;
	}
	
	next_window_item = current_window_item->next_item_;
	
	while (!ok)
	{	
		if (next_window_item == NULL)
		{
			// loop back to start of List
			if (looped == false)
			{
				next_window_item = List_GetFirst(the_system->list_windows_);
				looped = true;
				DEBUG_OUT(("%s %d: going back to first item in window list", __func__ , __LINE__));
			}
			else
			{
				DEBUG_OUT(("%s %d: have already looped once; giving up", __func__ , __LINE__));
				current_window = (Window*)current_window_item->payload_; // there is always ONE window until we do Sys_Destroy()
				return current_window;
			}
		}
		else
		{
			// check for backdrop and skip.
			next_window = (Window*)next_window_item->payload_;
			DEBUG_OUT(("%s %d: backdrop check: this window is '%s'", __func__ , __LINE__, next_window->title_));

			if (next_window->is_backdrop_)
			{
				next_window_item = next_window_item->next_item_;
				DEBUG_OUT(("%s %d: was backdrop", __func__ , __LINE__));
			}
			else
			{
				ok = true;
				DEBUG_OUT(("%s %d: ok reached", __func__ , __LINE__));
			}
		}
	}
	
	return next_window;
	
error:
	Sys_Exit(&global_system, PARAM_EXIT_ON_ERROR);	// crash early, crash often
	return NULL;
}


// return a reference to the previous window in the system's list, excluding backdrop windows
//! @param	the_system -- valid pointer to system object
Window* Sys_GetPreviousWindow(System* the_system)
{
	Window*		current_window;
	Window*		next_window;
	List*		current_window_item;
	List*		next_window_item;
	bool		ok = false;

 	if (the_system == NULL)
 	{
		LOG_ERR(("%s %d: passed class object was null", __func__ , __LINE__));
		goto error;
 	}
	
	if (the_system->window_count_ < 1)
	{
		return NULL;
	}
	
	current_window = Sys_GetActiveWindow(the_system);

	if (current_window == NULL)
	{
		return NULL;
	}

	if (the_system->window_count_ < 2)
	{
		return current_window;
	}
	
	current_window_item = List_FindThisObject(the_system->list_windows_, (void*)current_window);

	if (current_window_item == NULL)
	{
		LOG_ERR(("%s %d: couldn't find current window in the list of windows", __func__ , __LINE__));
		goto error;
	}
	
	next_window_item = current_window_item->prev_item_;
	
	while (!ok)
	{	
		if (next_window_item == NULL)
		{
			// loop back to start of List
			next_window_item = List_GetLast(the_system->list_windows_);
		}
		else
		{
			// check for backdrop and skip.
			next_window = (Window*)next_window_item->payload_;

			if (next_window->is_backdrop_)
			{
				next_window_item = next_window_item->prev_item_;
			}
			else
			{
				ok = true;
			}
		}
	}
	
	return next_window;
	
error:
	Sys_Exit(&global_system, PARAM_EXIT_ON_ERROR);	// crash early, crash often
	return NULL;
}


// Find the Window under the mouse -- accounts for z depth (topmost window will be found)
//! @param	the_system -- valid pointer to system object
//! @param	x -- global horizontal coordinate
//! @param	y -- global vertical coordinate
Window* Sys_GetWindowAtXY(System* the_system, int16_t x, int16_t y)
{
 	List*	the_item;

 	if (the_system == NULL)
 	{
		LOG_ERR(("%s %d: passed class object was null", __func__ , __LINE__));
		goto error;
 	}
	
	if (the_system->window_count_ < 1)
	{
		return NULL;
	}
	
	if (x < 0 || y < 0)
	{
		return NULL;
	}
	
	// LOGIC:
	//   OS/f windows are all known by the system
	//   each window has a display order property set by the system, from low to high being backmost to frontmost
	//   the system also keeps them sorted in its list, with first item being front most
		
	the_item = *(the_system->list_windows_);

	while (the_item != NULL)
	{
 		bool		in_this_win;
		Window*		this_window = (Window*)(the_item->payload_);
		
		in_this_win = General_PointInRect(x, y, this_window->global_rect_);
		
		if (in_this_win)
		{
			DEBUG_OUT(("%s %d: window at %i, %i = '%s'", __func__, __LINE__, x, y, this_window->title_));
			return this_window;
		}

		the_item = the_item->next_item_;
	}
	
	return NULL;
	
error:
	Sys_Exit(&global_system, PARAM_EXIT_ON_ERROR);	// crash early, crash often
	return NULL;
}


//! Set the passed window to the active window, and marks the previously active window as inactive
//! NOTE: This will resort the list of windows to move the (new) active one to the front
//! NOTE: The exception to this is that the backdrop window is never moved in front of other windows
//! @param	the_system -- valid pointer to system object
bool Sys_SetActiveWindow(System* the_system, Window* the_window)
{
	if (the_system == NULL)
 	{
		LOG_ERR(("%s %d: passed class object was null", __func__ , __LINE__));
		goto error;
 	}

	if (the_system->active_window_ != NULL)
	{
		Window_SetActive(the_system->active_window_, false);
	}
	
	// Before making the new window active, survey any windows in front of it, and add their shapes to this window's damage rects so it can fix itself.
	Sys_CollectDamageRects(the_system, the_window);
	
	the_system->active_window_ = the_window;

	Window_SetActive(the_window, true);

	if (the_window->is_backdrop_)
	{
		return true;
	}
	
	//DEBUG_OUT(("%s %d: window list before change of active window:", __func__, __LINE__));
	//List_Print(the_system->list_windows_, (void*)&Window_PrintBrief);
	
	the_window->display_order_ = SYS_WIN_Z_ORDER_NEWLY_ACTIVE;
	List_InitMergeSort(the_system->list_windows_, &Window_CompareDisplayOrder);
	
	//DEBUG_OUT(("%s %d: window list after change of active window:", __func__, __LINE__));
	//List_Print(the_system->list_windows_, (void*)&Window_PrintBrief);
	
	// that changes their linked order, but doesn't renumber their display_order_; need that too
	Sys_RenumberWindows(the_system);
	
	// temp
	Sys_Render(global_system);
	
	return true;
	
error:
	Sys_Exit(&global_system, PARAM_EXIT_ON_ERROR);	// crash early, crash often
	return false;
}


// List-sort compatible function for sorting windows by their display order property
bool Window_CompareDisplayOrder(void* first_payload, void* second_payload)
{
	Window*		win_1 = (Window*)first_payload;
	Window*		win_2 = (Window*)second_payload;

	if (win_1->display_order_ < win_2->display_order_)
	{
		//DEBUG_OUT(("%s %d: win1 ('%s') behind win2 ('%s')", __func__, __LINE__, win_1->title_, win_2->title_));
		return true;
	}
	else
	{
		//DEBUG_OUT(("%s %d: win1 ('%s') in front of win2 ('%s')", __func__, __LINE__, win_1->title_, win_2->title_));
		return false;
	}
}


// remove one window from system's list of windows, and close it
void Sys_CloseOneWindow(System* the_system, Window* the_window)
{
	bool		need_different_active_window = false;
	List*		this_window_item;
	Rectangle	the_new_rect;

 	if (the_system == NULL)
 	{
		LOG_ERR(("%s %d: passed class object was null", __func__ , __LINE__));
		goto error;
 	}
	
	// check that we have a window
	if (the_window == NULL)
	{
		LOG_ERR(("%s %d: passed a null window -- no window to close", __func__ , __LINE__));
		goto error;
	}

	this_window_item = List_FindThisObject(the_system->list_windows_, (void*)the_window);

	if (this_window_item == NULL)
	{
		LOG_ERR(("%s %d: couldn't find current window in the list of windows", __func__ , __LINE__));
		goto error;
	}
	
	// nullify any upcoming events that reference this Window
	EventManager_RemoveEventsForWindow(the_window);
	
	// before destroying the window, calculate and distribute any damage rects that may result from it being removed from screen
	the_new_rect.MinX = -2;
	the_new_rect.MinY = -2;
	the_new_rect.MaxX = -1;
	the_new_rect.MaxY = -1;
	Window_GenerateDamageRects(the_window, &the_new_rect);
	Sys_IssueDamageRects(the_system);
	
	// if this was active window, we'll need to pick a new one after we delete it
	need_different_active_window = (the_system->active_window_ == the_window);
	DEBUG_OUT(("%s %d: closing active window=%i", __func__ , __LINE__, need_different_active_window));
	
	if (need_different_active_window)
	{
		the_system->active_window_ = NULL;
	}
	
	// destroy the window, making sure to set a new active window
	Window_Destroy(&the_window);
	DEBUG_OUT(("%s %d: window destroyed", __func__ , __LINE__));
	--the_system->window_count_;
	List_RemoveItem(the_system->list_windows_, this_window_item);
	LOG_ALLOC(("%s %d:	__FREE__	the_item	%p	size	%i", __func__ , __LINE__, this_window_item, sizeof(List)));
	free(this_window_item);
	this_window_item = NULL;
	
	if (need_different_active_window)
	{
		Window*		the_next_window;

		DEBUG_OUT(("%s %d: trying to get next window...", __func__ , __LINE__));
		the_next_window = Sys_GetNextWindow(the_system);

		if (the_next_window != NULL)
		{
			Sys_SetActiveWindow(the_system, the_next_window); // this also calls system render
		}

		DEBUG_OUT(("%s %d: new active window='%s'", __func__ , __LINE__, the_system->active_window_->title_));		
	}

	// Re-render all windows
	Sys_Render(global_system);		
	
	return;
	
error:
	Sys_Exit(&global_system, PARAM_EXIT_ON_ERROR);	// crash early, crash often
	return;
}


//! Issue damage rects from the Active Window down to each other window in the system so that they can redraw portions of themselves
//! Note: does not call for system re-render
void Sys_IssueDamageRects(System* the_system)
{
	List*		the_item;
	Window*		the_active_window;
	int16_t		num_rects;
	
	if (the_system == NULL)
	{
		LOG_ERR(("%s %d: passed class object was null", __func__ , __LINE__));
		goto error;
	}
	
	if (the_system->list_windows_ == NULL)
	{
		LOG_ERR(("%s %d: the window list was NULL", __func__ , __LINE__));
		goto error;
	}
	
	// LOGIC:
	//   This will be called when the active window has been resized or moved
	//   The goal of this function is to find out which parts of windows behind the active window were previously not exposed, and now are, so they windows can redraw those portions to Screen
	//   Because this function does not actually re-render every window, the order they are processed here does not matter.
	//   Windows are ordered in the window list, by Z order, from back (head) to front (tail)

	the_active_window = Sys_GetActiveWindow(global_system);
	num_rects = the_active_window->damage_count_;
	
	DEBUG_OUT(("%s %d: active window '%s' has %i damage rects", __func__ , __LINE__, the_active_window->title_, num_rects));

	the_item = *(the_system->list_windows_);

	while (the_item != NULL)
	{
		Window*		this_window = (Window*)(the_item->payload_);
		
		//DEBUG_OUT(("%s %d: this_window '%s' has %i clip rects", __func__ , __LINE__, this_window->title_, this_window->clip_count_));

		if (this_window != the_active_window)
		{
			int16_t		i;
			
			for (i = 0; i < num_rects; i++)
			{
				if (Window_AcceptDamageRect(this_window, &the_active_window->damage_rect_[i]) == false)
				{
				}
			}			
		}

		the_item = the_item->next_item_;
	}
	
	return;
	
error:
	Sys_Exit(&global_system, PARAM_EXIT_ON_ERROR);	// crash early, crash often
	return;
}


//! Issue damage rects from the menu down to every other window in the system so that they can redraw portions of themselves when menu closes
//! Note: does not call for system re-render
void Sys_IssueMenuDamageRects(System* the_system)
{
	List*		the_item;
	
	if (the_system == NULL)
	{
		LOG_ERR(("%s %d: passed class object was null", __func__ , __LINE__));
		goto error;
	}
	
	if (the_system->list_windows_ == NULL)
	{
		LOG_ERR(("%s %d: the window list was NULL", __func__ , __LINE__));
		goto error;
	}
	
	// LOGIC:
	//   This will be called when the menu is closed
	//   The goal of this function is to find out which parts of windows behind the menu were covered by the menu and need to be redrawn
	//   Because this function does not actually re-render every window, the order they are processed here does not matter.
	//   Windows are ordered in the window list, by Z order, from back (head) to front (tail)

	the_item = *(the_system->list_windows_);

	while (the_item != NULL)
	{
		Window*		this_window = (Window*)(the_item->payload_);
		
		//DEBUG_OUT(("%s %d: this_window '%s' has %i clip rects", __func__ , __LINE__, this_window->title_, this_window->clip_count_));
		if (Window_AcceptDamageRect(this_window, &the_system->menu_manager_->global_rect_) == false)
		{
			LOG_ERR(("%s %d: Failed to apply menu damage rect to window '%s'", __func__ , __LINE__, this_window->title_));
			//goto error;
		}

		the_item = the_item->next_item_;
	}
	
	return;
	
error:
	Sys_Exit(&global_system, PARAM_EXIT_ON_ERROR);	// crash early, crash often
	return;
}


//! Collect damage rects for a window that is about to be made the active (foremost) window, so it can redraw portions of itself that may have been covered up by other windows
//! Note: does not call for system re-render
void Sys_CollectDamageRects(System* the_system, Window* the_future_active_window)
{
	List*		the_item;
	
	if (the_system == NULL)
	{
		LOG_ERR(("%s %d: passed class object was null", __func__ , __LINE__));
		goto error;
	}
	
	if (the_system->list_windows_ == NULL)
	{
		LOG_ERR(("%s %d: the window list was NULL", __func__ , __LINE__));
		goto error;
	}

	if (the_future_active_window == NULL)
	{
		LOG_ERR(("%s %d: passed window object was null", __func__ , __LINE__));
		goto error;
	}
	
	
	// LOGIC:
	//   This will be called when a non-active window is being brought to the foreground and made the active window
	//   The goal of this function is to find out which parts of that window have been behind the active window and any other windows, so those portions can be redrawn
	//   Windows are ordered in the window list, by Z order, from back (head) to front (tail)
	//   Because this function does not actually re-render every window, the order they are processed here does not matter.
	//     However, we do need to not include damage rects from windows that were under the window being brought forward
	//     This can be accomplished by walking through window list until we hit the window in question, then start collecting from the next window after that
	
	DEBUG_OUT(("%s %d: future active win '%s' has display order of %i", __func__ , __LINE__, the_future_active_window->title_, the_future_active_window->display_order_));

	// LOGIC:
	//   A backdrop window never needs to collect damage rects from a window above it on active/inactive change
	//   That is because no matter what the order is, the backdrop window is always at the very back. 
	//   Windows only need to collect damage rects from windows above them if they can actually get in front of those windows
	if (the_future_active_window->is_backdrop_)
	{
		return;
	}
	
	the_item = *(the_system->list_windows_);

	while (the_item != NULL)
	{
		Window*		this_window = (Window*)(the_item->payload_);
		
		DEBUG_OUT(("%s %d: this_window '%s' has display order of %i", __func__ , __LINE__, this_window->title_, this_window->display_order_));

		if (this_window->display_order_ > the_future_active_window->display_order_)
		{
			if (Window_AcceptDamageRect(the_future_active_window, &this_window->global_rect_) == false)
			{
			}
		}

		the_item = the_item->next_item_;
	}
	
	return;
	
error:
	Sys_Exit(&global_system, PARAM_EXIT_ON_ERROR);	// crash early, crash often
	return;
}




// **** Other GET functions *****


//! @param	the_system -- valid pointer to system object
Theme* Sys_GetTheme(System* the_system)
{
	if (the_system == NULL)
	{
		LOG_ERR(("%s %d: passed class object was null", __func__ , __LINE__));
		goto error;
	}
	
	return the_system->theme_;
	
error:
	Sys_Exit(&global_system, PARAM_EXIT_ON_ERROR);	// crash early, crash often
	return NULL;
}

//! @param	the_system -- valid pointer to system object
Font* Sys_GetSystemFont(System* the_system)
{
	if (the_system == NULL)
	{
		LOG_ERR(("%s %d: passed class object was null", __func__ , __LINE__));
		goto error;
	}
	
	return the_system->system_font_;
	
error:
	Sys_Exit(&global_system, PARAM_EXIT_ON_ERROR);	// crash early, crash often
	return NULL;
}


//! @param	the_system -- valid pointer to system object
Font* Sys_GetAppFont(System* the_system)
{
	if (the_system == NULL)
	{
		LOG_ERR(("%s %d: passed class object was null", __func__ , __LINE__));
		goto error;
	}
	
	return the_system->app_font_;
	
error:
	Sys_Exit(&global_system, PARAM_EXIT_ON_ERROR);	// crash early, crash often
	return NULL;
}


//! @param	the_system -- valid pointer to system object
Screen* Sys_GetScreen(System* the_system, int16_t channel_id)
{
	if (the_system == NULL)
	{
		LOG_ERR(("%s %d: passed class object was null", __func__ , __LINE__));
		goto error;
	}
	
	if (channel_id != ID_CHANNEL_A && channel_id != ID_CHANNEL_B)
	{
		LOG_ERR(("%s %d: passed channel_id (%i) was invalid", __func__ , __LINE__, channel_id));
		goto error;
	}
	
	return the_system->screen_[channel_id];
	
error:
	Sys_Exit(&global_system, PARAM_EXIT_ON_ERROR);	// crash early, crash often
	return NULL;
}


//! @param	the_system -- valid pointer to system object
Menu* Sys_GetMenu(System* the_system)
{
	if (the_system == NULL)
	{
		LOG_ERR(("%s %d: passed class object was null", __func__ , __LINE__));
		goto error;
	}
	
	return the_system->menu_manager_;
	
error:
	Sys_Exit(&global_system, PARAM_EXIT_ON_ERROR);	// crash early, crash often
	return NULL;
}


//! NOTE: Foenix systems only have 1 screen with bitmap graphics, even if the system has 2 screens overall. The bitmap returned will always be from the appropriate channel (A or B).
//! @param	the_system -- valid pointer to system object
Bitmap* Sys_GetScreenBitmap(System* the_system, bitmap_layer the_layer)
{
	if (the_system == NULL)
	{
		LOG_ERR(("%s %d: passed class object was null", __func__ , __LINE__));
		goto error;
	}
	
	if (the_layer > fore_layer)
	{
		LOG_ERR(("%s %d: passed layer (%i) was invalid", __func__ , __LINE__, the_layer));
		goto error;
	}
	
	return the_system->screen_[ID_CHANNEL_B]->bitmap_[the_layer];
	
error:
	Sys_Exit(&global_system, PARAM_EXIT_ON_ERROR);	// crash early, crash often
	return NULL;
}


//! @param	the_system -- valid pointer to system object
EventManager* Sys_GetEventManager(System* the_system)
{
	if (the_system == NULL)
	{
		LOG_ERR(("%s %d: passed class object was null", __func__ , __LINE__));
		goto error;
	}
	
	return the_system->event_manager_;
	
error:
	Sys_Exit(&global_system, PARAM_EXIT_ON_ERROR);	// crash early, crash often
	return NULL;
}





// **** Other SET functions *****

//! @param	the_system -- valid pointer to system object
void Sys_SetSystemFont(System* the_system, Font* the_font)
{
	if (the_system == NULL)
	{
		LOG_ERR(("%s %d: passed class object was null", __func__ , __LINE__));
		goto error;
	}
	
	the_system->system_font_ = the_font;
	
	return;
	
error:
	Sys_Exit(&global_system, PARAM_EXIT_ON_ERROR);	// crash early, crash often
	return;
}


//! @param	the_system -- valid pointer to system object
void Sys_SetAppFont(System* the_system, Font* the_font)
{
	if (the_system == NULL)
	{
		LOG_ERR(("%s %d: passed class object was null", __func__ , __LINE__));
		goto error;
	}
	
	the_system->app_font_ = the_font;
	
	return;
	
error:
	Sys_Exit(&global_system, PARAM_EXIT_ON_ERROR);	// crash early, crash often
	return;
}


//! @param	the_system -- valid pointer to system object
void Sys_SetScreen(System* the_system, int16_t channel_id, Screen* the_screen)
{
	if (the_system == NULL)
	{
		LOG_ERR(("%s %d: passed class object was null", __func__ , __LINE__));
		goto error;
	}
	
	if (channel_id != ID_CHANNEL_A && channel_id != ID_CHANNEL_B)
	{
		LOG_ERR(("%s %d: passed channel_id (%i) was invalid", __func__ , __LINE__, channel_id));
		goto error;
	}
	
	the_system->screen_[channel_id] = the_screen;
	
	return;
	
error:
	Sys_Exit(&global_system, PARAM_EXIT_ON_ERROR);	// crash early, crash often
	return;
}


//! NOTE: Foenix systems only have 1 screen with bitmap graphics, even if the system has 2 screens overall. The bitmap returned will always be from the appropriate channel (A or B).
//! @param	the_system -- valid pointer to system object
void Sys_SetScreenBitmap(System* the_system, Bitmap* the_bitmap, bitmap_layer the_layer)
{
	if (the_system == NULL)
	{
		LOG_ERR(("%s %d: passed class object was null", __func__ , __LINE__));
		goto error;
	}
	
	if (the_bitmap == NULL)
	{
		LOG_WARN(("%s %d: passed bitmap object was null", __func__ , __LINE__));
		goto error;
	}
	
	if (the_layer > fore_layer)
	{
		LOG_ERR(("%s %d: passed layer (%i) was invalid", __func__ , __LINE__, the_layer));
		goto error;
	}
	
	the_system->screen_[ID_CHANNEL_B]->bitmap_[the_layer] = the_bitmap;
	
	Sys_SetVRAMAddr(the_system, the_layer, the_bitmap->addr_);
	
	DEBUG_OUT(("%s %d: layer=%i, bitmap_[the_layer]=%p", __func__, __LINE__, the_layer, the_bitmap->addr_));
	
	return;
	
error:
	Sys_Exit(&global_system, PARAM_EXIT_ON_ERROR);	// crash early, crash often
	return;
}


//! Set the passed theme as the System's current theme
//! Note: this will dispose of the current theme after setting the new one
//! @param	the_system -- valid pointer to system object
//! @return	Returns false on any error condition
bool Sys_SetTheme(System* the_system, Theme* the_theme)
{
	if (the_system == NULL)
	{
		LOG_ERR(("%s %d: passed class object was null", __func__ , __LINE__));
		goto error;
	}
	
	if (the_theme == NULL)
	{
		LOG_WARN(("%s %d: passed theme object was null", __func__ , __LINE__));
		goto error;
	}
	
	if (the_system->theme_ != NULL)
	{
		Theme_Destroy(&the_system->theme_);
	}
	
	the_system->theme_ = the_theme;
	
	Sys_SetSystemFont(the_system, the_theme->control_font_);
	Sys_SetAppFont(the_system, the_theme->icon_font_);
	
	// if a menu has been loaded, have it change its font
	if (the_system->menu_manager_)
	{
		Menu_SetFont(the_system->menu_manager_, the_theme->icon_font_);
	}

	// have all windows update their controls / etc with info from new theme
	if (the_system->window_count_ > 0)
	{
		Sys_UpdateWindowTheme(the_system);
	
		// force re-render
		Sys_Render(the_system);
	}
	
	return true;
	
error:
	Sys_Exit(&global_system, PARAM_EXIT_ON_ERROR);	// crash early, crash often
	return false;
}





// **** xxx functions *****


//! Tell the VICKY to use a different address for the specified bitmap layer
//! @param	the_system -- valid pointer to system object
//! @param	the_bitmap_layer -- 0 or 1, the bitmap layer to get a new address
//! @param	the_address -- The address within the VRAM zone that the bitmap layer should be repointed to
bool Sys_SetVRAMAddr(System* the_system, uint8_t the_bitmap_layer, unsigned char* the_address)
{
	uint32_t			new_vicky_bitmap_vram_value;

return true;

	if (the_system == NULL)
	{
		LOG_ERR(("%s %d: passed class object was null", __func__ , __LINE__));
		goto error;
	}
	
	if (the_bitmap_layer > 1)
	{
		LOG_ERR(("%s %d: passed bitmap layer number (%u) was invalid", __func__ , __LINE__, the_bitmap_layer));
		goto error;
	}
	
	DEBUG_OUT(("%s %d: VICKY VRAM for bitmap layer 0: want to point to bitmap at %p", __func__, __LINE__, the_address));

	//DEBUG_OUT(("%s %d: VICKY VRAM for bitmap layer 0 before change: 0x%x (with offset=0x%x)", __func__, __LINE__, R32(the_system->screen_[ID_CHANNEL_B]->vicky_ + BITMAP_L0_VRAM_ADDR_OFFSET_L), (uint32_t)VRAM_START + R32(the_system->screen_[ID_CHANNEL_B]->vicky_ + BITMAP_L0_VRAM_ADDR_OFFSET_L)));
	DEBUG_OUT(("%s %d: VICKY VRAM for bitmap layer %i before change: %x / %x / %x", __func__, __LINE__, the_bitmap_layer, R8(BITMAP_L0_VRAM_ADDR_L), R8(BITMAP_L0_VRAM_ADDR_M), R8(BITMAP_L0_VRAM_ADDR_H)));
	
	new_vicky_bitmap_vram_value = (uint32_t)the_address - (uint32_t)VRAM_START;		

	DEBUG_OUT(("%s %d: VICKY VRAM for bitmap layer %i about to change to: %p (with offset=%p)", __func__, __LINE__, the_bitmap_layer, P32(new_vicky_bitmap_vram_value), P32(the_address)));

	if (the_bitmap_layer == 0)
	{
		R32(the_system->screen_[ID_CHANNEL_B]->vicky_ + BITMAP_L0_VRAM_ADDR_OFFSET_L) = new_vicky_bitmap_vram_value;
	}
	else
	{
		R32(the_system->screen_[ID_CHANNEL_B]->vicky_ + BITMAP_L1_VRAM_ADDR_OFFSET_L) = new_vicky_bitmap_vram_value;
	}


	
	//DEBUG_OUT(("%s %d: VICKY VRAM for bitmap layer 0 now set to 0x%x (with offset=0x%x)", __func__, __LINE__, R32(the_system->screen_[ID_CHANNEL_B]->vicky_ + BITMAP_L0_VRAM_ADDR_OFFSET_L), (uint32_t)VRAM_START + R32(the_system->screen_[ID_CHANNEL_B]->vicky_ + BITMAP_L0_VRAM_ADDR_OFFSET_L)));
	
	return true;
	
error:
	Sys_Exit(&global_system, PARAM_EXIT_ON_ERROR);	// crash early, crash often
	return false;
}






// **** Render functions *****


//! Render all visible windows
//! NOTE: this will move to a private Sys function later, once event handling is available
//! @param	the_system -- valid pointer to system object
void Sys_Render(System* the_system)
{
	int16_t		num_nodes = 0;
	List*		the_item;

 	if (the_system == NULL)
 	{
		LOG_ERR(("%s %d: passed class object was null", __func__ , __LINE__));
		goto error;
 	}
	
	// LOGIC:
	//   as we do not have regions and layers set up yet, we have to render every single window in its entirely, including the backdrop
	//   therefore, it is critical that the rendering take place in the order of back to front
	//   display order is built into the system's window list: the first item is the foremost, and the last is the backmost
	//   need to render from back of list towards front of list, so they built up over each other in right order.
	
	// have each window (re)render its controls/content/etc to its bitmap, and blit itself to the main screen/backdrop window bitmap
	
	if (the_system->list_windows_ == NULL)
	{
		DEBUG_OUT(("%s %d: the window list was NULL", __func__ , __LINE__));
		goto error;
	}
	
	//List_Print(the_system->list_windows_, (void*)&Window_PrintBrief);
	the_item = List_GetLast(the_system->list_windows_);
	//the_item = *(the_system->list_windows_);

	while (the_item != NULL)
	{
		Window*		this_window = (Window*)(the_item->payload_);
		
		//DEBUG_OUT(("%s %d: rendering window '%s'", __func__ , __LINE__, this_window->title_));
		
		if (Window_IsVisible(this_window) == true)
		{
			++num_nodes;
			Window_Render(this_window);

// 			// blit to screen
// 			Bitmap_Blit(this_window->bitmap_, 0, 0, the_system->screen_[ID_CHANNEL_B]->bitmap_, this_window->x_, this_window->y_, this_window->width_, this_window->height_);
		}

		the_item = the_item->prev_item_;
	}

	//DEBUG_OUT(("%s %d: %i windows rendered out of %i total window", __func__ , __LINE__, num_nodes, the_system->window_count_));
	
	return;
	
error:
	Sys_Exit(&global_system, PARAM_EXIT_ON_ERROR);	// crash early, crash often
	return;
}

