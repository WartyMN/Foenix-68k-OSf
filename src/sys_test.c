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
/*                             Global Variables                              */
/*****************************************************************************/

System*			global_system;




/*****************************************************************************/
/*                       Private Function Prototypes                         */
/*****************************************************************************/



/*****************************************************************************/
/*                       Private Function Definitions                        */
/*****************************************************************************/



// int _Stub_close(int fd)
// {
// 	return 0;
// }
// 
// int _Stub_open(const char *path, int oflag, ...)
// {
// 	return 0;
// }
// 
// size_t _Stub_write(int fd, const void *buf, size_t count)
// {
// 	return 0;
// }
// 
// size_t _Stub_read(int fd, void *buf, size_t count)
// {
// 	return 0;
// }
// 
// long _Stub_lseek(int fd, long offset, int whence)
// {
// 	return 0;
// }



/*****************************************************************************/
/*                        MinUnit Function Defintions                        */
/*****************************************************************************/



// void text_test_setup(void)	// this is called EVERY test
// {
// // 	foo = 7;
// // 	bar = 4;
// // 	
// }
// 
// 
// void text_test_teardown(void)	// this is called EVERY test
// {
// 
// }
// 
// 
// 
// // **** speed tests
// 
// MU_TEST(text_test_hline_speed)
// {
// 	long start1;
// 	long end1;
// 	long start2;
// 	long end2;
// 	
// 	// test speed of first variant
// 	start1 = mu_timer_real();
// 	
// 
// 
// 	// speed test 1 goes here
// 	
// 	
// 	end1 = mu_timer_real();
// 	
// 	// test speed of 2nd variant
// 	start2 = mu_timer_real();
// 	
// 
// 
// 	// speed test 2 goes here
// 	
// 	
// 	end2 = mu_timer_real();
// 	
// 	printf("\nSpeed results: first routine completed in %li ticks; second in %li ticks\n", end1 - start1, end2 - start2);
// }
// 
// 
// 
// 	// speed tests
// MU_TEST_SUITE(text_test_suite_speed)
// {	
// 	MU_SUITE_CONFIGURE(&text_test_setup, &text_test_teardown);
// 	
// // 	MU_RUN_TEST(text_test_hline_speed);
// }
// 
// 
// // unit tests
// MU_TEST_SUITE(text_test_suite_units)
// {	
// 	MU_SUITE_CONFIGURE(&text_test_setup, &text_test_teardown);
// 	
// // 	MU_RUN_TEST(font_replace_test);
// }
// 



/*****************************************************************************/
/*                        Public Function Definitions                        */
/*****************************************************************************/



int main(int argc, char* argv[])
{
	printf("**** sys.c Test Suite **** \n");

	// initialize the system object
	if ((global_system = Sys_New()) == NULL)
	{
		//LOG_ERR(("%s %d: Couldn't instantiate system object", __func__, __LINE__));
		printf("Couldn't instantiate system object \n");
		exit(0);
	}
// 
// 	DEBUG_OUT(("%s %d: System object created ok. Initiating list of windows...", __func__, __LINE__));
// 	
// 	if (Sys_InitSystem(global_system) == false)
// 	{
// 		DEBUG_OUT(("%s %d: Couldn't initialize the system", __func__, __LINE__));
// 		exit(0);
// 	}
// 
// 	DEBUG_OUT(("%s %d: Setting graphics mode...", __func__, __LINE__));
// 
// 	printf("Hiya from sys world.");
// 	
// 	Sys_SetModeGraphics(global_system);		
// 	
// 	printf("now in graphics mode");
// 
// 	MU_RUN_SUITE(text_test_suite_units);
// // 	MU_RUN_SUITE(text_test_suite_speed);
// 	MU_REPORT();
// 
// 	Sys_SetModeText(global_system, false);

	printf("sys test complete \n");

	return MU_EXIT_CODE;
}
