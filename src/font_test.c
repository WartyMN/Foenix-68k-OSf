/*
 * font_test.c
 *
 *  Created on: Mar 15, 2022
 *      Author: micahbly
 */






/*****************************************************************************/
/*                                Includes                                   */
/*****************************************************************************/

// unit testing framework
#include "minunit.h"

// project includes
#include "debug.h"
#include "sys.h"

// class being tested
#include "font.h"

// C includes
#include <stdbool.h>

// A2560 includes
#include "a2560k.h"
//#include <mcp/syscalls.h>



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
	
// 	MU_RUN_TEST(test_speed_1);
}


// unit tests
MU_TEST_SUITE(test_suite_units)
{	
	MU_SUITE_CONFIGURE(&test_setup, &test_teardown);
	
// 	MU_RUN_TEST(font_replace_test);
}




/*****************************************************************************/
/*                        Public Function Definitions                        */
/*****************************************************************************/



int main(int argc, char* argv[])
{
	printf("**** font.c Test Suite **** \n");

	// initialize the system object
	if ((global_system = Sys_New()) == NULL)
	{
		printf("Couldn't instantiate system object \n");
		sys_exit(-1);
	}

	DEBUG_OUT(("%s %d: System object created ok. Initiating system components...", __func__, __LINE__));
	
	if (Sys_InitSystem(global_system) == false)
	{
		DEBUG_OUT(("%s %d: Couldn't initialize the system", __func__, __LINE__));
		Sys_Exit(&global_system, PARAM_EXIT_ON_ERROR);
	}

	DEBUG_OUT(("%s %d: Setting graphics mode...", __func__, __LINE__));

	
	Sys_SetGraphicMode(global_system, PARAM_SPRITES_OFF, PARAM_BITMAP_ON, PARAM_TILES_OFF, PARAM_TEXT_OVERLAY_ON, PARAM_TEXT_ON);
	printf("now in graphics mode with text overlay");


	MU_RUN_SUITE(test_suite_units);
// 	MU_RUN_SUITE(test_suite_speed);
	MU_REPORT();

	Sys_SetModeText(global_system, false);
	printf("now in normal text mode");

	Sys_Exit(&global_system, PARAM_EXIT_NO_ERROR);
	
	return MU_EXIT_CODE;
}
