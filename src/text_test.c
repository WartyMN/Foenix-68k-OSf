/*
 * text_test.c
 *
 *  Created on: Feb 19, 2022
 *      Author: micahbly
 */






/*****************************************************************************/
/*                                Includes                                   */
/*****************************************************************************/

// unit testing framework
#include "minunit.h"

// project includes
#include "debug.h"
#include "general.h"
#include "sys.h"

// class being tested
#include "text.h"

// C includes
#include <stdbool.h>

// A2560 includes
#include "a2560k.h"



/*****************************************************************************/
/*                               Definitions                                 */
/*****************************************************************************/



/*****************************************************************************/
/*                               Enumerations                                */
/*****************************************************************************/


/*****************************************************************************/
/*                          File-scoped Variables                            */
/*****************************************************************************/

	static char* text_test_big_string = 
	"\nThe Anecdote\n\nBill Atkinson worked mostly at home, but whenever he made significant progress he rushed in to Apple to show it off to anyone who would appreciate it. This time, he visited the Macintosh offices at Texaco Towers to show off his brand new oval routines in Quickdraw, which were implemented using a really clever algorithm.\n\nBill had added new code to QuickDraw (which was still called LisaGraf at this point) to draw circles and ovals very quickly. That was a bit hard to do on the Macintosh, since the math for circles usually involved taking square roots, and the 68000 processor in the Lisa and Macintosh didn't support floating point operations. But Bill had come up with a clever way to do the circle calculation that only used addition and subtraction, not even multiplication or division, which the 68000 could do, but was kind of slow at.\n\nBill's technique used the fact the sum of a sequence of odd numbers is always the next perfect square (For example, 1 + 3 = 4, 1 + 3 + 5 = 9, 1 + 3 + 5 + 7 = 16, etc). So he could figure out when to bump the dependent coordinate value by iterating in a loop until a threshold was exceeded. This allowed QuickDraw to draw ovals very quickly.\n\nBill fired up his demo and it quickly filled the Lisa screen with randomly-sized ovals, faster than you thought was possible. But something was bothering Steve Jobs. 'Well, circles and ovals are good, but how about drawing rectangles with rounded corners? Can we do that now, too?'\n\n'No, there's no way to do that. In fact it would be really hard to do, and I don't think we really need it'. I think Bill was a little miffed that Steve wasn't raving over the fast ovals and still wanted more.\n\nSteve suddenly got more intense. 'Rectangles with rounded corners are everywhere! Just look around this room!'. And sure enough, there were lots of them, like the whiteboard and some of the desks and tables. Then he pointed out the window. 'And look outside, there's even more, practically everywhere you look!'. He even persuaded Bill to take a quick walk around the block with him, pointing out every rectangle with rounded corners that he could find.\n\n\nWhen Steve and Bill passed a no-parking sign with rounded corners, it did the trick. 'OK, I give up', Bill pleaded. 'I'll see if it's as hard as I thought.' He went back home to work on it.\n\nBill returned to Texaco Towers the following afternoon, with a big smile on his face. His demo was now drawing rectangles with beautifully rounded corners blisteringly fast, almost at the speed of plain rectangles. When he added the code to LisaGraf, he named the new primitive 'RoundRects'. Over the next few months, roundrects worked their way into various parts of the user interface, and soon became indispensable.\n\nThe Code\n\nAuthor: Bill Atkinson\nYear: 1981\n\nQuickDraw is the Macintosh library for creating bit-mapped graphics, which was used by MacPaint and other applications. It consists of a total of 17,101 lines in 36 files, all written in assembler language for the 68000.\n\n               .INCLUDE  GRAFTYPES.TEXT\n;-----------------------------------------------------------\n;\n;\n;     ****   ****   *****   ***   *****   ***\n;     *   *  *   *  *      *   *    *    *   *\n;     *   *  *   *  *      *        *    *\n;     ****   ****   ***    *        *     ***\n;     * *    * *    *      *        *        *\n;     *  *   *  *   *      *   *    *    *   *\n;     *   *  *   *  *****   ***     *     ***\n;\n;\n;  procedures for operating on RoundRects.\n;\n;\n    .PROC StdRRect,4\n    .REF  CheckPic,DPutPicByte,PutPicVerb,PutPicLong,PutPicRect\n    .REF  PutOval,PushVerb,DrawArc\n;---------------------------------------------------------------\n;\n;  PROCEDURE StdRRect(verb: GrafVerb; r: Rect; ovWd,ovHt: INTEGER);\n;\n;  A6 OFFSETS OF PARAMS AFTER LINK:\n;\nPARAMSIZE       .EQU    10\nVERB            .EQU    PARAMSIZE+8-2           ;GRAFVERB\nRECT            .EQU    VERB-4                  ;LONG, ADDR OF RECT\nOVWD            .EQU    RECT-2                  ;WORD\nOVHT            .EQU    OVWD-2                  ;WORD\n            LINK    A6,#0                           ;NO LOCALS\n            MOVEM.L D7/A3-A4,-(SP)                  ;SAVE REGS\n            MOVE.B  VERB(A6),D7                     ;GET VERB\n            JSR     CHECKPIC                        ;SET UP A4,A3 AND CHECK PICSAVE\n            BLE.S   NOTPIC                          ;BRANCH IF NOT PICSAVE\n            MOVE.B  D7,-(SP)                        ;PUSH VERB\n            JSR     PutPicVerb                      ;PUT ADDIONAL PARAMS TO THEPIC\n;\n;  CHECK FOR NEW OVAL SIZE\n;\n            MOVE.L  PICSAVE(A3),A0                  ;GET PICSAVE HANDLE\n            MOVE.L  (A0),A0                         ;DE-REFERENCE PICSAVE\n            MOVE.L  OVHT(A6),D0                     ;GET OVWD AND OVHT\n            CMP.L   PICOVSIZE(A0),D0                ;SAME AS CURRENT OVAL SIZE ?\n            BEQ.S   OVALOK                          ;YES, CONTINUE\n            MOVE.L  D0,PICOVSIZE(A0)                ;NO, UPDATE STATE VARIABLE\n            MOVE.L  D0,-(SP)                        ;PUSH OVSIZE FOR PutPicLong CALL\n            MOVEQ   #$0B,D0\n            JSR     DPutPicByte                     ;PUT OVSIZE OPCODE\n            JSR     PutPicLong                      ;PUT NEW OVAL SIZE DATA\nOVALOK  MOVEQ   #$40,D0                         ;PUT RRECT NOUN IN HI NIBBLE\n            ADD     D7,D0                           ;PUT VERB IN LO NIBBLE\n            MOVE.B  D0,-(SP)                        ;PUSH OPCODE\n            MOVE.L  RECT(A6),-(SP)                  ;PUSH ADDR OF RECT\n            JSR     PutPicRect                      ;PUT OPCODE AND RECTANGLE\nNOTPIC  MOVE.L  RECT(A6),-(SP)                  ;PUSH ADDR OF RECT\n            CLR.B   -(SP)                           ;PUSH HOLLOW = FALSE\n            TST.B   D7                              ;IS VERB FRAME ?\n            BNE.S   DOIT                            ;NO, CONTINUE\n            TST.L   RGNSAVE(A3)                     ;YES, IS RGNSAVE TRUE ?\n            BEQ.S   NOTRGN                          ;NO, CONTINUE\n            MOVE.L  RECT(A6),-(SP)                  ;YES, PUSH ADDR OF RECT\n            MOVE.L  OVHT(A6),-(SP)                  ;PUSH OVWD, OVHT\n            MOVE.L  RGNBUF(A4),-(SP)                ;PUSH RGNBUF\n            PEA     RGNINDEX(A4)                    ;PUSH VAR RGNINDEX\n            PEA     RGNMAX(A4)                      ;PUSH VAR RGNMAX\n            JSR     PutOval                         ;ADD AN OVAL TO THERGN\nNOTRGN  MOVE.B  #1,(SP)                         ;REPLACE, PUSH HOLLOW = TRUE\nDOIT    MOVE.L  OVHT(A6),-(SP)                  ;PUSH OVWD,OVHT\n            JSR     PushVerb                        ;PUSH MODE AND PATTERN\n            CLR     -(SP)                           ;PUSH STARTANGLE = 0\n            MOVE    #360,-(SP)                      ;PUSH ARCANGLE = 360";

	// Exported using VChar64 v0.2.4
	// Total bytes: 2048
	static uint8_t testfont[] = {  
		0xf0,0xf0,0xf0,0xf0,0x0f,0x0f,0x0f,0x0f,0x00,0x00,0xff,0xff,0x00,0x00,0x00,0x00,
		0xff,0xff,0xff,0xff,0xff,0x00,0x00,0xff,0x36,0x7f,0x7f,0x7f,0x3e,0x1c,0x08,0x00,
		0x08,0x1c,0x3e,0x7f,0x3e,0x1c,0x08,0x00,0x18,0x18,0x66,0x66,0x18,0x18,0x3c,0x00,
		0x08,0x1c,0x3e,0x7f,0x7f,0x1c,0x3e,0x00,0x00,0x3c,0x7e,0x7e,0x7e,0x7e,0x3c,0x00,
		0xff,0xc3,0x81,0x81,0x81,0x81,0xc3,0xff,0x00,0x3c,0x7e,0x66,0x66,0x7e,0x3c,0x00,
		0xff,0xc3,0x81,0x99,0x99,0x81,0xc3,0xff,0x00,0x01,0x03,0x07,0x0f,0x1f,0x3f,0x7f,
		0xff,0xfe,0xfc,0xf8,0xf0,0xe0,0xc0,0x80,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0x00,0x00,
		0xff,0xff,0xfc,0xc1,0x89,0xc9,0xc9,0xff,0xe7,0xe7,0xe7,0xe7,0xe7,0xe7,0xe7,0xe7,
		0xf7,0xe3,0xc1,0x80,0x80,0xe3,0xc1,0xff,0xff,0xff,0xff,0x00,0x00,0xff,0xff,0xff,
		0xff,0xff,0xff,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0xff,0xff,0xff,0xff,
		0xff,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0xff,0xff,
		0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xcf,0xcf,0xcf,0xcf,0xcf,0xcf,0xcf,0xcf,
		0xf3,0xf3,0xf3,0xf3,0xf3,0xf3,0xf3,0xf3,0xff,0xff,0xff,0x1f,0x0f,0xc7,0xe7,0xe7,
		0xe7,0xe7,0xe3,0xf0,0xf8,0xff,0xff,0xff,0x00,0x10,0x30,0x7f,0x7f,0x30,0x10,0x00,
		0x00,0x00,0x00,0x00,0x0f,0x0f,0x0f,0x0f,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,
		0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x03,0x07,0x0e,0x1c,0x38,0x70,0xe0,0xc0,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x18,0x18,0x00,0x00,0x18,0x00,
		0x66,0x66,0x66,0x00,0x00,0x00,0x00,0x00,0x66,0x66,0xff,0x66,0xff,0x66,0x66,0x00,
		0x18,0x3e,0x60,0x3c,0x06,0x7c,0x18,0x00,0x62,0x66,0x0c,0x18,0x30,0x66,0x46,0x00,
		0x3c,0x66,0x3c,0x38,0x67,0x66,0x3f,0x00,0x06,0x0c,0x18,0x00,0x00,0x00,0x00,0x00,
		0x0c,0x18,0x30,0x30,0x30,0x18,0x0c,0x00,0x30,0x18,0x0c,0x0c,0x0c,0x18,0x30,0x00,
		0x00,0x66,0x3c,0xff,0x3c,0x66,0x00,0x00,0x00,0x18,0x18,0x7e,0x18,0x18,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x30,0x00,0x00,0x00,0x7e,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x03,0x06,0x0c,0x18,0x30,0x60,0x00,
		0x3c,0x66,0x6e,0x76,0x66,0x66,0x3c,0x00,0x18,0x18,0x38,0x18,0x18,0x18,0x7e,0x00,
		0x3c,0x66,0x06,0x0c,0x30,0x60,0x7e,0x00,0x3c,0x66,0x06,0x1c,0x06,0x66,0x3c,0x00,
		0x06,0x0e,0x1e,0x66,0x7f,0x06,0x06,0x00,0x7e,0x60,0x7c,0x06,0x06,0x66,0x3c,0x00,
		0x3c,0x66,0x60,0x7c,0x66,0x66,0x3c,0x00,0x7e,0x66,0x0c,0x18,0x18,0x18,0x18,0x00,
		0x3c,0x66,0x66,0x3c,0x66,0x66,0x3c,0x00,0x3c,0x66,0x66,0x3e,0x06,0x66,0x3c,0x00,
		0x00,0x00,0x18,0x00,0x00,0x18,0x00,0x00,0x00,0x00,0x18,0x00,0x00,0x18,0x18,0x30,
		0x0e,0x18,0x30,0x60,0x30,0x18,0x0e,0x00,0x00,0x00,0x7e,0x00,0x7e,0x00,0x00,0x00,
		0x70,0x18,0x0c,0x06,0x0c,0x18,0x70,0x00,0x3c,0x66,0x06,0x0c,0x18,0x00,0x18,0x00,
		0x3c,0x66,0x6e,0x6e,0x60,0x62,0x3c,0x00,0x18,0x3c,0x66,0x7e,0x66,0x66,0x66,0x00,
		0x7c,0x66,0x66,0x7c,0x66,0x66,0x7c,0x00,0x3c,0x66,0x60,0x60,0x60,0x66,0x3c,0x00,
		0x78,0x6c,0x66,0x66,0x66,0x6c,0x78,0x00,0x7e,0x60,0x60,0x78,0x60,0x60,0x7e,0x00,
		0x7e,0x60,0x60,0x78,0x60,0x60,0x60,0x00,0x3c,0x66,0x60,0x6e,0x66,0x66,0x3c,0x00,
		0x66,0x66,0x66,0x7e,0x66,0x66,0x66,0x00,0x3c,0x18,0x18,0x18,0x18,0x18,0x3c,0x00,
		0x1e,0x0c,0x0c,0x0c,0x0c,0x6c,0x38,0x00,0x66,0x6c,0x78,0x70,0x78,0x6c,0x66,0x00,
		0x60,0x60,0x60,0x60,0x60,0x60,0x7e,0x00,0x63,0x77,0x7f,0x6b,0x63,0x63,0x63,0x00,
		0x66,0x76,0x7e,0x7e,0x6e,0x66,0x66,0x00,0x3c,0x66,0x66,0x66,0x66,0x66,0x3c,0x00,
		0x7c,0x66,0x66,0x7c,0x60,0x60,0x60,0x00,0x3c,0x66,0x66,0x66,0x66,0x3c,0x0e,0x00,
		0x7c,0x66,0x66,0x7c,0x78,0x6c,0x66,0x00,0x3c,0x66,0x60,0x3c,0x06,0x66,0x3c,0x00,
		0x7e,0x18,0x18,0x18,0x18,0x18,0x18,0x00,0x66,0x66,0x66,0x66,0x66,0x66,0x3c,0x00,
		0x66,0x66,0x66,0x66,0x66,0x3c,0x18,0x00,0x63,0x63,0x63,0x6b,0x7f,0x77,0x63,0x00,
		0x66,0x66,0x3c,0x18,0x3c,0x66,0x66,0x00,0x66,0x66,0x66,0x3c,0x18,0x18,0x18,0x00,
		0x7e,0x06,0x0c,0x18,0x30,0x60,0x7e,0x00,0x3c,0x30,0x30,0x30,0x30,0x30,0x3c,0x00,
		0xc0,0xe0,0x70,0x38,0x1c,0x0e,0x07,0x03,0x3c,0x0c,0x0c,0x0c,0x0c,0x0c,0x3c,0x00,
		0x00,0x18,0x3c,0x7e,0x18,0x18,0x18,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,
		0x00,0x00,0x00,0xe0,0xf0,0x38,0x18,0x18,0x00,0x00,0x3c,0x06,0x3e,0x66,0x3e,0x00,
		0x00,0x60,0x60,0x7c,0x66,0x66,0x7c,0x00,0x00,0x00,0x3c,0x60,0x60,0x60,0x3c,0x00,
		0x00,0x06,0x06,0x3e,0x66,0x66,0x3e,0x00,0x00,0x00,0x3c,0x66,0x7e,0x60,0x3c,0x00,
		0x00,0x0e,0x18,0x3e,0x18,0x18,0x18,0x00,0x00,0x00,0x3e,0x66,0x66,0x3e,0x06,0x7c,
		0x00,0x60,0x60,0x7c,0x66,0x66,0x66,0x00,0x00,0x18,0x00,0x38,0x18,0x18,0x3c,0x00,
		0x00,0x06,0x00,0x06,0x06,0x06,0x06,0x3c,0x00,0x60,0x60,0x6c,0x78,0x6c,0x66,0x00,
		0x00,0x38,0x18,0x18,0x18,0x18,0x3c,0x00,0x00,0x00,0x66,0x7f,0x7f,0x6b,0x63,0x00,
		0x00,0x00,0x7c,0x66,0x66,0x66,0x66,0x00,0x00,0x00,0x3c,0x66,0x66,0x66,0x3c,0x00,
		0x00,0x00,0x7c,0x66,0x66,0x7c,0x60,0x60,0x00,0x00,0x3e,0x66,0x66,0x3e,0x06,0x06,
		0x00,0x00,0x7c,0x66,0x60,0x60,0x60,0x00,0x00,0x00,0x3e,0x60,0x3c,0x06,0x7c,0x00,
		0x00,0x18,0x7e,0x18,0x18,0x18,0x0e,0x00,0x00,0x00,0x66,0x66,0x66,0x66,0x3e,0x00,
		0x00,0x00,0x66,0x66,0x66,0x3c,0x18,0x00,0x00,0x00,0x63,0x6b,0x7f,0x3e,0x36,0x00,
		0x00,0x00,0x66,0x3c,0x18,0x3c,0x66,0x00,0x00,0x00,0x66,0x66,0x66,0x3e,0x0c,0x78,
		0x00,0x00,0x7e,0x0c,0x18,0x30,0x7e,0x00,0x18,0x18,0x18,0xff,0xff,0x18,0x18,0x18,
		0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0x18,0x18,0x18,0xff,0xff,0x00,0x00,0x00,
		0x0c,0x12,0x30,0x7c,0x30,0x62,0xfc,0x00,0x01,0x03,0x06,0x6c,0x78,0x70,0x60,0x00,
		0xff,0xff,0xff,0xe0,0xe0,0xe7,0xe7,0xe7,0xe7,0xe7,0xe7,0x00,0x00,0xff,0xff,0xff,
		0xff,0xff,0xff,0x00,0x00,0xe7,0xe7,0xe7,0x3f,0x3f,0xcf,0xcf,0x3f,0x3f,0xcf,0xcf,
		0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f,
		0xf8,0xf8,0xf8,0xf8,0xf8,0xf8,0xf8,0xf8,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,
		0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xf0,0xf0,0xf0,0xf0,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xe7,0xe7,0xc7,0x0f,0x1f,0xff,0xff,0xff,
		0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x00,0x00,0x3f,0x1f,0x8f,0xc7,0xe3,0xf1,0xf8,0xfc,
		0xfc,0xf8,0xf1,0xe3,0xc7,0x8f,0x1f,0x3f,0x00,0x00,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,
		0x00,0x00,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0x0f,0x0f,0x0f,0x0f,0xf0,0xf0,0xf0,0xf0,
		0x00,0x00,0x00,0x00,0xf0,0xf0,0xf0,0xf0,0xc9,0x80,0x80,0x80,0xc1,0xe3,0xf7,0xff,
		0x9f,0x9f,0x9f,0x9f,0x9f,0x9f,0x9f,0x9f,0xff,0xff,0xff,0xf8,0xf0,0xe3,0xe7,0xe7,
		0x3c,0x18,0x81,0xc3,0xc3,0x81,0x18,0x3c,0x0f,0x0f,0x0f,0x0f,0x00,0x00,0x00,0x00,
		0xe7,0xe7,0x99,0x99,0xe7,0xe7,0xc3,0xff,0xf9,0xf9,0xf9,0xf9,0xf9,0xf9,0xf9,0xf9,
		0xf7,0xe3,0xc1,0x80,0xc1,0xe3,0xf7,0xff,0xe7,0xe7,0xe7,0x00,0x00,0xe7,0xe7,0xe7,
		0xc0,0xc0,0x30,0x30,0xc0,0xc0,0x30,0x30,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,
		0x18,0x18,0x18,0x1f,0x1f,0x18,0x18,0x18,0x00,0x80,0xc0,0xe0,0xf0,0xf8,0xfc,0xfe,
		0x18,0x18,0x18,0xff,0xff,0x18,0x18,0x18,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,
		0xff,0x7f,0x3f,0x1f,0x0f,0x07,0x03,0x01,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,
		0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,
		0xff,0xff,0xff,0xff,0x33,0x33,0xcc,0xcc,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,
		0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xe7,0xe7,0xe7,0xe0,0xe0,0xe7,0xe7,0xe7,
		0xff,0xff,0xff,0xff,0xf0,0xf0,0xf0,0xf0,0xe7,0xe7,0xe7,0xe0,0xe0,0xff,0xff,0xff,
		0xff,0xff,0xff,0x07,0x07,0xe7,0xe7,0xe7,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,
		0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc,0x33,0x99,0xcc,0x66,0x33,0x99,0xcc,0x66,
		0xcc,0xcc,0x33,0x33,0xcc,0xcc,0x33,0x33,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,
		0x18,0x18,0x18,0xf8,0xf8,0x18,0x18,0x18,0x00,0x00,0x00,0xff,0xff,0x18,0x18,0x18,
		0xe7,0xe7,0xe7,0x07,0x07,0xe7,0xe7,0xe7,0xe0,0xe0,0xe0,0xe0,0xe0,0xe0,0xe0,0xe0,
		0x00,0x00,0x00,0xff,0xff,0x00,0x00,0x00,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,
		0xe7,0xe7,0xe7,0xe7,0xe7,0xe7,0xe7,0xe7,0xff,0xff,0xff,0xff,0x0f,0x0f,0x0f,0x0f,
		0xf0,0xf0,0xf0,0xf0,0xff,0xff,0xff,0xff,0xe7,0xe7,0xe7,0x07,0x07,0xff,0xff,0xff,
		0x0f,0x0f,0x0f,0x0f,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0xf8,0xf8,0x18,0x18,0x18,
		0x18,0x18,0x18,0x1f,0x1f,0x00,0x00,0x00,0xc0,0xc0,0x30,0x30,0xc0,0xc0,0x30,0x30,
		0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x00,0x00,0x00,0xff,0xff,0x00,0x00,0x00,
		0x00,0x00,0x00,0xff,0xff,0x00,0x00,0x00,0x00,0xff,0xff,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0xff,0xff,0x00,0x00,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x34,
		0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,
		0x18,0x18,0x1c,0x0f,0x07,0x00,0x00,0x00,0x18,0x18,0x38,0xf0,0xe0,0x00,0x00,0x00,
		0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,
		0xff,0xff,0x03,0x03,0x03,0x03,0x03,0x03,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x00,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,
		0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x00,0x00,0x00,0x07,0x0f,0x1c,0x18,0x18,
		0xc3,0xe7,0x7e,0x3c,0x3c,0x7e,0xe7,0xc3,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,
		0x00,0x00,0x00,0x00,0xcc,0xcc,0x33,0x33,0x18,0x18,0x18,0xf8,0xf8,0x00,0x00,0x00,
		0x00,0x00,0x00,0x1f,0x1f,0x18,0x18,0x18,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
		0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,
		0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,
		0x00,0x00,0x03,0x3e,0x76,0x36,0x36,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,
		0xcc,0xcc,0x33,0x33,0xcc,0xcc,0x33,0x33,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,
		0x00,0x00,0x00,0x00,0xcc,0xcc,0x33,0x33,0xcc,0x99,0x33,0x66,0xcc,0x99,0x33,0x66,
		0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x18,0x18,0x18,0x1f,0x1f,0x18,0x18,0x18,
		0x00,0x00,0x00,0x00,0x0f,0x0f,0x0f,0x0f,0x18,0x18,0x18,0x1f,0x1f,0x00,0x00,0x00,
		0x00,0x00,0x00,0xf8,0xf8,0x18,0x18,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,
		0x00,0x00,0x00,0x1f,0x1f,0x18,0x18,0x18,0x18,0x18,0x18,0xff,0xff,0x00,0x00,0x00,
		0x00,0x00,0x00,0xff,0xff,0x18,0x18,0x18,0x18,0x18,0x18,0xf8,0xf8,0x18,0x18,0x18,
		0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xe0,0xe0,0xe0,0xe0,0xe0,0xe0,0xe0,0xe0,
		0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,
		0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,
		0x03,0x03,0x03,0x03,0x03,0x03,0xff,0xff,0x00,0x00,0x00,0x00,0xf0,0xf0,0xf0,0xf0,
		0x0f,0x0f,0x0f,0x0f,0x00,0x00,0x00,0x00,0x18,0x18,0x18,0xf8,0xf8,0x00,0x00,0x00,
		0xf0,0xf0,0xf0,0xf0,0x00,0x00,0x00,0x00,0xf0,0xf0,0xf0,0xf0,0x0f,0x0f,0x0f,0x0f,
	};
	
	static uint8_t*	the_new_font_data = testfont;


