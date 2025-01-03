/*
 * sys_test.c
 *
 *  Created on: Mar 22, 2022
 *      Author: micahbly
 */






/*****************************************************************************/
/*                                Includes                                   */
/*****************************************************************************/

// unit testing framework
#include "minunit.h"

// class being tested
#include "sys.h"

// project includes
#include "debug.h"
#include "startup.h"

// C includes
#include <stdbool.h>

// A2560 includes
#include "a2560k.h"
#include <mcp/syscalls.h>
#include <stdint.h>


/*****************************************************************************/
/*                               Definitions                                 */
/*****************************************************************************/



/*****************************************************************************/
/*                               Enumerations                                */
/*****************************************************************************/



/*****************************************************************************/
/*                             Global Variables                              */
/*****************************************************************************/

System*			global_system;




/*****************************************************************************/
/*                       Private Function Prototypes                         */
/*****************************************************************************/

// try to open serial comms
bool Test_OpenSerial(void);

// test ps/2 mouse
void Test_MCPMouse(void);

/*****************************************************************************/
/*                       Private Function Definitions                        */
/*****************************************************************************/

// try to open serial comms
bool Test_OpenSerial(void)
{
	// test serial via MCP
	
	#define CDEV_CONSOLE 0
	#define CDEV_EVID 1
	#define CDEV_COM1 2
	#define CDEV_COM2 3
	#define CDEV_LPT 4
	#define CDEV_MIDI 5

	int16_t uart;
	uint8_t	open_command[32] = "9600,8,1,NONE";
	uint8_t*	the_open_command = open_command;
	
	//  uart = sys_chan_open(3, "9600,8,1,NONE", 0);
	// //sys_chan_open(3, NULL, 0x03);	// RW
	// sys_chan_write(3, "hello from A2560k", 17);
	// sys_chan_close(3);

	uart = sys_chan_open(CDEV_COM1, the_open_command, 0);
	if (uart >= 0)
    {
		printf("COM%d: 9600, no parity, 1 stop bit, 8 data bits\nPress ESC to finish.\n", CDEV_COM1 - CDEV_COM1 + 1);
        
		sys_chan_write_b(uart, 65);
		return true;
 	}
	else
 	{
		printf("couldn't open CDEV_COM1 \n");
		return false;
 	}
	
// short cli_test_uart(short channel, int argc, const char * argv[]) {
//     char c, c_out;
//     short scan_code;
//     char buffer[80];
//     short cdev = CDEV_COM1;
//     short uart = -1;
//     short uart_index = 0;
//     unsigned long uart_address = 0;
// 
//     if (argc > 1) {
//         // Get the COM port number
//         short port = (short)cli_eval_number(argv[1]);
//         if (port <= 1) cdev = CDEV_COM1;
//         if (port >= 2) cdev = CDEV_COM2;
//     }
// 
//     uart_index = cdev - CDEV_COM1;
//     uart_address = (unsigned long)uart_get_base(uart_index);
// 
//     sprintf(buffer, "Serial port loopback test of COM%d at 0x%08X...\n", cdev - CDEV_COM1 + 1, uart_address);
//     print(channel, buffer);
// 
//     uart = sys_chan_open(cdev, "9600,8,1,NONE", 0);
//     if (uart >= 0) {
//         sprintf(buffer, "COM%d: 9600, no parity, 1 stop bit, 8 data bits\nPress ESC to finish.\n", cdev - CDEV_COM1 + 1);
//         print(channel, buffer);
// 
//         c_out = ' ';
//         do {
//             sys_chan_write_b(uart, c_out++);
//             if (c_out > '}') {
//                 c_out = ' ';
//                 sys_chan_write_b(uart, '\r');
//                 sys_chan_write_b(uart, '\n');
//             }
// 
//             if (sys_chan_status(uart) & CDEV_STAT_READABLE) {
//                 c = sys_chan_read_b(uart);
//                 if (c != 0) {
//                     sys_chan_write_b(channel, c);
//                 }
//             }
// 
//             scan_code = sys_kbd_scancode();
//         } while (scan_code != 0x01);
//     } else {
//         sprintf(buffer, "Unable to open the serial port: %d\n", uart);
//         print(channel, buffer);
//     }
// 
//     return 0;
// }

}


// test ps/2 mouse
void Test_MCPMouse(void)
{
// 	if (mouse_init() != 0)
// 	{
// 		printf("Mouse init returned error. \n");
// 	}
// 	else
// 	{
// 		printf("Mouse init returned error. \n");
// 	}
	
	
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



// **** speed tests

MU_TEST(test_speed_1)
{
	long start1;
	long end1;
	long start2;
	long end2;
	
	// test speed of first variant
	start1 = mu_timer_real();
	


	// speed test 1 goes here
	
	
	end1 = mu_timer_real();
	
	// test speed of 2nd variant
	start2 = mu_timer_real();
	


	// speed test 2 goes here
	
	
	end2 = mu_timer_real();
	
	printf("\nSpeed results: first routine completed in %li ticks; second in %li ticks\n", end1 - start1, end2 - start2);
}



// speed tests
MU_TEST_SUITE(test_suite_speed)
{	
	MU_SUITE_CONFIGURE(&test_setup, &test_teardown);
	
	MU_RUN_TEST(test_speed_1);
}


// unit tests
MU_TEST_SUITE(test_suite_units)
{	
	MU_SUITE_CONFIGURE(&test_setup, &test_teardown);
	
// 	MU_RUN_TEST(string_manipulation_test);
// 	MU_RUN_TEST(misc_test);
// 	MU_RUN_TEST(number_string_test);
// 	MU_RUN_TEST(rect_test);
// 	MU_RUN_TEST(filepath_manipulation_test);
}




/*****************************************************************************/
/*                        Public Function Definitions                        */
/*****************************************************************************/



int main(int argc, char* argv[])
{
	printf("**** sys.c Test Suite **** \n");

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

	DEBUG_OUT(("%s %d: Setting graphics mode...", __func__, __LINE__));



// printf("Setting overlay text mode... \n ");
// 	Sys_SetModeText(global_system, true);
// printf("text overlay mode on. \n ");

// 	printf("Setting graphics mode... \n ");
// 	
// 	Sys_SetModeGraphics(global_system);		
// 	
// 	printf("now in graphics mode \n ");

	Startup_ShowSplash();
	
	// turn on sprites too
	Sys_SetGraphicMode(global_system, PARAM_SPRITES_ON, PARAM_BITMAP_ON, PARAM_TILES_OFF, PARAM_TEXT_OVERLAY_ON, PARAM_TEXT_ON);
	
	MU_RUN_SUITE(test_suite_units);
// 	MU_RUN_SUITE(test_suite_speed);
	MU_REPORT();

	//Sys_SetModeText(global_system, false);
	
	// test of serial connection
	//Test_OpenSerial();

	// test ps/2 mouse
	//Test_MCPMouse();
	
	printf("sys test complete \n");

	Sys_Exit(&global_system, PARAM_EXIT_NO_ERROR);
	
	return MU_EXIT_CODE;
}
