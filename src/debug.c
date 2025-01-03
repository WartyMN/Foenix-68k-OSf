/*
 * debug.c
 *
 *  Created on: July 6, 2024
 *      Author: micahbly
 *
 *  - debug functions extracted from general.c
 */

// in addition to extracting debug stuff from general.c
// adapted from F256e version to A2560K beginning Dec 26, 2024


/*****************************************************************************/
/*                                Includes                                   */
/*****************************************************************************/

// project includes
#include "debug.h"


// C includes
// #include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

// A2560 includes
#include "a2560k.h"
#include <mcp/syscalls.h>


/*****************************************************************************/
/*                               Definitions                                 */
/*****************************************************************************/

// if doing serial debug out, # of times to try before giving up.
#define UART_MAX_SEND_ATTEMPTS	1000


/*****************************************************************************/
/*                          File-scoped Variables                            */
/*****************************************************************************/

// logging related: only do if debugging is active
#if defined LOG_LEVEL_1 || defined LOG_LEVEL_2 || defined LOG_LEVEL_3 || defined LOG_LEVEL_4 || defined LOG_LEVEL_5
	static char				debug_varargs_buffer_storage[256];	// create once, use for every debug and logging function
	static char*			debug_varargs_buffer = debug_varargs_buffer_storage;
	static char				debug_out_buffer_storage[256];	// create once, use for every debug and logging function
	static char*			debug_out_buffer = debug_out_buffer_storage;
	static const char*		kDebugFlag[5] = {
								"[ERROR]",
								"[WARNING]",
								"[INFO]",
								"[DEBUG]",
								"[ALLOC]"
							};

	#ifndef USE_SERIAL_LOGGING
		static FILE*			debug_log_file;
		//static int16_t			debug_log_file_handle;
		//static FIL				debug_log_file_handle;
		//unsigned int			debug_bytes_written_to_disk = 0;
		//static FRESULT			debug_ffs_result;
	#endif
#endif

/*****************************************************************************/
/*                             Global Variables                              */
/*****************************************************************************/

extern System*			global_system;




/*****************************************************************************/
/*                       Private Function Prototypes                         */
/*****************************************************************************/



/*****************************************************************************/
/*                       Private Function Definitions                        */
/*****************************************************************************/






/*****************************************************************************/
/*                        Public Function Definitions                        */
/*****************************************************************************/




// **** LOGGING AND DEBUG UTILITIES *****