/*****************************************************************************/
/*                             Global Variables                              */
/*****************************************************************************/

System*			global_system;


/*****************************************************************************/
/*                       Private Function Prototypes                         */
/*****************************************************************************/

// test using sys_kbd_scancode() instead of a channel driver - TEMP - BAD
bool keyboard_test_2(void);

// test using channel driver - TEMP - BAD
bool keyboard_test(void);


/*****************************************************************************/
/*                       Private Function Definitions                        */
/*****************************************************************************/


// test using sys_kbd_scancode() instead of a channel driver - TEMP - BAD
bool keyboard_test_2(void)
{
	// LOGIC: 
	//   page 34 of FoenixMCP Manual
	//   try a simple loop that just keeps banging on the keyboard scanner
	
	//   keys return their keycode when pushed, and their keyboard + 128 when released.
	
	int16_t		the_code;
	
	do
	{
		the_code = sys_kbd_scancode();
		
		if (the_code > 127)
		{
			DEBUG_OUT(("%s %d: key released: code=%u, keycode=%u", __func__, __LINE__, the_code, the_code & 0xF0));
			// handle_event_key_up()
			
		}
		else
		{
			DEBUG_OUT(("%s %d: key pressed: code=%u", __func__, __LINE__, the_code));
		}
		
	} while (the_code != 0);
	
	return true;
}


