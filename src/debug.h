//! @file debug.h

/*
 * debug.h
 *
 *  Created on: July 6, 2024
 *      Author: micahbly
 */

// This is the debug stuff extracted from general.c.
// adapted from F256e version to A2560K beginning Dec 26, 2024

#ifndef DEBUG_H_
#define DEBUG_H_


/* about this class
 *
 *
 *
 *** things this class needs to be able to do
 * print debug statements to file or screen or RS232
 * (all functions in this class are excluded from compiling unless one or more debug LOG_LEVEL_1, etc macros are defined)
 *
 *** things objects of this class have
 *
 *
 */


/*****************************************************************************/
/*                                Includes                                   */
/*****************************************************************************/

// project includes
//#include "text.h"

// C includes
#include <stdbool.h>

// F256 includes

/*****************************************************************************/
/*                            Macro Definitions                              */
/*****************************************************************************/


#ifdef LOG_LEVEL_1 
	#define LOG_ERR(x) General_LogError x
#else
	#define LOG_ERR(x)
#endif
#ifdef LOG_LEVEL_2
	#define LOG_WARN(x) General_LogWarning x
#else
	#define LOG_WARN(x)
#endif
#ifdef LOG_LEVEL_3
	#define LOG_INFO(x) General_LogInfo x
#else
	#define LOG_INFO(x)
#endif
#ifdef LOG_LEVEL_4
	#define DEBUG_OUT(x) General_DebugOut x
#else
	#define DEBUG_OUT(x)
#endif
#ifdef LOG_LEVEL_5
	#define LOG_ALLOC(x) General_LogAlloc x
#else
	#define LOG_ALLOC(x)
#endif


/*****************************************************************************/
/*                               Enumerations                                */
/*****************************************************************************/

#define LogError	0
#define LogWarning	1
#define LogInfo		2
#define LogDebug	3
#define LogAlloc	4


/*****************************************************************************/
/*                                 Structs                                   */
/*****************************************************************************/


/*****************************************************************************/
/*                             Global Variables                              */
/*****************************************************************************/


/*****************************************************************************/
/*                       Public Function Prototypes                          */
/*****************************************************************************/



// **** LOGGING AND DEBUG UTILITIES *****



// // Print out a section of memory in a hex viewer style
// // display length is hard-coded to one screen at 80x59 (MEM_DUMP_BYTES_PER_ROW * MAX_TEXT_VIEW_ROWS_PER_PAGE)
// void General_ViewHexDump(uint8_t* the_buffer);



// *********  logging functionality. requires global_log_file to have been opened.
void General_LogError(const char* format, ...);
void General_LogWarning(const char* format, ...);
void General_LogInfo(const char* format, ...);
void General_DebugOut(const char* format, ...);
void General_LogAlloc(const char* format, ...);
bool General_LogInitialize(void);
void General_LogCleanUp(void);





#endif /* DEBUG_H_ */
