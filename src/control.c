/*
 * control.c
 *
 *  Created on: Mar 20, 2022
 *      Author: micahbly
 */





/*****************************************************************************/
/*                                Includes                                   */
/*****************************************************************************/

// project includes
#include "bitmap.h"
#include "control.h"
#include "control_template.h"
#include "debug.h"
#include "font.h"
#include "general.h"
#include "sys.h"
#include "text.h"
#include "window.h"

// C includes
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// A2560 includes
#include <mcp/syscalls.h>
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

extern System*			global_system;


/*****************************************************************************/
/*                       Private Function Prototypes                         */
/*****************************************************************************/

//! Draws the control's caption text into the control's available space using the parent window's bitmaps' current font
//! If the caption cannot fit in its entirety, it will be truncated
//! This is a private function; it is the calling function's responsibility to ensure the window's bitmap is set to the desired font before calling.
//! @param	the_control -- a valid pointer to a Control with a non-NULL caption
static void Control_DrawCaption(Control* the_control);


// **** Debug functions *****

void Control_Print(Control* the_control);

/*****************************************************************************/
/*                       Private Function Definitions                        */
/*****************************************************************************/

//! Draws the control's caption text into the control's available space using the parent window's bitmaps' current font
//! If the caption cannot fit in its entirety, it will be truncated
//! This is a private function; it is the calling function's responsibility to ensure the window's bitmap is set to the desired font before calling.
//! @param	the_control -- a valid pointer to a Control with a non-NULL caption
static void Control_DrawCaption(Control* the_control)
{
	Theme*		the_theme;
	Font*		the_font;
	int16_t		available_width;
	int16_t		x_offset;
	int16_t		x;
	int16_t		y;
	int16_t		chars_that_fit;
	int16_t		pixels_used;
	uint8_t		font_color;

	if (the_control == NULL)
	{
		LOG_ERR(("%s %d: passed class object was null", __func__ , __LINE__));
		goto error;
	}

	// Draw control caption with parent window's current font. 
// 	// Assumption is that all controls with text are going to be rendered one after another, so getting/setting font each time is wasteful.
// 	//   If this assumption proves to be false, re-enable the font selection here. 
	// caption is to be drawn centered vertically within the control Rect
	// caption is to be clipped if too long to fit horizontally

	the_theme = Sys_GetTheme(global_system);
	the_font = Sys_GetSystemFont(global_system);

	if (Bitmap_SetFont(the_control->parent_win_->bitmap_, the_font) == false)
	{
		DEBUG_OUT(("%s %d: Couldn't get the system font and assign it to bitmap", __func__, __LINE__));
		goto error;
	}
	
	available_width = the_control->avail_text_width_;	
	chars_that_fit = Font_MeasureStringWidth(the_font, the_control->caption_, GEN_NO_STRLEN_CAP, available_width, 0, &pixels_used);
	//DEBUG_OUT(("%s %d: available_width=%i, chars_that_fit=%i, text='%s', pixels_used=%i", __func__, __LINE__, available_width, chars_that_fit, the_control->caption_, pixels_used));

	x_offset = the_control->rect_.MinX + (the_control->width_ - the_control->avail_text_width_) / 2; // potentially, this could be problematic if a theme designer set up a theme with right width 10, left width 2. 
	x = x_offset + (available_width - pixels_used) / 2;
	//y = the_control->rect_.MinY + (the_control->rect_.MaxY - the_control->rect_.MinY + the_font->nDescent) / 2 - 1;
	y = the_control->rect_.MinY + (the_font->fRectHeight - the_font->ascent);
	//DEBUG_OUT(("%s %d: available_width=%i, x_offset=%i, x=%i, y=%i", __func__, __LINE__, available_width, x_offset, x, y));

	if (the_control->active_)
	{
		if (the_control->pressed_)
		{
			font_color = the_theme->standard_back_color_; // fore/back colors expected to be inversed when pressed.
		}
		else
		{
			font_color = the_theme->standard_fore_color_;
		}
	}
	else
	{
		if (the_control->pressed_)
		{
			font_color = the_theme->highlight_fore_color_;
		}
		else
		{
			font_color = the_theme->inactive_fore_color_;
		}
	}
	
	Bitmap_SetColor(the_control->parent_win_->bitmap_, font_color);
	Bitmap_SetXY(the_control->parent_win_->bitmap_, x, y);

	if (Font_DrawString(the_control->parent_win_->bitmap_, the_control->caption_, chars_that_fit) == false)
	{
		DEBUG_OUT(("%s %d: font draw returned false; chars_that_fit=%i, text='%s'", __func__, __LINE__, chars_that_fit, the_control->caption_));
		goto error;
	}
	
	return;
	
error:
	Sys_Exit(&global_system, PARAM_EXIT_ON_ERROR);	// crash early, crash often
	return;
}




