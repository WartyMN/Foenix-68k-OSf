/*
 * window_test.c
 *
 *  Created on: Mar 19, 2022
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
#include "window.h"

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

// handler for the hello world window
void HelloWindowEventHandler(EventRecord* the_event);


/*****************************************************************************/
/*                       Private Function Definitions                        */
/*****************************************************************************/

// handler for the hello world window
void HelloWindowEventHandler(EventRecord* the_event)
{
	DEBUG_OUT(("%s %d: event received!", __func__, __LINE__));
	
	return;
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


// test for memory corruption byu comparing known values for bitmap images to actual ram (not VRAM, can't test that) 
MU_TEST(unit_test_1)
{
	Window*				the_window;
	NewWinTemplate*		the_win_template;
	static char*		the_win_title = "My New Window";
	
	if ( (the_win_template = Window_GetNewWinTemplate(the_win_title)) == NULL)
	{
		LOG_ERR(("%s %d: Could not get a new window template", __func__ , __LINE__));
		return;
	}	
	// note: all the default values are fine for us in this case.
	
	DEBUG_OUT(("%s %d: x=%i, y=%i, width=%i, title='%s'", __func__, __LINE__, the_win_template->x_, the_win_template->y_, the_win_template->width_, the_win_template->title_));
	
	if ( (the_window = Window_New(the_win_template, &HelloWindowEventHandler)) == NULL)
	{
		DEBUG_OUT(("%s %d: Couldn't instantiate a window", __func__, __LINE__));
		return;
	}

	// say hi
	Window_SetPenXY(the_window, 5, 5);

	if (Window_DrawString(the_window, (char*)"Hello, World", GEN_NO_STRLEN_CAP) == false)
	{
		// oh, no! you should handle this.
		printf("Could not do Window_DrawString \n");
	}
	

	// draw some stuff in the window we created
	{
		// draw some color blocks
		int16_t i;
		int16_t x = 1;
		int16_t y = the_window->content_rect_.MinY + 50;
		int16_t height = 16;
	
		// chromatic
		for (i=1; i<(256-41); i++)
		{
			Bitmap_FillBox(the_window->bitmap_, x, y, 2, height, i);
			x += 2;
		
			if (i % 36 == 0)
			{
				x = 1;
				y += height;
			}
		}

		// reds > greens > blues > grays
		x = 1;
		y += height;

		for (; i<256; i++)
		{
			Bitmap_FillBox(the_window->bitmap_, x, y, 2, height, i);
			x += 2;
		}
	}

	// declare the window to be visible
	Window_SetVisible(the_window, true);
	
	
// 	// Failure: could not alloc space for screen buffer 1
// 	mu_check( (buffer1 = (char*)calloc(global_system->screen_[ID_CHANNEL_A]->text_mem_cols_ * global_system->screen_[ID_CHANNEL_A]->text_mem_rows_, sizeof(char)) ) != NULL );
//  	// Failure: could not alloc space for screen buffer 2
// 	mu_check( (buffer2 = (char*)calloc(global_system->screen_[ID_CHANNEL_A]->text_mem_cols_ * global_system->screen_[ID_CHANNEL_A]->text_mem_rows_, sizeof(char)) ) != NULL );

	
	Window_Destroy(&the_window);
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
	
// 	MU_RUN_TEST(unit_test_1);
}




/*****************************************************************************/
/*                        Public Function Definitions                        */
/*****************************************************************************/



int main(int argc, char* argv[])
{
	printf("**** window.c Test Suite **** \n");

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