#if defined LOG_LEVEL_1 || defined LOG_LEVEL_2 || defined LOG_LEVEL_3 || defined LOG_LEVEL_4 || defined LOG_LEVEL_5

	#if defined USE_SERIAL_LOGGING
		// LOGIC FOR SERIAL LOGGING:
		//   we will use the baud speed defined in by macro in build script. 81N.
		//   we will NOT check if the other side is receiving, we just blast away
		//   no READING of the serial port happens. just writing to it.
		//   UART setup is based on mgr42's dcopy project, the uart.asm file.
		//   linux/mac setup for using 'screen' command as terminal: screen /dev/tty.usbserial-FT53JP031 300,cs8,-ixon,-ixoff,-istrip,-parenb

		// set UART chip to DLAB mode
		void Serial_SetDLAB(void);
	
		// turn off DLAB mode on UART chip
		void Serial_ClearDLAB(void);
			
		// set up UART for serial comms
		void Serial_InitUART(void);
		
		// send 1-255 bytes to the UART serial connection
		// returns # of bytes successfully sent (which may be less than number requested, in event of error, etc.)
		uint8_t Serial_SendData(char* the_buffer, uint16_t buffer_size);
		
		// send a byte over the UART serial connection
		// if the UART send buffer does not have space for the byte, it will try for UART_MAX_SEND_ATTEMPTS then return an error
		// returns false on any error condition
		bool Serial_SendByte(uint8_t the_byte);
		
		
		// set UART chip to DLAB mode
		void Serial_SetDLAB(void)
		{
			R8(UART_LCR) = R8(UART_LCR) | UART_DLAB_MASK;
		}
		
		// turn off DLAB mode on UART chip
		void Serial_ClearDLAB(void)
		{
			R8(UART_LCR) = R8(UART_LCR) & (~UART_DLAB_MASK);
		}
			
		// set up UART for serial comms
		void Serial_InitUART(void)
		{
			R8(UART_LCR) = UART_DATA_BITS;	// 8 bits, no parity, 1 stop bit, no break signal required
			Serial_SetDLAB();
			//R16(UART_DLL) = UART_BAUD_DIV_57600;
			//R16(UART_DLL) = BSWAP(UART_BAUD_DIV_57600);
			//printf(" UART_BAUD_DIV_57600=%x, swapped=%x \n", UART_BAUD_DIV_57600, BSWAP(UART_BAUD_DIV_57600));
			//printf(" UART_BAUD_DIV_57600 MSB=%x, LSB=%x \n", (UART_BAUD_DIV_57600 >> 8) & 0xFF, UART_BAUD_DIV_57600 & 0xFF);
			R8(UART_DLL) = UART_BAUD_DIV_115200 & 0xFF;
			R8(UART_DLM) = (UART_BAUD_DIV_115200 >> 8) & 0xFF;
			
			Serial_ClearDLAB();

			//R8(UART_FCR) = 0xc1;	// enable FIFO mode - pjw example. same as UART_IIR

	// no idea what this does. copying blindly from EMWhite's BASIC example
	R8(UART_IIR) = 231;
	R8(UART_IER) = 0;	// probably turns off interrupts

	// Read and clear status registers
	uint8_t		junk;
	junk = R8(UART_LSR);
	junk = R8(UART_MSR);

// 			// check for an error, and clear it by reading LSR
// 			uint8_t		error_check;
// 			
// 			error_check = R8(UART_LSR) & UART_ERROR_MASK;
// 			
// 			if (error_check > 0)
// 			{
// 				R8(UART_BASE);
// 			}


		}


		// send a byte over the UART serial connection
		// if the UART send buffer does not have space for the byte, it will try for UART_MAX_SEND_ATTEMPTS then return an error
		// returns false on any error condition
		bool Serial_SendByte(uint8_t the_byte)
		{
			uint8_t		error_code;
			bool		uart_out_buff_is_empty = false;
			uint16_t	num_tries = 0;
			
			error_code = R8(UART_LSR) & UART_ERROR_MASK;
			
			if (error_code > 0)
			{
				// try to clear error. 
				error_code = R8(UART_LSR);
				error_code = R8(UART_MSR);
			}
			
			while (uart_out_buff_is_empty == false && num_tries < UART_MAX_SEND_ATTEMPTS)
			{
				uart_out_buff_is_empty = R8(UART_LSR) & UART_THR_IS_EMPTY;
				++num_tries;
			};
			
			if (uart_out_buff_is_empty == false)
			{
				//printf(" in buff was still full after all tries \n");
				goto error;
			}
			
			R8(UART_THR) = the_byte;
			
			return true;
			
			error:
				//printf(" serial send hit error condition \n");
				return false;
		}
		
		
		// send 1-255 bytes to the UART serial connection
		// returns # of bytes successfully sent (which may be less than number requested, in event of error, etc.)
		uint8_t Serial_SendData(char* the_buffer, uint16_t buffer_size)
		{
			uint16_t	i;
			uint8_t		the_byte;
			
			//printf("serial debug trying to send buffer of size %u: '%s' \n", buffer_size, the_buffer);
			//printf("'%s' \n", the_buffer);
			if (buffer_size > 256)
			{
				buffer_size = 256;
				//return 0;
			}
			
			for (i=0; i <= buffer_size; i++)
			{
				the_byte = the_buffer[i];
				
				if (Serial_SendByte(the_byte) == false)
				{
					return i;
				}
			}
			
			// add a line return if we got this far
			//Serial_SendByte(0x0D);
			
			return i;
		}
		
    
	#endif
	

// DEBUG functionality I want:
//   3 levels of logging (err/warn/info)
//   additional debug out function that leaves no footprint in compiled release version of code (calls to it also disappear)
//   able to pass format string and multiple variables when needed