// **** Debug functions *****

void Control_Print(Control* the_control)
{
	DEBUG_OUT(("Control print out:"));
	DEBUG_OUT(("  id_: %u",	 				the_control->id_));
	DEBUG_OUT(("  type_: %i",	 			the_control->type_));
	DEBUG_OUT(("  group_id_: %i",	 		the_control->group_id_));
	DEBUG_OUT(("  next_: %p",				the_control->next_));
	DEBUG_OUT(("  parent_win_: %p",			the_control->parent_win_));
	DEBUG_OUT(("  parent_rect_: %i, %i, %i, %i",	the_control->parent_rect_->MinX, the_control->parent_rect_->MinY, the_control->parent_rect_->MaxX, the_control->parent_rect_->MaxY));
	DEBUG_OUT(("  rect_: %i, %i, %i, %i",	the_control->rect_.MinX, the_control->rect_.MinY, the_control->rect_.MaxX, the_control->rect_.MaxY));
	DEBUG_OUT(("  h_align_: %i", 			the_control->h_align_));
	DEBUG_OUT(("  v_align_: %i",			the_control->v_align_));
	DEBUG_OUT(("  x_offset_: %i",			the_control->x_offset_));
	DEBUG_OUT(("  y_offset_: %i", 			the_control->y_offset_));
	DEBUG_OUT(("  width_: %i", 				the_control->width_));
	DEBUG_OUT(("  height_: %i", 			the_control->height_));
	DEBUG_OUT(("  visible_: %i", 			the_control->visible_));
	DEBUG_OUT(("  active_: %i", 			the_control->active_));
	DEBUG_OUT(("  enabled_: %i", 			the_control->enabled_));
	DEBUG_OUT(("  pressed_: %i", 			the_control->pressed_));
	DEBUG_OUT(("  invalidated_: %i", 		the_control->invalidated_));
	DEBUG_OUT(("  value_: %i",	 			the_control->value_));
	DEBUG_OUT(("  min_: %i",	 			the_control->min_));
	DEBUG_OUT(("  max_: %i", 				the_control->max_));
	DEBUG_OUT(("  inactive image up: %p", 	the_control->image_[CONTROL_INACTIVE][CONTROL_NOT_PRESSED]));
	DEBUG_OUT(("  inactive image dn: %p", 	the_control->image_[CONTROL_INACTIVE][CONTROL_PRESSED]));
	DEBUG_OUT(("  active image up: %p", 	the_control->image_[CONTROL_ACTIVE][CONTROL_NOT_PRESSED]));
	DEBUG_OUT(("  active image dn: %p", 	the_control->image_[CONTROL_ACTIVE][CONTROL_PRESSED]));
	DEBUG_OUT(("  caption_: %p", 			the_control->caption_));	
	DEBUG_OUT(("  avail_text_width_: %i", 	the_control->avail_text_width_));
}


/*****************************************************************************/
/*                        Public Function Definitions                        */
/*****************************************************************************/

// **** CONSTRUCTOR AND DESTRUCTOR *****

