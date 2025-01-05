/*
 * hello.c
 *
 *  Created on: Jan 5, 2025
 *      Author: micahbly
 *
 *  A Hello World program using the windowing environment
 *
 */


/*****************************************************************************/
/*                                Includes                                   */
/*****************************************************************************/


// project includes
#include "debug.h"
// #include "event.h"
#include "general.h"
#include "sys.h"

// C includes
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>	// just for initiating rand()

// A2560 includes
#include "a2560k.h"
#include <mcp/syscalls.h>


/*****************************************************************************/
/*                               Definitions                                 */
/*****************************************************************************/


/*****************************************************************************/
/*                             Global Variables                              */
/*****************************************************************************/

System*			global_system;



/*****************************************************************************/
/*                       Private Function Prototypes                         */
/*****************************************************************************/

// example of adding a control to a window
void AddControls(Window* the_window);

// open a basic window
void OpenAWindow(void);

/*****************************************************************************/
/*                       Private Function Definitions                        */
/*****************************************************************************/


// example of adding a control to a window
void AddControls(Window* the_window)
{
	Control*			the_label;
	int16_t				x_offset;
	int16_t				y_offset;
	int16_t				width;
	int16_t				height;
	int16_t				the_id;
	int16_t				group_id;
	char				caption_buff[CONTROL_MAX_CAPTION_SIZE];
	char*				caption = caption_buff;
	
	the_id = 1000;	// arbitrary, for use of programmer. manage them though, and keep them unique within any one window
	group_id = 0;	// will not be grouping this label with any other label	
	height = 15;	// height of labels is whatever you want, as they are not "flexible" controls in the way the buttons, etc., are.
	width = 110;
	x_offset = 10;	// local coords within the parent window, not global screen coords
	y_offset = 15;	// local coords within the parent window, not global screen coords

	General_Strlcpy(caption, "Hello World", CONTROL_MAX_CAPTION_SIZE);
	the_label = Window_AddNewControl(the_window, LABEL, width, height, x_offset, y_offset, H_ALIGN_LEFT, V_ALIGN_TOP, caption, the_id++, group_id);
}


// open a basic window
void OpenAWindow(void)
{
	Window*				the_window;
	NewWinTemplate*		the_win_template;
	char*				the_win_title = (char*)"Hello";
	int16_t				win_orig_x = 10;
	int16_t				win_orig_y = 10;
	int16_t				max_width = 300;
	int16_t				max_height = 200;
	
	if ( (the_win_template = Window_GetNewWinTemplate(the_win_title)) == NULL)
	{
		LOG_ERR(("%s %d: Could not get a new window template", __func__ , __LINE__));
		return;
	}	
	// note: all the default values are fine for us in this case.
		
	the_win_template->x_ = win_orig_x;
	the_win_template->y_ = win_orig_y;
	the_win_template->width_ = max_width;
	the_win_template->height_ = max_height;

	if ( (the_window = Window_New(the_win_template, NULL)) == NULL)
	{
		DEBUG_OUT(("%s %d: Couldn't instantiate a window", __func__, __LINE__));
		return;
	}
		
	// add controls to the window
	AddControls(the_window);

	// declare the window to be visible
	Window_SetVisible(the_window, true);

	// temporary until event handler is written: tell system to render the screen and all windows
	Sys_Render(global_system);
}


/*****************************************************************************/
/*                        Public Function Definitions                        */
/*****************************************************************************/

int main(int argc, char* argv[])
{
	Screen*		the_screen;

	// initialize the system object
	if ((global_system = Sys_New()) == NULL)
	{
		sys_exit(-1);
	}

	if (Sys_InitSystem(global_system) == false)
	{
		Sys_Exit(&global_system, PARAM_EXIT_ON_ERROR);
	}
	
	Sys_SetGraphicMode(global_system, PARAM_SPRITES_OFF, PARAM_BITMAP_ON, PARAM_TILES_OFF, PARAM_TEXT_OVERLAY_OFF, PARAM_TEXT_OFF);

	OpenAWindow();
	
	// when not using event manager: tell system to render the screen and all windows
	Sys_Render(global_system);

	// wait for user to do something
	General_GetChar();

 	Sys_SetModeText(global_system, false);
	
	Sys_Exit(&global_system, PARAM_EXIT_NO_ERROR);
	
	return 0;
}