#ifdef LOG_LEVEL_1 
	void General_LogError(const char* format, ...)
	{
		va_list		args;
		uint16_t	the_len;
		
		va_start(args, format);
		vsprintf(debug_varargs_buffer, format, args);
		va_end(args);
	
		sprintf(debug_out_buffer, "%s %s\n", kDebugFlag[LogError], debug_varargs_buffer);
		the_len = strlen(debug_out_buffer);
	
		#if defined USE_SERIAL_LOGGING
			Serial_SendData(debug_out_buffer, the_len);
		#else
			fprintf(debug_log_file, "%s %s\n", kDebugFlag[LogError], debug_varargs_buffer);
		#endif
	}
#endif


#ifdef LOG_LEVEL_2
	void General_LogWarning(const char* format, ...)
	{
		va_list		args;
		uint16_t	the_len;
		
		va_start(args, format);
		vsprintf(debug_varargs_buffer, format, args);
		va_end(args);
	
		sprintf(debug_out_buffer, "%s %s\n", kDebugFlag[LogWarning], debug_varargs_buffer);
		the_len = strlen(debug_out_buffer);
	
		#if defined USE_SERIAL_LOGGING
			Serial_SendData(debug_out_buffer, the_len);
		#else
			fprintf(debug_log_file, "%s %s\n", kDebugFlag[LogWarning], debug_varargs_buffer);
		#endif
	}
#endif


#ifdef LOG_LEVEL_3
	void General_LogInfo(const char* format, ...)
	{
		va_list		args;
		uint16_t	the_len;
		
		va_start(args, format);
		vsprintf(debug_varargs_buffer, format, args);
		va_end(args);
	
		sprintf(debug_out_buffer, "%s %s\n", kDebugFlag[LogInfo], debug_varargs_buffer);
		the_len = strlen(debug_out_buffer);
	
		#if defined USE_SERIAL_LOGGING
			Serial_SendData(debug_out_buffer, the_len);
		#else
			fprintf(debug_log_file, "%s %s\n", kDebugFlag[LogInfo], debug_varargs_buffer);
		#endif
	}	
#endif

#ifdef LOG_LEVEL_4
	void General_DebugOut(const char* format, ...)
	{
		va_list		args;
		uint16_t	the_len;
		
		va_start(args, format);
		vsprintf(debug_varargs_buffer, format, args);
		va_end(args);
		
		sprintf(debug_out_buffer, "%s %s\n", kDebugFlag[LogDebug], debug_varargs_buffer);
		the_len = strlen(debug_out_buffer);
	
		#if defined USE_SERIAL_LOGGING
			Serial_SendData(debug_out_buffer, the_len);
		#else
			fprintf(debug_log_file, "%s %s\n", kDebugFlag[LogDebug], debug_varargs_buffer);
		#endif
	}
#endif

#ifdef LOG_LEVEL_5
	void General_LogAlloc(const char* format, ...)
	{
		va_list		args;
		uint16_t	the_len;
		
		va_start(args, format);
		vsprintf(debug_varargs_buffer, format, args);
		va_end(args);
		
		sprintf(debug_out_buffer, "%s %s\n", kDebugFlag[LogAlloc], debug_varargs_buffer);
		the_len = strlen(debug_out_buffer);
	
		#if defined USE_SERIAL_LOGGING
			Serial_SendData(debug_out_buffer, the_len);
		#else
			fprintf(debug_log_file, "%s %s\n", kDebugFlag[LogAlloc], debug_varargs_buffer);
		#endif
	}
#endif