// constructor
//! Allocate a Control object
//! @param	the_template -- reference to a valid, populated ControlTemplate object. The created control will take most of its properties from this template.
//! @param	the_window -- reference to a valid Window object. The newly-created control will not be added to the window's list of controls, but the control will remember this window as its parent
//! @param	the_parent_rect -- Reference to rect object that the control will position itself relative to. This rect must remain valid throughout the life of the control.
//! @param	control_id -- the unique ID (within the specified window) to be assigned to the control. WARNING: assigning multiple controls the same ID will result in undefined behavior.
//! @param	group_id -- group ID value to be assigned to the control. Pass CONTROL_NO_GROUP if the control is not to be part of a group.
//! @return	Returns a new Control object that has been localized to the passed parent rect. Returns NULL on any error condition.
Control* Control_New(ControlTemplate* the_template, Window* the_window, Rectangle* the_parent_rect, int16_t control_id, int16_t group_id)
{
	Control*		the_control = NULL;

	if ( the_template == NULL)
	{
		LOG_ERR(("%s %d: passed template was NULL", __func__ , __LINE__));
		goto error;
	}

	if ( the_window == NULL)
	{
		LOG_ERR(("%s %d: passed parent window was NULL", __func__ , __LINE__));
		goto error;
	}

	if ( the_parent_rect == NULL)
	{
		LOG_ERR(("%s %d: passed parent rect was NULL", __func__ , __LINE__));
		goto error;
	}
		
	// LOGIC:
	//   the control template will contain most of the information needed to establish the Control
	//   to personalize the control for a given window, the parent window is needed
	//   the final location of the control is calculated based on the offset info in the template + the size of the parent window
	
	if ( (the_control = (Control*)calloc(1, sizeof(Control)) ) == NULL)
	{
		LOG_ERR(("%s %d: could not allocate memory to create new control record", __func__ , __LINE__));
		goto error;
	}
	LOG_ALLOC(("%s %d:	__ALLOC__	the_control	%p	size	%i", __func__ , __LINE__, the_control, sizeof(Control)));
	TRACK_ALLOC((sizeof(Control)));

	// copy caption; not all controls will have a caption
	if (the_template->caption_ != NULL)
	{
		if ( (the_control->caption_ = General_StrlcpyWithAlloc(the_template->caption_, CONTROL_MAX_CAPTION_SIZE)) == NULL)
		{
			LOG_ERR(("%s %d: could not allocate memory for the control's caption string", __func__ , __LINE__));
			goto error;
		}
		//DEBUG_OUT(("%s %d:	__ALLOC__	the_control->caption_	%p	size	%i		'%s'", __func__ , __LINE__, the_control->caption_, General_Strnlen(the_control->caption_, CONTROL_MAX_CAPTION_SIZE) + 1, the_control->caption_));
	}
	else
	{
		the_control->caption_ = NULL;
	}

	// copy template info in before localizing to the window
	the_control->type_ = the_template->type_;
	the_control->h_align_ = the_template->h_align_;
	the_control->v_align_ = the_template->v_align_;
	the_control->x_offset_ = the_template->x_offset_;
	the_control->y_offset_ = the_template->y_offset_;
	the_control->width_ = the_template->width_;
	the_control->height_ = the_template->height_;
	the_control->min_ = the_template->min_;
	the_control->max_ = the_template->max_;
	the_control->image_[CONTROL_INACTIVE][CONTROL_NOT_PRESSED] = the_template->image_[CONTROL_INACTIVE][CONTROL_NOT_PRESSED];
	the_control->image_[CONTROL_INACTIVE][CONTROL_PRESSED] = the_template->image_[CONTROL_INACTIVE][CONTROL_PRESSED];
	the_control->image_[CONTROL_ACTIVE][CONTROL_NOT_PRESSED] = the_template->image_[CONTROL_ACTIVE][CONTROL_NOT_PRESSED];
	the_control->image_[CONTROL_ACTIVE][CONTROL_PRESSED] = the_template->image_[CONTROL_ACTIVE][CONTROL_PRESSED];
	the_control->avail_text_width_ = the_template->avail_text_width_;
	
	// at start, all new controls are inactive, value 0, disabled, not-pressed, and invisible
	the_control->visible_ = false;
	the_control->active_ = false;
	the_control->enabled_ = false;
	the_control->pressed_ = false;
	the_control->invalidated_ = true;

	the_control->value_ = 0;
	
	// localize to the parent window
	the_control->id_ = control_id;
	the_control->group_id_ = group_id;
	the_control->parent_win_ = the_window;
	the_control->parent_rect_ = the_parent_rect;
	Control_AlignToParentRect(the_control);
	
	//Control_Print(the_control);
	
	return the_control;
	
error:
	if (the_control)					Control_Destroy(&the_control);
	Sys_Exit(&global_system, PARAM_EXIT_ON_ERROR);	// crash early, crash often
	return NULL;
}