// test using channel driver - TEMP - BAD
bool keyboard_test(void)
{
	int16_t					bytes_read = 0;
	int16_t					the_channel_id;
	bool					stop = false;
	int16_t					y = 30;
	
 	// open keyboard console for reading. Console is on device 0 and 1. 
 	//the_channel_id = sys_chan_open(the_device_id, (unsigned char*)"", 1);	
	the_channel_id=0; // 0=console. pjw says it's not necessary to open console, it comes pre-opened. 
	
	sys_chan_flush(the_channel_id);
	
	DEBUG_OUT(("%s %d: Flushed console channel", __func__, __LINE__));

	if ( ((sys_chan_status(the_channel_id) & CDEV_STAT_ERROR) == 1) )
	{
		DEBUG_OUT(("%s %d: channel status had error (0x01)", __func__, __LINE__));
		return false;
	}
	
	// read and type characters to screen until there is an channel error, or the char typed is tab
	//while ( ((sys_chan_status(the_channel_id) & CDEV_STAT_ERROR) == 0) && !stop)
	while ( !stop)
	{
		unsigned char	the_char;
		
		the_char = sys_chan_read_b(the_channel_id);
		bytes_read++;
		
		if (the_char == '\t' || the_char == 'q')
		{
			stop = true;
		}
		else
		{
			Text_SetCharAtXY(global_system->screen_[ID_CHANNEL_B], bytes_read, 40, the_char);
		}
	}
		
	return true;
}



/*****************************************************************************/
/*                        MinUnit Function Defintions                        */
/*****************************************************************************/



void test_setup(void)	// this is called EVERY test
{
// 	foo = 7;
// 	bar = 4;
// 	
}


void test_teardown(void)	// this is called EVERY test
{

}