// initialize log file
// globals for the log file
bool General_LogInitialize(void)
{
printf("initializing logging... \n");

	#if defined USE_SERIAL_LOGGING
		// LOGIC FOR SERIAL LOGGING:
		//   we will use the baud speed defined in by macro in build script. 81N.
		//   we will NOT check if the other side is receiving, we just blast away
		//   no READING of the serial port happens. just writing to it.
		//   UART setup is based on mgr42's dcopy project, the uart.asm file.
		//   linux/mac setup for using 'screen' command as terminal: screen /dev/tty.usbserial-FT53JP031 300,cs8,-ixon,-ixoff,-istrip,-parenb
printf("Configuring UART for serial logging... \n");

		Serial_InitUART();

	#else
		const char*		the_file_path = "/sd/sys_log.txt";
	
		//printf("Opening log file for disk logging... \n");

		debug_log_file = fopen( the_file_path, "w");
		
		if (debug_log_file == NULL)
		{
			printf("General_LogInitialize: log file could not be opened! \n");
			return false;
		}
	
		fprintf(debug_log_file, "*** started log file ***\n");
	#endif
	
	return true;
}


// close the log file
void General_LogCleanUp(void)
{
	#if defined USE_SERIAL_LOGGING
	#else
		if (debug_log_file != NULL)
		{
			fprintf(debug_log_file, "*** end of log entries ***\n");
			fclose(debug_log_file);
		}
	#endif
}

#endif



// // Print out a section of memory in a hex viewer style
// // display length is hard-coded to one screen at 80x59 (MEM_DUMP_BYTES_PER_ROW * MAX_TEXT_VIEW_ROWS_PER_PAGE)
// void General_ViewHexDump(uint8_t* the_buffer)
// {
// 	// LOGIC
// 	//   we only have 80x60 to work with, and we need a row for "hit space for more, esc to stop"
// 	//     so 60 rows * 16 bytes = 960 max bytes can be shown
// 	//   we only need one buffer as we read and print to screen line by line (80 bytes)
// 	//   we need to keep the file stream open until it is used up, or user exits loop
// 
// 	uint8_t		y;
// 	uint8_t		cut_off_pos;
// 	uint16_t	num_bytes_to_read = MEM_DUMP_BYTES_PER_ROW;
// 	uint8_t*	loc_in_file = 0x000;	// will track the location within the file, so we can show to users on left side. 
// 
// 	cut_off_pos = MEM_DUMP_BYTES_PER_ROW * 3; // each char represented by 2 hex digits and a space
// 	y = 0;
// 	Text_ClearScreen(FILE_CONTENTS_FOREGROUND_COLOR, FILE_CONTENTS_BACKGROUND_COLOR);
// 	sprintf(global_string_buff1, "** Hex view of '%s' -- Hit SPACE for more; Esc to stop **", "Memory Dump");
// 	Text_DrawStringAtXY(0, y++, global_string_buff1, FILE_CONTENTS_ACCENT_COLOR, FILE_CONTENTS_BACKGROUND_COLOR);
// 				
// 	// loop until all screen rows used
// 	do
// 	{
// 
// 		sprintf(global_string_buff2, "%p: ", the_buffer);
// 		Text_DrawStringAtXY(0, y, global_string_buff2, FILE_CONTENTS_ACCENT_COLOR, FILE_CONTENTS_BACKGROUND_COLOR);
// 	
// 		sprintf(global_string_buff2, "%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x  ", 
// 			the_buffer[0], the_buffer[1], the_buffer[2], the_buffer[3], the_buffer[4], the_buffer[5], the_buffer[6], the_buffer[7], 
// 			the_buffer[8], the_buffer[9], the_buffer[10], the_buffer[11], the_buffer[12], the_buffer[13], the_buffer[14], the_buffer[15]);
// 		
// 		// cut off the string
// 		global_string_buff2[cut_off_pos] = '\0';
// 		
// 		Text_DrawStringAtXY(MEM_DUMP_START_X_FOR_HEX, y, global_string_buff2, FILE_CONTENTS_FOREGROUND_COLOR, FILE_CONTENTS_BACKGROUND_COLOR);
// 
// 		// render chars with char draw function to avoid problem of 0s getting treated as nulls in sprintf
// 		Text_DrawCharsAtXY(MEM_DUMP_START_X_FOR_CHAR, y, (uint8_t*)the_buffer, MEM_DUMP_BYTES_PER_ROW);
// 	
// 		the_buffer += MEM_DUMP_BYTES_PER_ROW;
// 		++y;
// 	
// 	} while (y < MAX_TEXT_VIEW_ROWS_PER_PAGE);
// }