// destructor
//! Frees all allocated memory associated with the passed object, and the object itself
//! @param	the_control -- pointer to the pointer for the Control object to be destroyed
//! @return	Returns false if the pointer to the passed control was NULL
bool Control_Destroy(Control** the_control)
{
	if (*the_control == NULL)
	{
		LOG_ERR(("%s %d: passed class object was null", __func__ , __LINE__));
		goto error;
	}

	if ((*the_control)->caption_ != NULL)
	{
		LOG_ALLOC(("%s %d:	__FREE__	(*the_control)->caption_	%p	size	%i		'%s'", __func__ , __LINE__, (*the_control)->caption_, General_Strnlen((*the_control)->caption_, CONTROL_MAX_CAPTION_SIZE) + 1, (*the_control)->caption_));
		TRACK_ALLOC((0 - General_Strnlen((*the_control)->caption_, CONTROL_MAX_CAPTION_SIZE) + 1));
		free((*the_control)->caption_);
		(*the_control)->caption_ = NULL;
	}
	
	LOG_ALLOC(("%s %d:	__FREE__	*the_control	%p	size	%i", __func__ , __LINE__, *the_control, sizeof(Control)));
	TRACK_ALLOC((0 - sizeof(Control)));
	free(*the_control);
	*the_control = NULL;
	
	return true;
	
error:
	Sys_Exit(&global_system, PARAM_EXIT_ON_ERROR);	// crash early, crash often
	return false;
}





// **** xxx functions *****

//! Updates the passed control with new theme info from the passed control template
//! Call this when the theme has been changed
//! It allows existing controls to be updated in place, without having to free them and create new theme controls
//! @param	the_control -- a valid Control object
//! @param	the_template -- a valid ControlTemplate object to update the Control object from.
//! @return	Returns false on any error
bool Control_UpdateFromTemplate(Control* the_control, ControlTemplate* the_template)
{
	if ( the_control == NULL)
	{
		LOG_ERR(("%s %d: passed class object was NULL", __func__ , __LINE__));
		goto error;
	}

	if ( the_template == NULL)
	{
		LOG_ERR(("%s %d: passed template was NULL", __func__ , __LINE__));
		goto error;
	}

	// LOGIC:
	//   the control template will contain most of the information needed to establish the Control
	//   we already have a working control, and we don't need to change the type, visibility, active/inactive, etc. 
	//   we want to change only the properties that might have changed due to a change in theme.
	
	// copy template info in before localizing to the window
	the_control->h_align_ = the_template->h_align_;
	the_control->v_align_ = the_template->v_align_;
	the_control->x_offset_ = the_template->x_offset_;
	the_control->y_offset_ = the_template->y_offset_;
	the_control->width_ = the_template->width_;
	the_control->height_ = the_template->height_;
	the_control->avail_text_width_ = the_template->avail_text_width_;

	// do NOT free old images, they didn't really belong to the control, they belonged to the previous theme
	the_control->image_[CONTROL_INACTIVE][CONTROL_NOT_PRESSED] = the_template->image_[CONTROL_INACTIVE][CONTROL_NOT_PRESSED];
	the_control->image_[CONTROL_INACTIVE][CONTROL_PRESSED] = the_template->image_[CONTROL_INACTIVE][CONTROL_PRESSED];
	the_control->image_[CONTROL_ACTIVE][CONTROL_NOT_PRESSED] = the_template->image_[CONTROL_ACTIVE][CONTROL_NOT_PRESSED];
	the_control->image_[CONTROL_ACTIVE][CONTROL_PRESSED] = the_template->image_[CONTROL_ACTIVE][CONTROL_PRESSED];
		
	// localize to the parent window
	Control_AlignToParentRect(the_control);

	// ensure it will get rendered in next pass
	the_control->invalidated_ = true;
	
	//Control_Print(the_control);
	
	return true;
	
error:
	Sys_Exit(&global_system, PARAM_EXIT_ON_ERROR);	// crash early, crash often
	return false;
}