MU_TEST(test_block_copy)
{
	char*	buffer1;
	char*	buffer2;

	// these test pass, but visually, copying from screen A to B or vice versa doesn't work well right now
	// these reason has to do with different # of columns/rows. 
	// TODO: re-write these tests to copy to an off-screen location. have a different offscreen buffer with some preprerated text. copy that to screen. then copy original buffer back to screen. 
	// TODO: if function is necessary, device a smarter, dedicated buffer to screen copy that accounts for width of copied buffer and width of target screen. 
		
	// Failure: could not alloc space for screen buffer 1
	mu_check( (buffer1 = (char*)calloc(global_system->screen_[ID_CHANNEL_A]->text_mem_cols_ * global_system->screen_[ID_CHANNEL_A]->text_mem_rows_, sizeof(char)) ) != NULL );
 	// Failure: could not alloc space for screen buffer 2
	mu_check( (buffer2 = (char*)calloc(global_system->screen_[ID_CHANNEL_A]->text_mem_cols_ * global_system->screen_[ID_CHANNEL_A]->text_mem_rows_, sizeof(char)) ) != NULL );

	
	// copy text on channel B, to off-screen buffer 1	
	// Failure: Could not copy chan B char to buffer 1
	mu_check( Text_CopyScreen(global_system->screen_[ID_CHANNEL_B], buffer1, SCREEN_COPY_FROM_SCREEN, SCREEN_FOR_TEXT_CHAR == true) );
 
 	// copy text on channel A, to off-screen buffer 2
 	// Failure: Could not copy chan A char to buffer 2
	mu_check( Text_CopyScreen(global_system->screen_[ID_CHANNEL_A], buffer2, SCREEN_COPY_FROM_SCREEN, SCREEN_FOR_TEXT_CHAR == true) );
 	
 	// copy text in offscreen buffer 1, to channel A
 	// Failure: Could not copy buffer1 to chan A char
	mu_check( Text_CopyScreen(global_system->screen_[ID_CHANNEL_A], buffer1, SCREEN_COPY_TO_SCREEN, SCREEN_FOR_TEXT_CHAR == true) );
 
 	// copy text in offscreen buffer 2, to channel B
	// Failure: Could not copy buffer2 to chan B char
	mu_check( Text_CopyScreen(global_system->screen_[ID_CHANNEL_B], buffer2, SCREEN_COPY_TO_SCREEN, SCREEN_FOR_TEXT_CHAR == true) );
 	
 	// copy attr on channel B, to off-screen buffer 1	
	// Failure: Could not copy chan B attr to buffer 1
	mu_check( Text_CopyScreen(global_system->screen_[ID_CHANNEL_B], buffer1, SCREEN_COPY_FROM_SCREEN, SCREEN_FOR_TEXT_ATTR == true) );
 
 	// copy attr on channel A, to off-screen buffer 2
	// Failure: Could not copy chan A attr to buffer 2
	mu_check( Text_CopyScreen(global_system->screen_[ID_CHANNEL_A], buffer2, SCREEN_COPY_FROM_SCREEN, SCREEN_FOR_TEXT_ATTR == true) );
 	
 	// copy attr in offscreen buffer 1, to channel A
	// Failure: Could not copy buffer1 to chan A attr
	mu_check( Text_CopyScreen(global_system->screen_[ID_CHANNEL_A], buffer1, SCREEN_COPY_TO_SCREEN, SCREEN_FOR_TEXT_ATTR == true) );
 
 	// copy attr in offscreen buffer 2, to channel B
	// Failure: Could not copy buffer2 to chan B attr
	mu_check( Text_CopyScreen(global_system->screen_[ID_CHANNEL_B], buffer2, SCREEN_COPY_TO_SCREEN, SCREEN_FOR_TEXT_ATTR == true) );


	// copy text on channel B, to off-screen buffer 1	
	// Failure: Could not copy chan B char to buffer 1
	mu_check( Text_CopyCharMemFromScreen(global_system->screen_[ID_CHANNEL_B], buffer1) == true );

	// copy text on channel A, to off-screen buffer 2
	// Failure: Could not copy chan A char to buffer 2
	mu_check( Text_CopyCharMemFromScreen(global_system->screen_[ID_CHANNEL_A], buffer2) == true );
	
	// copy text in offscreen buffer 1, to channel A
	// Failure: Could not copy buffer1 to chan A char
	mu_check( Text_CopyCharMemToScreen(global_system->screen_[ID_CHANNEL_A], buffer1) == true );

	// copy text in offscreen buffer 2, to channel B
	// Failure: Could not copy buffer2 to chan B char
	mu_check( Text_CopyCharMemToScreen(global_system->screen_[ID_CHANNEL_B], buffer2) == true );
	
	// copy attr on channel B, to off-screen buffer 1	
	// Failure: Could not copy chan B attr to buffer 1
	mu_check( Text_CopyAttrMemFromScreen(global_system->screen_[ID_CHANNEL_B], buffer1) == true );

	// copy attr on channel A, to off-screen buffer 2
	// Failure: Could not copy chan A attr to buffer 2
	mu_check( Text_CopyAttrMemFromScreen(global_system->screen_[ID_CHANNEL_A], buffer2) == true );
	
	// copy attr in offscreen buffer 1, to channel A
	// Failure: Could not copy buffer1 to chan A attr
	mu_check( Text_CopyAttrMemToScreen(global_system->screen_[ID_CHANNEL_A], buffer1) == true );

	// copy attr in offscreen buffer 2, to channel B
	// Failure: Could not copy buffer2 to chan B attr
	mu_check( Text_CopyAttrMemToScreen(global_system->screen_[ID_CHANNEL_B], buffer2) == true );
	
	free(buffer1);
	free(buffer2);
}


MU_TEST(test_block_copy_box)
{
	int16_t			x;
	int16_t			y;
	int16_t			h_line_len;
	int16_t			v_line_len;
	char*			buffer1;
	char*			buffer2;
	
	x = 45;
	y = 4;
	h_line_len = 6;
	v_line_len = 6;
	
	// get out 2 buffers that are the full size of the screens. this block copy is designed to use same offsets as a normal sized screen. 
	// Failure: could not alloc space for screen buffer 1
	mu_check( (buffer1 = (char*)calloc(global_system->screen_[ID_CHANNEL_A]->text_mem_cols_ * global_system->screen_[ID_CHANNEL_A]->text_mem_rows_, sizeof(char)) ) != NULL );
	// Failure: could not alloc space for screen buffer 2
	mu_check( (buffer2 = (char*)calloc(global_system->screen_[ID_CHANNEL_A]->text_mem_cols_ * global_system->screen_[ID_CHANNEL_A]->text_mem_rows_, sizeof(char)) ) != NULL );
	
	// copy text on channel B, to off-screen buffer 1	
	// Failure: Could not copy box of chan B char to buffer 1
	mu_check( Text_CopyMemBox(global_system->screen_[ID_CHANNEL_B], buffer1, x, y, x+h_line_len, y+v_line_len, SCREEN_COPY_FROM_SCREEN, SCREEN_FOR_TEXT_CHAR == true) );

	// copy text on channel A, to off-screen buffer 2
	// Failure: Could not copy box of chan A char to buffer 2
	mu_check( Text_CopyMemBox(global_system->screen_[ID_CHANNEL_A], buffer2, 0, 0, 71, 8, SCREEN_COPY_FROM_SCREEN, SCREEN_FOR_TEXT_CHAR == true) );
	
	// copy text in offscreen buffer 1, to channel A
	// Failure: Could not copy box of buffer1 to chan A char
	mu_check( Text_CopyMemBox(global_system->screen_[ID_CHANNEL_A], buffer1, x, y, x+h_line_len, y+v_line_len, SCREEN_COPY_TO_SCREEN, SCREEN_FOR_TEXT_CHAR == true) );

	// copy text in offscreen buffer 2, to channel B
	// Failure: Could not copy box of buffer2 to chan B char
	mu_check( Text_CopyMemBox(global_system->screen_[ID_CHANNEL_B], buffer2, 0, 0, 71, 8, SCREEN_COPY_TO_SCREEN, SCREEN_FOR_TEXT_CHAR == true) );
	
	// copy attr on channel B, to off-screen buffer 1	
	// Failure: Could not copy box of chan B attr to buffer 1
	mu_check( Text_CopyMemBox(global_system->screen_[ID_CHANNEL_B], buffer1, x, y, x+h_line_len, y+v_line_len, SCREEN_COPY_FROM_SCREEN, SCREEN_FOR_TEXT_ATTR == true) );

	// copy attr on channel A, to off-screen buffer 2
	// Failure: Could not copy box of chan A attr to buffer 2
	mu_check( Text_CopyMemBox(global_system->screen_[ID_CHANNEL_A], buffer2, 0, 0, 71, 8, SCREEN_COPY_FROM_SCREEN, SCREEN_FOR_TEXT_ATTR == true) );
	
	// copy attr in offscreen buffer 1, to channel A
	// Failure: Could not copy box of buffer1 to chan A attr
	mu_check( Text_CopyMemBox(global_system->screen_[ID_CHANNEL_A], buffer1, x, y, x+h_line_len, y+v_line_len, SCREEN_COPY_TO_SCREEN, SCREEN_FOR_TEXT_ATTR == true) );

	// copy attr in offscreen buffer 2, to channel B
 	// Failure: Could not copy box of buffer2 to chan B attr
	mu_check( Text_CopyMemBox(global_system->screen_[ID_CHANNEL_B], buffer2, 0, 0, 71, 8, SCREEN_COPY_TO_SCREEN, SCREEN_FOR_TEXT_ATTR == true) );
		
	free(buffer1);
	free(buffer2);
}


MU_TEST(test_fill_text)
{
	// Failure: Could not fill character memory in channel A
	mu_check( Text_FillCharMem(global_system->screen_[ID_CHANNEL_A], 'Z') );
	// Failure: Could not fill character memory in channel B
	mu_check( Text_FillCharMem(global_system->screen_[ID_CHANNEL_B], 4) );
	// 4 = diamond. good mix of fore/back color
	
	// bad values
	// Failure: Text_FillCharMem accepted bad parameter
	mu_check( Text_FillCharMem(NULL, 4) == false );
}


MU_TEST(test_fill_attr)
{
	// Failure: Could not fill attribute memory in channel A
	mu_check( Text_FillAttrMem(global_system->screen_[ID_CHANNEL_A], 127) );
	// Failure: Could not fill attribute memory in channel B
	mu_check( Text_FillAttrMem(global_system->screen_[ID_CHANNEL_B], 148) );

	// illegal values
	// Failure: Text_FillAttrMem accepted bad parameter
	mu_check( Text_FillAttrMem(NULL, 148) == false );
	
	
		// 31=black on white
		// 64=dark blue on black
		// 96=dark cyan on black
		// 112=medium gray on black
		// 128=medium gray on black
		// 138=black on light green
		// 139=black on bright yellow
		// 140=gray? on medium blue
		// 141=gray? on pink
		// 142=gray? on light cyan

		// 143=black/gray? on white
		// 15=black on white
	
		// 144=red on black
		// 16=dark red on black
	
		// 145=light red on dark red
		// 17=dark red on dark red
	
		// 146=light red on medium green
		// 18=dark red on medium green
	
		// 147=light red on olive
		// 19=dark red on medium green?
	
		// 148=light red on dark blue
}