// **** Set functions *****

//! Links the control to the next control passed
//! @param	the_control -- a valid Control object
//! @param	the_next_control -- a valid Control object to be set as the next control from the_control. Can be NULL.
//! @return	Returns false on any error
bool Control_SetNextControl(Control* the_control, Control* the_next_control)
{
	if (the_control == NULL)
	{
		LOG_ERR(("%s %d: passed class object was null", __func__ , __LINE__));
		goto error;
	}
	
	the_control->next_ = the_next_control;
	
	return true;
	
error:
	Sys_Exit(&global_system, PARAM_EXIT_ON_ERROR);	// crash early, crash often
	return false;
}


//! Set the control's active/inactive state
//! @param	the_control -- a valid Control object
//! @param	is_active -- set to true to set control to active, false to set to inactive
void Control_SetActive(Control* the_control, bool is_active)
{
	if (the_control == NULL)
	{
		LOG_ERR(("%s %d: passed class object was null", __func__ , __LINE__));
		goto error;
	}
	
	the_control->active_ = is_active;

	// ensure it will get rendered in next pass
	the_control->invalidated_ = true;
	
	return;
	
error:
	Sys_Exit(&global_system, PARAM_EXIT_ON_ERROR);	// crash early, crash often
	return;
}


//! Set the control's pressed/unpressed state
//! @param	the_control -- a valid Control object
//! @param	is_pressed -- set to true to set control state to pressed, false to set to not-pressed
void Control_SetPressed(Control* the_control, bool is_pressed)
{
	if (the_control == NULL)
	{
		LOG_ERR(("%s %d: passed class object was null", __func__ , __LINE__));
		goto error;
	}
	
	the_control->pressed_ = is_pressed;

	// ensure it will get rendered in next pass
	the_control->invalidated_ = true;
	
	return;
	
error:
	Sys_Exit(&global_system, PARAM_EXIT_ON_ERROR);	// crash early, crash often
	return;
}


//! Mark the specified control is invalidated or validated
//! Note: Marking a control as invalidated causes it to be added to the parent window's list of rects to be reblitted to the screen in next render pass
//! @param	the_control -- a valid Control object
//! @param	invalidated -- set to true to set control state to invalidated (will need redraw), false to set to not-invalidated
void Control_MarkInvalidated(Control* the_control, bool invalidated)
{
	if (the_control == NULL)
	{
		LOG_ERR(("%s %d: passed class object was null", __func__ , __LINE__));
		goto error;
	}
	
	the_control->invalidated_ = invalidated;
	
	return;
	
error:
	Sys_Exit(&global_system, PARAM_EXIT_ON_ERROR);	// crash early, crash often
	return;
}