MU_TEST(test_fill_box)
{
	// bad values
	mu_check( Text_FillBoxSlow(global_system->screen_[ID_CHANNEL_A], 0, 6, 15, 8, CH_DIAG_R1, COLOR_BLACK, BG_COLOR_BRIGHT_WHITE, CHAR_AND_ATTR) == true );
	mu_check( Text_FillBoxSlow(global_system->screen_[ID_CHANNEL_A], 21, 5, 39, 7, CH_HFILLC_UP_8, COLOR_RED, COLOR_BRIGHT_RED, CHAR_AND_ATTR) == true );
	mu_check( Text_FillBox(global_system->screen_[ID_CHANNEL_A], 3, 6, 67, 50, CH_HDITH_8, COLOR_BRIGHT_GREEN, COLOR_GREEN) == true );
	mu_check( Text_FillBox(global_system->screen_[ID_CHANNEL_B], 21, 21, 40, 40, CH_HDITH_8, COLOR_BRIGHT_YELLOW, COLOR_YELLOW) == true );

	// bad values
	// Text_FillBoxSlow accepts an illegal screen ID?
	mu_check( Text_FillBoxSlow(NULL, 0, 6, 15, 8, CH_DIAG_R1, COLOR_BRIGHT_MAGENTA, COLOR_CYAN, CHAR_AND_ATTR) == false );
	// Text_FillBoxSlow accepts an illegal x coord?
	mu_check( Text_FillBox(global_system->screen_[ID_CHANNEL_B], -67, 6, 72, 30, CH_HDITH_8, COLOR_BRIGHT_BLUE, COLOR_BLUE) == false );
	// Text_FillBoxSlow accepts an illegal x coord?
	mu_check( Text_FillBox(global_system->screen_[ID_CHANNEL_B], 32767, 6, 72, 30, CH_HDITH_8, COLOR_BRIGHT_BLUE, COLOR_BLUE) == false );
	// Text_FillBoxSlow accepts an illegal y coord?
	mu_check( Text_FillBox(global_system->screen_[ID_CHANNEL_B], 5, -6, 72, 30, CH_HDITH_8, COLOR_BRIGHT_BLUE, COLOR_BLUE) == false );
	// Text_FillBoxSlow accepts an illegal y coord?
	mu_check( Text_FillBox(global_system->screen_[ID_CHANNEL_B], 5, 6000, 72, 30, CH_HDITH_8, COLOR_BRIGHT_BLUE, COLOR_BLUE) == false );
}


MU_TEST(test_invert_box)
{
	int32_t	i;
	
	for (i = 0; i < 999; i++)
	{
		// Failure: Could not invert color of a box
		mu_check( Text_InvertBox(global_system->screen_[ID_CHANNEL_B], 0, 6, 15, 8) );
	}
	
	// Failure: Could not invert color of a box
	mu_check( Text_InvertBox(global_system->screen_[ID_CHANNEL_B], 50, 13, 71, 16) );

	// bad values
	// Failure: Text_InvertBox accepted an illegal screen ID
	mu_check( Text_InvertBox(NULL, 50, 13, 71, 16) == false );
	// Failure: Text_InvertBox accepted illegal rect coordinates
	mu_check( Text_InvertBox(global_system->screen_[ID_CHANNEL_B], 50, 13, 1500, 16) == false );
	// Failure: Text_InvertBox accepted illegal rect coordinates
	mu_check( Text_InvertBox(global_system->screen_[ID_CHANNEL_B], 71, 16, 50, 10) == false );

}


MU_TEST(test_font_overwrite)
{
	// Failure: Could not replace font data for channel A
	mu_check( Text_UpdateFontData(global_system->screen_[ID_CHANNEL_A], (char*)0x000000) );
	// Failure: Could not replace font data for channel B
	mu_check( Text_UpdateFontData(global_system->screen_[ID_CHANNEL_B], (char*)0x000000) );

	// bad values
	// Failure: Text_UpdateFontData accepted an illegal screen ID
	mu_check( Text_UpdateFontData(NULL, (char*)0x000000) == false );
}


MU_TEST(test_show_font)
{
	// Failure: Could not show font chars for channel A
	mu_check( Text_ShowFontChars(global_system->screen_[ID_CHANNEL_A], 10) );
	// Failure: Could not show font chars for channel B
	mu_check( Text_ShowFontChars(global_system->screen_[ID_CHANNEL_B], 10) );

	// bad values
	// Failure:  accepted an illegal screen ID
	mu_check( Text_ShowFontChars(NULL, 10) == false );
}


//test char placement
MU_TEST(test_char_placement)
{
	int16_t		x;
	int16_t		y;
	
	for (y = 4; y < 40; y = y+2)
	{
		for (x = 0; x < 50; x++)
		{
			// Failure: text char placement failed
			mu_check( Text_SetCharAtXY(global_system->screen_[ID_CHANNEL_A], x, y, 'X') );
		}
	}
	
	// bad values
 	// Failure: Text_SetCharAtXY accepted illegal screen ID
	mu_check( Text_SetCharAtXY(NULL, 0, 0, 'X') == false );
	// Failure: Text_SetCharAtXY accepted illegal coordinates
	mu_check( Text_SetCharAtXY(global_system->screen_[ID_CHANNEL_A], -1, 3, 'b') == false );
	// Failure: Text_SetCharAtXY accepted illegal coordinates
	mu_check( Text_SetCharAtXY(global_system->screen_[ID_CHANNEL_A], -1, -1, 'c') == false );
	// Failure: Text_SetCharAtXY accepted illegal coordinates
	mu_check( Text_SetCharAtXY(global_system->screen_[ID_CHANNEL_A], 0, -1, 'd') == false );
	// Failure: Text_SetCharAtXY accepted illegal coordinates
	mu_check( Text_SetCharAtXY(global_system->screen_[ID_CHANNEL_A], 500, 4, 'e') == false );
	// Failure: Text_SetCharAtXY accepted illegal coordinates
	mu_check( Text_SetCharAtXY(global_system->screen_[ID_CHANNEL_A], 500, 500, 'f') == false );
	// Failure: Text_SetCharAtXY accepted illegal coordinates
	mu_check( Text_SetCharAtXY(global_system->screen_[ID_CHANNEL_A], 0, 500, 'g') == false );
}


// char and color writing
MU_TEST(test_char_and_attr_writing)
{
// same story here: 4 or so works ok, add more, and it's likely to crash. 
 	// Failure: Text_SetCharAndColorAtXY failed
	mu_check( Text_SetCharAndColorAtXY(global_system->screen_[ID_CHANNEL_B], 0, 4, 33, FG_COLOR_BLACK, BG_COLOR_GRAY) == true );
 	// Failure: Text_SetCharAndColorAtXY failed
	mu_check( Text_SetCharAndColorAtXY(global_system->screen_[ID_CHANNEL_B], 1, 4, 34, FG_COLOR_RED, BG_COLOR_GRAY) == true );
 	// Failure: Text_SetCharAndColorAtXY failed
	mu_check( Text_SetCharAndColorAtXY(global_system->screen_[ID_CHANNEL_B], 2, 4, 35, FG_COLOR_BRIGHT_GREEN, BG_COLOR_GRAY) == true );
 	// Failure: Text_SetCharAndColorAtXY failed
	mu_check( Text_SetCharAndColorAtXY(global_system->screen_[ID_CHANNEL_B], 3, 4, 36, FG_COLOR_BRIGHT_BLUE, BG_COLOR_GRAY) == true );
 	// Failure: Text_SetCharAndColorAtXY failed
	mu_check( Text_SetCharAndColorAtXY(global_system->screen_[ID_CHANNEL_B], 4, 4, 37, FG_COLOR_WHITE, BG_COLOR_GRAY) == true );
 	// Failure: Text_SetCharAndColorAtXY failed
	mu_check( Text_SetCharAndColorAtXY(global_system->screen_[ID_CHANNEL_B], 5, 4, 38, FG_COLOR_WHITE, BG_COLOR_GRAY) == true );
 	// Failure: Text_SetCharAndColorAtXY failed
	mu_check( Text_SetCharAndColorAtXY(global_system->screen_[ID_CHANNEL_B], 6, 4, 39, FG_COLOR_BRIGHT_MAGENTA, BG_COLOR_GRAY) == true );
 	// Failure: Text_SetCharAndColorAtXY failed
	mu_check( Text_SetCharAndColorAtXY(global_system->screen_[ID_CHANNEL_B], 7, 4, 40, FG_COLOR_WHITE, BG_COLOR_GRAY) == true );
 	// Failure: Text_SetCharAndColorAtXY failed
	mu_check( Text_SetCharAndColorAtXY(global_system->screen_[ID_CHANNEL_B], 8, 4, 41, COLOR_BLACK, BG_COLOR_GRAY) == true );
 	// Failure: Text_SetCharAndColorAtXY failed
	mu_check( Text_SetCharAndColorAtXY(global_system->screen_[ID_CHANNEL_B], 9, 4, 42, FG_COLOR_WHITE, BG_COLOR_GRAY) == true );
 	// Failure: Text_SetCharAndColorAtXY failed
	mu_check( Text_SetCharAndColorAtXY(global_system->screen_[ID_CHANNEL_B], 10, 4, 43, FG_COLOR_WHITE, BG_COLOR_GRAY) == true );
 	// Failure: Text_SetCharAndColorAtXY failed
	mu_check( Text_SetCharAndColorAtXY(global_system->screen_[ID_CHANNEL_B], 11, 4, 44, FG_COLOR_WHITE, BG_COLOR_GRAY) == true );
 	// Failure: Text_SetCharAndColorAtXY failed
	mu_check( Text_SetCharAndColorAtXY(global_system->screen_[ID_CHANNEL_B], 12, 4, 45, FG_COLOR_GRAY, BG_COLOR_GRAY) == true );
 	// Failure: Text_SetCharAndColorAtXY failed
	mu_check( Text_SetCharAndColorAtXY(global_system->screen_[ID_CHANNEL_B], 13, 4, 46, FG_COLOR_WHITE, BG_COLOR_GRAY) == true );
 	// Failure: Text_SetCharAndColorAtXY failed
	mu_check( Text_SetCharAndColorAtXY(global_system->screen_[ID_CHANNEL_B], 14, 4, 47, FG_COLOR_BRIGHT_WHITE, BG_COLOR_GRAY) == true );
 	// Failure: Text_SetCharAndColorAtXY failed
	mu_check( Text_SetCharAndColorAtXY(global_system->screen_[ID_CHANNEL_B], 15, 4, 48, FG_COLOR_WHITE, BG_COLOR_GRAY) == true );
 	// Failure: Text_SetCharAndColorAtXY failed
	mu_check( Text_SetCharAndColorAtXY(global_system->screen_[ID_CHANNEL_B], 0, 5, 33, FG_COLOR_BRIGHT_WHITE, BG_COLOR_BLACK) == true );
 	// Failure: Text_SetCharAndColorAtXY failed
	mu_check( Text_SetCharAndColorAtXY(global_system->screen_[ID_CHANNEL_B], 1, 5, 34, FG_COLOR_BRIGHT_WHITE, BG_COLOR_RED) == true );
 	// Failure: Text_SetCharAndColorAtXY failed
	mu_check( Text_SetCharAndColorAtXY(global_system->screen_[ID_CHANNEL_B], 2, 5, 35, FG_COLOR_BRIGHT_WHITE, BG_COLOR_BRIGHT_GREEN) == true );
 	// Failure: Text_SetCharAndColorAtXY failed
	mu_check( Text_SetCharAndColorAtXY(global_system->screen_[ID_CHANNEL_B], 3, 5, 36, FG_COLOR_BRIGHT_WHITE, BG_COLOR_BRIGHT_BLUE) == true );
 	// Failure: Text_SetCharAndColorAtXY failed
	mu_check( Text_SetCharAndColorAtXY(global_system->screen_[ID_CHANNEL_B], 4, 4, 37, FG_COLOR_BRIGHT_WHITE, COLOR_RED) == true );
 	// Failure: Text_SetCharAndColorAtXY failed
	mu_check( Text_SetCharAndColorAtXY(global_system->screen_[ID_CHANNEL_B], 5, 5, 38, FG_COLOR_BRIGHT_WHITE, COLOR_RED) == true );
 	// Failure: Text_SetCharAndColorAtXY failed
	mu_check( Text_SetCharAndColorAtXY(global_system->screen_[ID_CHANNEL_B], 6, 5, 39, FG_COLOR_BRIGHT_WHITE, COLOR_RED) == true );
 	// Failure: Text_SetCharAndColorAtXY failed
	mu_check( Text_SetCharAndColorAtXY(global_system->screen_[ID_CHANNEL_B], 7, 5, 40, FG_COLOR_BRIGHT_WHITE, BG_COLOR_GRAY) == true );
 	// Failure: Text_SetCharAndColorAtXY failed
	mu_check( Text_SetCharAndColorAtXY(global_system->screen_[ID_CHANNEL_B], 8, 5, 41, FG_COLOR_BRIGHT_WHITE, COLOR_RED) == true );
 	// Failure: Text_SetCharAndColorAtXY failed
	mu_check( Text_SetCharAndColorAtXY(global_system->screen_[ID_CHANNEL_B], 9, 5, 42, FG_COLOR_BRIGHT_WHITE, COLOR_RED) == true );
 	// Failure: Text_SetCharAndColorAtXY failed
	mu_check( Text_SetCharAndColorAtXY(global_system->screen_[ID_CHANNEL_B], 10, 5, 43, FG_COLOR_BRIGHT_WHITE, COLOR_RED) == true );
 	// Failure: Text_SetCharAndColorAtXY failed
	mu_check( Text_SetCharAndColorAtXY(global_system->screen_[ID_CHANNEL_B], 11, 5, 44, FG_COLOR_BRIGHT_WHITE, COLOR_RED) == true );
 	// Failure: Text_SetCharAndColorAtXY failed
	mu_check( Text_SetCharAndColorAtXY(global_system->screen_[ID_CHANNEL_B], 12, 5, 45, FG_COLOR_BRIGHT_WHITE, COLOR_RED) == true );
 	// Failure: Text_SetCharAndColorAtXY failed
	mu_check( Text_SetCharAndColorAtXY(global_system->screen_[ID_CHANNEL_B], 13, 5, 46, FG_COLOR_BRIGHT_WHITE, COLOR_RED) == true );
 	// Failure: Text_SetCharAndColorAtXY failed
	mu_check( Text_SetCharAndColorAtXY(global_system->screen_[ID_CHANNEL_B], 14, 5, 47, FG_COLOR_BRIGHT_WHITE, BG_COLOR_WHITE) == true );
 	// Failure: Text_SetCharAndColorAtXY failed
	mu_check( Text_SetCharAndColorAtXY(global_system->screen_[ID_CHANNEL_B], 15, 5, 48, FG_COLOR_BRIGHT_WHITE, BG_COLOR_BRIGHT_WHITE) == true );

	// bad values
 	// Failure: Text_SetCharAndColorAtXY accepted illegal screen ID
	mu_check( Text_SetCharAndColorAtXY(NULL, 0, 0, 'X', FG_COLOR_BRIGHT_WHITE, BG_COLOR_BRIGHT_WHITE) == false );
	// Failure: Text_SetCharAndColorAtXY accepted illegal coordinates
	mu_check( Text_SetCharAndColorAtXY(global_system->screen_[ID_CHANNEL_A], -1, 3, 'b', FG_COLOR_BRIGHT_WHITE, BG_COLOR_BRIGHT_WHITE) == false );
	// Failure: Text_SetCharAndColorAtXY accepted illegal coordinates
	mu_check( Text_SetCharAndColorAtXY(global_system->screen_[ID_CHANNEL_A], -1, -1, 'c', FG_COLOR_BRIGHT_WHITE, BG_COLOR_BRIGHT_WHITE) == false );
	// Failure: Text_SetCharAndColorAtXY accepted illegal coordinates
	mu_check( Text_SetCharAndColorAtXY(global_system->screen_[ID_CHANNEL_A], 0, -1, 'd', FG_COLOR_BRIGHT_WHITE, BG_COLOR_BRIGHT_WHITE) == false );
	// Failure: Text_SetCharAndColorAtXY accepted illegal coordinates
	mu_check( Text_SetCharAndColorAtXY(global_system->screen_[ID_CHANNEL_A], 500, 4, 'e', FG_COLOR_BRIGHT_WHITE, BG_COLOR_BRIGHT_WHITE) == false );
	// Failure: Text_SetCharAndColorAtXY accepted illegal coordinates
	mu_check( Text_SetCharAndColorAtXY(global_system->screen_[ID_CHANNEL_A], 500, 500, 'f', FG_COLOR_BRIGHT_WHITE, BG_COLOR_BRIGHT_WHITE) == false );
	// Failure: Text_SetCharAndColorAtXY accepted illegal coordinates
	mu_check( Text_SetCharAndColorAtXY(global_system->screen_[ID_CHANNEL_A], 0, 500, 'g', FG_COLOR_BRIGHT_WHITE, BG_COLOR_BRIGHT_WHITE) == false );
	// Failure: Text_SetCharAndColorAtXY accepted illegal color value
	mu_check( Text_SetCharAndColorAtXY(global_system->screen_[ID_CHANNEL_A], 1, 1, 'h', -1, 1) == false );
	// Failure: Text_SetCharAndColorAtXY accepted illegal color value
	mu_check( Text_SetCharAndColorAtXY(global_system->screen_[ID_CHANNEL_A], 1, 1, 'i', 17, 1) == false );
	// Failure: Text_SetCharAndColorAtXY accepted illegal color value
	mu_check( Text_SetCharAndColorAtXY(global_system->screen_[ID_CHANNEL_A], 1, 1, 'j', 1, -54) == false );
	// Failure: Text_SetCharAndColorAtXY accepted illegal color value
	mu_check( Text_SetCharAndColorAtXY(global_system->screen_[ID_CHANNEL_A], 1, 1, 'k', 1, 23) == false );
}