//! Set or uppdate the control's position and/or size as appropriate to the control's parent rect
//! Call when parent window size has changed, when control is first created, etc.
//! @param	the_control -- a valid Control object
void Control_AlignToParentRect(Control* the_control)
{
	// LOGIC:
	//   Controls have an h align and v align choice, and x and y offsets.
	//   Based on those choices, and the parent window's dimensions, 
	//     the control needs its' rect_ property adjusted to match where it should render
	//   rect_ values are relative to the control's parent window, but calculated relative to the parent rect
	//     for close/max/min/norm, this will be the window's titlebar rect
	//       (not to the window itself -- this allows them to render at bottom of window if that's where titlebar is)
	//     for iconbar controls, that will be the iconbar_rect_
	//     for any other control, that will be the contentarea_rect_
	
	if (the_control == NULL)
	{
		LOG_ERR(("%s %d: passed class object was null", __func__ , __LINE__));
		goto error;
	}
	
	if (the_control->parent_rect_ == NULL)
	{
		LOG_ERR(("%s %d: parent rect was null", __func__ , __LINE__));
		goto error;
	}

	if (the_control->h_align_ == H_ALIGN_LEFT)
	{
		the_control->rect_.MinX = the_control->parent_rect_->MinX + the_control->x_offset_;
	}
	else if (the_control->h_align_ == H_ALIGN_RIGHT)
	{
		the_control->rect_.MinX = the_control->parent_rect_->MaxX - the_control->x_offset_ - the_control->width_;
	}
	else
	{
		// center
		the_control->rect_.MinX = the_control->parent_rect_->MinX + ((the_control->parent_rect_->MaxX - the_control->parent_rect_->MinX - the_control->width_) / 2);		
	}
	
	the_control->rect_.MaxX = the_control->rect_.MinX + the_control->width_;
	
	if (the_control->v_align_ == V_ALIGN_TOP)
	{
		the_control->rect_.MinY = the_control->parent_rect_->MinY + the_control->y_offset_;
	}
	else if (the_control->v_align_ == V_ALIGN_BOTTOM)
	{
		the_control->rect_.MinY = the_control->parent_rect_->MaxY - the_control->y_offset_ -  the_control->height_;
	}
	else
	{
		// center
		the_control->rect_.MinY = the_control->parent_rect_->MinY + ((the_control->parent_rect_->MaxY - the_control->parent_rect_->MinY - the_control->height_) / 2);		
	}
	
	the_control->rect_.MaxY = the_control->rect_.MinY + the_control->height_;
	
	// ensure it will get rendered in next pass
	the_control->invalidated_ = true;
	
	//DEBUG_OUT(("%s %d: Control after AlignToWindow...", __func__, __LINE__));
	//Control_Print(the_control);
	
	return;
	
error:
	Sys_Exit(&global_system, PARAM_EXIT_ON_ERROR);	// crash early, crash often
	return;
}










// **** Get functions *****


//! Get the ID of the control
//! @param	the_control -- a valid Control object
//! @return	Returns the ID of the passed control, or -1 in any error condition
int16_t Control_GetID(Control* the_control)
{
	if (the_control == NULL)
	{
		LOG_ERR(("%s %d: passed class object was null", __func__ , __LINE__));
		goto error;
	}
	
	return the_control->id_;
	
error:
	Sys_Exit(&global_system, PARAM_EXIT_ON_ERROR);	// crash early, crash often
	return -1;
}


//! Get the next control in the chain
//! @param	the_control -- a valid Control object
//! @return	Returns the control object linked as the next control of the passed control. Returns NULL if there is no next control, or if the next control is itself NULL, or on any error. The last control in a chain will return NULL.
Control* Control_GetNextControl(Control* the_control)
{
	if (the_control == NULL)
	{
		LOG_ERR(("%s %d: passed class object was null", __func__ , __LINE__));
		goto error;
	}
	
	return the_control->next_;
	
error:
	Sys_Exit(&global_system, PARAM_EXIT_ON_ERROR);	// crash early, crash often
	return NULL;
}


//! Get the control type
//! @param	the_control -- a valid Control object
//! @return	Returns the control_type value of the passed control, or CONTROL_TYPE_ERROR (-1) on any error
control_type Control_GetType(Control* the_control)
{
	if (the_control == NULL)
	{
		LOG_ERR(("%s %d: passed class object was null", __func__ , __LINE__));
		goto error;
	}
	
	return the_control->type_;
	
error:
	Sys_Exit(&global_system, PARAM_EXIT_ON_ERROR);	// crash early, crash often
	return CONTROL_TYPE_ERROR;
}


//! Get the pressed/not pressed state
//! @param	the_control -- a valid Control object
//! @return	Returns true if control is pressed (down), false if up
bool Control_GetPressed(Control* the_control)
{
	if (the_control == NULL)
	{
		LOG_ERR(("%s %d: passed class object was null", __func__ , __LINE__));
		goto error;
	}
	
	return the_control->pressed_;
	
error:
	Sys_Exit(&global_system, PARAM_EXIT_ON_ERROR);	// crash early, crash often
	return false;
}