// test char and color reading
MU_TEST(test_char_and_attr_reading)
{
	unsigned char	the_color;
	unsigned char	the_char;
	unsigned char	the_attribute_value;
	int16_t			x;
	int16_t			y;
	
	x = 0;
	y = 6;
	the_attribute_value = ((FG_COLOR_MAGENTA << 4) | BG_COLOR_BRIGHT_WHITE);

	// set known chars and colors to test again
	Text_SetColorAtXY(global_system->screen_[ID_CHANNEL_A], x, y, FG_COLOR_BRIGHT_WHITE, BG_COLOR_BLACK);
	Text_SetColorAtXY(global_system->screen_[ID_CHANNEL_A], x+1, y, FG_COLOR_BRIGHT_MAGENTA, BG_COLOR_GRAY);
	Text_SetColorAtXY(global_system->screen_[ID_CHANNEL_A], x+2, y, FG_COLOR_MAGENTA, BG_COLOR_BRIGHT_WHITE);
	Text_SetCharAtXY(global_system->screen_[ID_CHANNEL_A], x, y, CH_MISC_DIA);
	Text_SetCharAtXY(global_system->screen_[ID_CHANNEL_A], x+1, y, CH_MISC_CLUB);
	Text_SetCharAtXY(global_system->screen_[ID_CHANNEL_A], x+2, y, CH_MISC_SPADE);
	
	// Failure: Text_GetForeColorAtXY failed
	mu_check( (the_color = Text_GetForeColorAtXY(global_system->screen_[ID_CHANNEL_A], x, y)) == FG_COLOR_BRIGHT_WHITE );
	// Failure: Text_GetBackColorAtXY failed
	mu_check( (the_color = Text_GetBackColorAtXY(global_system->screen_[ID_CHANNEL_A], x, y)) == BG_COLOR_BLACK );
	
	x++;
	// Failure: Text_GetForeColorAtXY failed
	mu_check( (the_color = Text_GetForeColorAtXY(global_system->screen_[ID_CHANNEL_A], x, y)) == FG_COLOR_BRIGHT_MAGENTA );
	// Failure: Text_GetBackColorAtXY failed
	mu_check( (the_color = Text_GetBackColorAtXY(global_system->screen_[ID_CHANNEL_A], x, y)) == BG_COLOR_GRAY );

	x++;
	// Failure: Text_GetAttrAtXY failed
	mu_check( (the_color = Text_GetAttrAtXY(global_system->screen_[ID_CHANNEL_A], x, y)) == the_attribute_value );
	
	x = 0;
	// Failure: Text_GetCharAtXY failed
	mu_check( (the_char = Text_GetCharAtXY(global_system->screen_[ID_CHANNEL_A], x, y)) == CH_MISC_DIA );
	x++;
	// Failure: Text_GetCharAtXY failed
	mu_check( (the_char = Text_GetCharAtXY(global_system->screen_[ID_CHANNEL_A], x, y)) == CH_MISC_CLUB );
	x++;
	// Failure: Text_GetCharAtXY failed
	mu_check( (the_char = Text_GetCharAtXY(global_system->screen_[ID_CHANNEL_A], x, y)) == CH_MISC_SPADE );
}


MU_TEST(test_line_drawing)
{
	int16_t			x;
	int16_t			y;
	int16_t			line_len;
	unsigned char	the_char;
	
	// good values	
	x = 20;
	y = 4;
	line_len = 20;
	the_char = SC_HLINE;	
 	// Failure: Text_DrawHLine failed
	mu_check( Text_DrawHLine(global_system->screen_[ID_CHANNEL_A], x, y, line_len, the_char, FG_COLOR_BRIGHT_GREEN, BG_COLOR_BLACK, CHAR_ONLY) == true );
	
	y = 8;
 	// Failure: Text_DrawHLine failed
	mu_check( Text_DrawHLine(global_system->screen_[ID_CHANNEL_A], x, y, line_len, the_char, FG_COLOR_BRIGHT_GREEN, BG_COLOR_BLACK, CHAR_AND_ATTR) == true );
	
	y = 4;
	line_len = 4;
	the_char = SC_VLINE;	
 	// Failure: Text_DrawVLine failed
	mu_check( Text_DrawVLine(global_system->screen_[ID_CHANNEL_A], x, y, line_len, the_char, BG_COLOR_BRIGHT_YELLOW, BG_COLOR_BLACK, ATTR_ONLY) == true );

	x = x + 20;	
 	// Failure: Text_DrawVLine failed
	mu_check( Text_DrawVLine(global_system->screen_[ID_CHANNEL_A], x, y, line_len, the_char, BG_COLOR_BRIGHT_YELLOW, BG_COLOR_BLACK, CHAR_AND_ATTR) == true );

	// bad values
 	// Failure: Text_DrawVLine accepted illegal screen ID
	mu_check( Text_DrawVLine(NULL, x, y, line_len, the_char, COLOR_RED, COLOR_BRIGHT_RED, CHAR_AND_ATTR) == false );
  	// Failure: Text_DrawVLine accepted illegal x coord
	mu_check( Text_DrawVLine(global_system->screen_[ID_CHANNEL_B], -1, y, line_len, the_char, COLOR_RED, COLOR_BRIGHT_RED, CHAR_AND_ATTR) == false );
 	// Failure: Text_DrawVLine accepted illegal y coord
	mu_check( Text_DrawVLine(global_system->screen_[ID_CHANNEL_B], x, 425, line_len, the_char, COLOR_RED, COLOR_BRIGHT_RED, CHAR_AND_ATTR) == false );
	

}


MU_TEST(test_basic_box_coords)
{
	int16_t			x;
	int16_t			y;
	int16_t			h_line_len;
	int16_t			v_line_len;
	unsigned char	the_char;
	
	x = 45;
	y = 4;
	h_line_len = 6;
	v_line_len = 6;
	the_char = CH_DIAG_R1;

	// good values	
 	// Failure: Text_DrawBoxCoords failed
	mu_check( Text_DrawBoxCoords(global_system->screen_[ID_CHANNEL_A], x, y, x + h_line_len, y + v_line_len, the_char, FG_COLOR_WHITE, BG_COLOR_GRAY, CHAR_AND_ATTR) == true );

	// bad values
	

}


MU_TEST(test_basic_box_hw)
{
	int16_t			x;
	int16_t			y;
	int16_t			h_line_len;
	int16_t			v_line_len;
	unsigned char	the_char;
	
	x = 60;
	y = 6;
	h_line_len = 6;
	v_line_len = 6;
	the_char = CH_HDITH_8;

	// good values	
 	// Failure: Text_DrawBox failed
	mu_check( Text_DrawBox(global_system->screen_[ID_CHANNEL_A], x, y, h_line_len, v_line_len, the_char, FG_COLOR_BRIGHT_CYAN, BG_COLOR_CYAN, CHAR_AND_ATTR) == true );

	x += 7;
	y += 2;
 	// Failure: Text_DrawBox failed
	mu_check( Text_DrawBox(global_system->screen_[ID_CHANNEL_A], x, y, h_line_len-2, v_line_len+5, --the_char, FG_COLOR_BRIGHT_CYAN, BG_COLOR_CYAN, CHAR_AND_ATTR) == true );


	// bad values
 	// Failure: Text_DrawBox accepted illegal x coord
	mu_check( Text_DrawBox(global_system->screen_[ID_CHANNEL_B], -10, y, h_line_len, v_line_len, the_char, FG_COLOR_BRIGHT_CYAN, BG_COLOR_CYAN, CHAR_AND_ATTR) == false );
}


MU_TEST(test_fancy_box)
{
	int16_t			x1;
	int16_t			y1;
	int16_t			x2;
	int16_t			y2;
// 	int16_t			h_line_len;
// 	int16_t			v_line_len;
	char*	the_message;
	
	// good values	
	the_message = text_test_big_string;

// 	the_message = General_StrlcpyWithAlloc((char*)"\nThe Anecdote\n\nBill Atkinson worked mostly at home, but whenever he made significant progress he rushed in to Apple to show it off to anyone who would appreciate it. This time, he visited the Macintosh offices at Texaco Towers to show off his brand new oval routines in Quickdraw, which were implemented using a really clever algorithm.\n\nBill had added new code to QuickDraw", 80*60+1);

// 	the_message = General_StrlcpyWithAlloc((char*)"\n\n\nLINE ONE\n\nLINE TWO", 80*60+1);
//  	the_message = General_StrlcpyWithAlloc((char*)"THISISAREALLYBIGWORDBIGGERTHANANYYOUCANTHINK_OF_OR_AT_LEAST_I_THINK_SO", 80*60+1);


	// draw huge on channel A screen
	x1 = 2;
	y1 = 2;
	x2 = 95;
	y2 = 70;
	// Failure: Text_FillBox failed
	mu_check( Text_FillBox(global_system->screen_[ID_CHANNEL_A], x1+1, y1+1, x2-1, y2-1, CH_DIAG_R1, BG_COLOR_BRIGHT_CYAN, BG_COLOR_BLUE) == true );
 	// Failure: Text_DrawBoxCoordsFancy failed
	mu_check( Text_DrawBoxCoordsFancy(global_system->screen_[ID_CHANNEL_A], x1, y1, x2, y2, FG_COLOR_WHITE, BG_COLOR_BLACK) == true );

	x1 = 3;
	y1 = 3;
	x2 = 94;
	y2 = 69;
  	// Failure: Text_DrawStringInBox failed
	mu_check( Text_DrawStringInBox(global_system->screen_[ID_CHANNEL_A], x1, y1, x2, y2, the_message, &global_system->text_temp_buffer_, FG_COLOR_BRIGHT_WHITE, BG_COLOR_BLACK, NULL) != NULL );


	// medium box on chan B
	x1 = 12;
	y1 = 4;
	x2 = 68;
	y2 = 51;

	// Failure: Text_FillBox failed
	mu_check( Text_FillBox(global_system->screen_[ID_CHANNEL_B], x1+1, y1+1, x2-1, y2-1, CH_HDITH_8, FG_COLOR_WHITE, BG_COLOR_BRIGHT_WHITE) == true );
 	// Failure: Text_DrawBoxCoordsFancy failed
	mu_check( Text_DrawBoxCoordsFancy(global_system->screen_[ID_CHANNEL_B], x1, y1, x2, y2, FG_COLOR_WHITE, BG_COLOR_GRAY) == true );

	x1 = 13;
	y1 = 5;
	x2 = 67;
	y2 = 50;
  	// Failure: Text_DrawStringInBox failed
	mu_check( Text_DrawStringInBox(global_system->screen_[ID_CHANNEL_B], x1, y1, x2, y2, the_message, &global_system->text_temp_buffer_, FG_COLOR_BLACK, BG_COLOR_BRIGHT_WHITE, NULL) != NULL );


	// small box on chan B
	x1 = 39;
	y1 = 19;
	x2 = 69;
	y2 = 41;

	// Failure: Text_FillBox failed
	mu_check( Text_FillBox(global_system->screen_[ID_CHANNEL_B], x1+1, y1+1, x2-1, y2-1, CH_DIAG_R1, BG_COLOR_BRIGHT_CYAN, BG_COLOR_BLUE) == true );
 	// Failure: Text_DrawBoxCoordsFancy failed
	mu_check( Text_DrawBoxCoordsFancy(global_system->screen_[ID_CHANNEL_B], x1, y1, x2, y2, FG_COLOR_WHITE, BG_COLOR_BLACK) == true );
	
	x1 = 40;
	y1 = 20;
	x2 = 68;
	y2 = 40;
	
	// MB: 2024-12-30: this test crashes system with a division by Zero Error. same test, with bigger box, doesn't cause this. 
	//     something in word wrap. maybe some intermediate buffer is overrunning? no idea why div by zero. 
  	// Failure: Text_DrawStringInBox failed
	//mu_check( Text_DrawStringInBox(global_system->screen_[ID_CHANNEL_B], x1, y1, x2, y2, the_message, &global_system->text_temp_buffer_, FG_COLOR_BRIGHT_WHITE, BG_COLOR_BLACK, NULL) != NULL );

}


MU_TEST(test_draw_string)
{
	char*	the_message;
	
	// good values	
	// Failure: General_StrlcpyWithAlloc returned NULL
	mu_check( (the_message = General_StrlcpyWithAlloc((char*)"this is a string", 250)) != NULL );
	mu_assert_string_eq("this is a string", (char*)the_message);
 	// Failure: Text_DrawStringAtXY failed
	mu_check( Text_DrawStringAtXY(global_system->screen_[ID_CHANNEL_B], 0, 5, the_message, FG_COLOR_BRIGHT_YELLOW, BG_COLOR_BLUE) == true );
 	// Failure: Text_DrawStringAtXY failed
	mu_check( Text_DrawStringAtXY(global_system->screen_[ID_CHANNEL_B], 67, 4, the_message, FG_COLOR_BLUE, BG_COLOR_BRIGHT_YELLOW) == true );
		
	// bad values
 	// Failure: Text_DrawBoxCoordsFancy accepted illegal x coord
	mu_check( Text_DrawStringAtXY(global_system->screen_[ID_CHANNEL_B], -1, 0, the_message, FG_COLOR_BLUE, BG_COLOR_BRIGHT_YELLOW) == false );
}

MU_TEST(test_draw_string_in_box)
{
	char*	the_message;
	
	// good values	
   	//mu_assert((the_message = General_StrlcpyWithAlloc((char*)"This is a short sentence.", 80*60+1)) != NULL, "General_StrlcpyWithAlloc returned NULL" );
  	//mu_assert((the_message = General_StrlcpyWithAlloc((char*)"This is a short sentence. Many are shorter. Like this. Heyo! Hello-Goodbye!", 80*60+1)) != NULL, "General_StrlcpyWithAlloc returned NULL" );
//    	mu_assert((the_message = General_StrlcpyWithAlloc((char*)"This is a longish sentence but others are longer. Many are shorter. Like this. oooooh, what a nice thingamajig you've got there.", 80*60+1)) != NULL, "General_StrlcpyWithAlloc returned NULL" );
	the_message = text_test_big_string;

  	// Failure: Text_DrawStringInBox failed
	mu_check( Text_DrawStringInBox(global_system->screen_[ID_CHANNEL_A], 3, 6, 67, 50, the_message, &global_system->text_temp_buffer_, FG_COLOR_BLACK, BG_COLOR_BRIGHT_WHITE, NULL) != NULL );
  	// Failure: Text_DrawStringInBox failed
	mu_check( Text_DrawStringInBox(global_system->screen_[ID_CHANNEL_B], 21, 21, 40, 40, the_message, &global_system->text_temp_buffer_, FG_COLOR_BRIGHT_WHITE, BG_COLOR_BLACK, NULL) != NULL );
}


MU_TEST(font_replace_test)
{
	// until file objects available in emulator, need to embed data to test font replacement.
	// this is a remapped C64 font for code page 437
	

//   	mu_assert(Text_UpdateFontData(global_system->screen_[ID_CHANNEL_A], (char*)the_new_font_data) == true, "Failed to update font data for Channel A" );
  	// Failure: Failed to update font data for Channel B
	mu_check( Text_UpdateFontData(global_system->screen_[ID_CHANNEL_B], (char*)the_new_font_data) == true );
}



// **** speed tests

MU_TEST(test_speed_1_hline)
{
	long start1;
	long end1;
	long start2;
	long end2;
	int16_t			x;
	int16_t			y;
	int16_t			line_len;
	unsigned char	the_char;
	int16_t			i;
	int16_t			num_passes = 90;
	int16_t			j;
	int16_t			num_cycles = 10;

	x = 1;
	y = 1;
	line_len = 120;
	the_char = SC_HLINE;	
	
	// test speed of first variant
	start1 = mu_timer_real();
	
	for (j = 0; j < num_cycles; j++)
	{
		for (i=0; i < num_passes; i++)
		{
			// Failure: Text_DrawHLine failed
			mu_check( Text_DrawHLineSlow(global_system->screen_[ID_CHANNEL_A], x, y + i, line_len, the_char, FG_COLOR_BRIGHT_GREEN, BG_COLOR_BLACK, CHAR_ONLY) == true );
		}
	}
		
	end1 = mu_timer_real();
	
	// test speed of second variant
	x++;
	start2 = mu_timer_real();
	
	for (j = 0; j < num_cycles; j++)
	{
		for (i=0; i < num_passes; i++)
		{
			// Failure: Text_DrawHLine failed
			mu_check( Text_DrawHLine(global_system->screen_[ID_CHANNEL_A], x, y + i, line_len, the_char, FG_COLOR_BRIGHT_RED, BG_COLOR_BLACK, CHAR_ONLY) == true );
		}
	}
	
	end2 = mu_timer_real();
	
	printf("\nSpeed results: first routine completed in %li ticks; second in %li ticks\n", end1 - start1, end2 - start2);
	
	// run again, with different values
	
	x = 1;
	y = 1;
	line_len = 120;
	the_char = SC_HLINE;	
	
	// test speed of first variant
	start1 = mu_timer_real();
	
	for (j = 0; j < num_cycles; j++)
	{
		for (i=0; i < num_passes; i++)
		{
			// Failure: Text_DrawHLine failed
			mu_check( Text_DrawHLineSlow(global_system->screen_[ID_CHANNEL_A], x, y + i, line_len, the_char, FG_COLOR_BRIGHT_GREEN, BG_COLOR_BLACK, CHAR_AND_ATTR) == true );
		}
	}
	
	end1 = mu_timer_real();
	
	// test speed of second variant
	x++;
	start2 = mu_timer_real();
	
	for (j = 0; j < num_cycles; j++)
	{
		for (i=0; i < num_passes; i++)
		{
			// Failure: Text_DrawHLine failed
			mu_check( Text_DrawHLine(global_system->screen_[ID_CHANNEL_A], x, y + i, line_len, the_char, FG_COLOR_BRIGHT_RED, BG_COLOR_BLACK, CHAR_AND_ATTR) == true );
		}
	}
	
	end2 = mu_timer_real();
	
	printf("\nSpeed results: first routine completed in %li ticks; second in %li ticks\n", end1 - start1, end2 - start2);
}



	// speed tests
MU_TEST_SUITE(test_suite_speed)
{	
	MU_SUITE_CONFIGURE(&test_setup, &test_teardown);
	
	MU_RUN_TEST(test_speed_1_hline);
}


// unit tests
MU_TEST_SUITE(test_suite_units)
{	
	MU_SUITE_CONFIGURE(&test_setup, &test_teardown);

	MU_RUN_TEST(test_fill_text);
	MU_RUN_TEST(test_fill_attr);
	MU_RUN_TEST(test_fill_box);
	
	MU_RUN_TEST(test_show_font);

	MU_RUN_TEST(test_char_placement);

	MU_RUN_TEST(test_block_copy_box);

// //	MU_RUN_TEST(test_font_overwrite);
// // 	MU_RUN_TEST(test_update_font);
	
	MU_RUN_TEST(test_char_and_attr_writing);
	MU_RUN_TEST(test_char_and_attr_reading);
	
	MU_RUN_TEST(test_line_drawing);
	MU_RUN_TEST(test_basic_box_coords);
	MU_RUN_TEST(test_basic_box_hw);
	MU_RUN_TEST(test_fancy_box);
	
	MU_RUN_TEST(test_draw_string);
	MU_RUN_TEST(test_draw_string_in_box);

	MU_RUN_TEST(test_invert_box);
	
	MU_RUN_TEST(test_block_copy);
	
	MU_RUN_TEST(font_replace_test);
}



/*****************************************************************************/
/*                        Public Function Definitions                        */
/*****************************************************************************/



int main(int argc, char* argv[])
{
	printf("**** text.c Test Suite **** \n");

	// allocate the system object
	if ((global_system = Sys_New()) == NULL)
	{
		printf("Couldn't instantiate system object \n");
		sys_exit(-1);
	}

	DEBUG_OUT(("%s %d: System object created ok. Initiating system components...", __func__, __LINE__));
	
	// initialize system and allocate all its child parts
	if (Sys_InitSystem(global_system) == false)
	{
		DEBUG_OUT(("%s %d: Couldn't initialize the system", __func__, __LINE__));
		Sys_Exit(&global_system, PARAM_EXIT_ON_ERROR);
	}

	DEBUG_OUT(("%s %d: System initiation complete, starting tests...", __func__, __LINE__));

	Sys_SetGraphicMode(global_system, PARAM_SPRITES_OFF, PARAM_BITMAP_OFF, PARAM_TILES_OFF, PARAM_TEXT_OVERLAY_OFF, PARAM_TEXT_ON);

 	MU_RUN_SUITE(test_suite_units);
// 	MU_RUN_SUITE(test_suite_speed);
 	MU_REPORT();

	Sys_Exit(&global_system, PARAM_EXIT_NO_ERROR);
	
	return MU_EXIT_CODE;
}