//! Compare the control's right-edge coordinate to the passed value
//! If the control is more to the right than the passed value, the passed value is updated with the control's right edge
//! @param	the_control -- a valid Control object
//! @param	x -- a pointer to a bitmap horizontal position. May be changed by the function.
//! @return	Returns true if the control is further to the right than the passed value.
bool Control_IsRighter(Control* the_control, int16_t* x)
{
	if (the_control == NULL)
	{
		LOG_ERR(("%s %d: passed class object was null", __func__ , __LINE__));
		goto error;
	}
	
	if (the_control->rect_.MaxX > *x)
	{
		*x = the_control->rect_.MaxX;
		return true;
	}
	
	return false;
	
error:
	Sys_Exit(&global_system, PARAM_EXIT_ON_ERROR);	// crash early, crash often
	return false;
}

//! Compare the control's left-edge coordinate to the passed value
//! If the control is more to the left than the passed value, the passed value is updated with the control's left edge
//! @param	the_control -- a valid Control object
//! @param	x -- a pointer to a bitmap horizontal position. May be changed by the function.
//! @return	Returns true if the control is further to the left than the passed value.
bool Control_IsLefter(Control* the_control, int16_t* x)
{
	if (the_control == NULL)
	{
		LOG_ERR(("%s %d: passed class object was null", __func__ , __LINE__));
		goto error;
	}
	
	if (the_control->rect_.MinX < *x)
	{
		*x = the_control->rect_.MinX;
		return true;
	}
	
	return false;
	
error:
	Sys_Exit(&global_system, PARAM_EXIT_ON_ERROR);	// crash early, crash often
	return false;
}



// **** Render functions *****

//! Blits the control to the control's parent window if the control is visible, and if it is listed as invalidated
//! Controls that are visible but not invalidated do not need redrawing.
//! Also draws the caption of the control, if it has one.
//! Marks control as no longer invalidated.
//! @param	the_control -- a valid Control object
void Control_Render(Control* the_control)
{
	Bitmap*				the_bitmap;
	
	if (the_control == NULL)
	{
		LOG_ERR(("%s %d: passed class object was null", __func__ , __LINE__));
		goto error;
	}
	
	if (the_control->parent_win_ == NULL)
	{
		LOG_ERR(("%s %d: control's parent window object was null", __func__ , __LINE__));
		goto error;
	}
	
	// LOGIC: 
	//   For an individual control, "render" consists of blitting it's bitmap onto the parent window's bitmap
	//   Depending on the state of the control, one of 3 bitmaps will be blitted to the parent window
	//   If control is set to invisible, none will be rendered
	//   if a control has not been invalidated since last render, it will not be redrawn/rendered
	
	if (the_control->visible_ == false || the_control->invalidated_ == false)
	{
		return;
	}
	
	the_bitmap = the_control->image_[the_control->active_][the_control->pressed_];
	//the_bitmap = the_control->image_[1][1];

	//DEBUG_OUT(("%s %d: about to blit control %p to parent window bitmap", __func__, __LINE__, the_control));
	//DEBUG_OUT(("%s %d: pbitmap w/h=%i, %i; this MinX/MinY=%i, %i", __func__, __LINE__, the_control->parent_->bitmap_->width_, the_control->parent_->bitmap_->height_, the_control->rect_.MinX, the_control->rect_.MinY));
	//DEBUG_OUT(("%s %d: control type=%i, active=%i, pressed=%i", __func__, __LINE__, the_control->type_, the_control->active_, the_control->pressed_));
	//Control_Print(the_control);

	// LOGIC: the control's rect is the in-window coordinates, not a 0,0xheight,width rect local to the Control
	
	Bitmap_Blit(the_bitmap, 0, 0, 
				the_control->parent_win_->bitmap_, 
				the_control->rect_.MinX, 
				the_control->rect_.MinY, 
				the_control->width_, 
				the_control->height_
				);
				
	// some controls have captions. if present, draw them directly to the parent bitmap
	// (leave the control's bitmaps clean, so text can be changed, font changed, etc.)
	if (the_control->caption_ != NULL)
	{
		Control_DrawCaption(the_control);
	}
	
	// mark control as non-invalidated
	the_control->invalidated_ = false;
	
	// add this control's rect to the list of clip rects that need to be blitted from window to screen
	Window_AddClipRect(the_control->parent_win_, &the_control->rect_);
	
	return;
	
error:
	Sys_Exit(&global_system, PARAM_EXIT_ON_ERROR);	// crash early, crash often
	return;
}



