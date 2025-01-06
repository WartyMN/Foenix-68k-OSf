//! @file control.h

/*
 * control.h
 *
*  Created on: Mar 20, 2022
 *      Author: micahbly
 */

#ifndef CONTROL_H_
#define CONTROL_H_


/* about this class: Control
 *
 * Provides structures and functions for managing window controls such as close widgets, buttons, sliders, etc.
 *
 *** things this class needs to be able to do
 * Create a control instance from a proto-control (control template struct)
 * Toggle a control between active to inactive state
 * Tell a control to reposition itself on the window
 * Toggle a control's visibility
 * Render a control onto its parent bitmap
 * Get and Set the ID of a control
 * Get and Set the caption of the control (if any)
 * Get and Set the state of the control
 * Get and Set the max, minimum, and current values of the control
 * 
 *
 * STRETCH GOALS
 * 
 *
 * SUPER STRETCH GOALS
 * 
 * 
 */


/*****************************************************************************/
/*                                Includes                                   */
/*****************************************************************************/

// project includes


// C includes
#include <stdbool.h>


// A2560 includes
#include <mcp/syscalls.h>
#include "a2560k.h"


/*****************************************************************************/
/*                            Macro Definitions                              */
/*****************************************************************************/

#define CONTROL_NO_GROUP			-1	//! For any given control's group_ value, this denotes a control that is not part of any other group
#define CONTROL_MAX_CAPTION_SIZE	128	//! The maximum length (including terminator) for the caption string

#define CONTROL_ID_ERROR			-2	//! For any function trying to return the ID of a control, a value indicating an error occurred. This error must be handled.
#define CONTROL_ID_NOT_FOUND		-1	//! For any function trying to return the ID of a control, a value indicating the that the described control could not be found.


/*****************************************************************************/
/*                               Enumerations                                */
/*****************************************************************************/

typedef enum control_active_state
{
	CONTROL_INACTIVE = 0,
	CONTROL_ACTIVE = 1,
} control_active_state;

typedef enum control_pushed_state
{
	CONTROL_NOT_PRESSED = 0,
	CONTROL_PRESSED = 1,
} control_pushed_state;

typedef enum control_type
{
	CONTROL_TYPE_ERROR = -1,	//! for functions that return a control type, this value will be returned in the event of an error
	TEXT_BUTTON			= 0,	//! flexible-width button with text caption
	TEXT_FIELD			= 1,	//! flexible-width single-line text input field
	RADIO_BUTTON	,		//! mutually-exclusive radio button control, assign each control in the set the same group id
	CHECKBOX 		,		//! checkbox control. do not assign the same group id to each control, as checkboxes are not mutually exclusive.
	TITLEBAR		,		//! the titlebar of a window
	TEXT_BOX		,		//! multi-line text input field
	IMAGE_BUTTON	,		//! a fixed height/width button control with no caption, up/down states, and a no defined shape in the theme, relying on the creating program to provide a bitmap.
	CLOSE_WIDGET	,		//! standard system control; the close widget of a window. do not add outside of the context of a window title bar.
	SIZE_MINIMIZE	,		//! standard system control; the minimize widget of a window. do not add outside of the context of a window title bar.
	SIZE_NORMAL		,		//! standard system control; the normal-size widget of a window. do not add outside of the context of a window title bar.
	SIZE_MAXIMIZE	,		//! standard system control; the maximize widget of a window. do not add outside of the context of a window title bar.
	H_SCROLLER		,		//! proportional control for handling horizontal scrolling within a defined area
	V_SCROLLER		,		//! proportional control for handling vertical scrolling within a defined area	
	LABEL			,		//! static text label
	FUTURE_GROW_L	,		//! not sure these will be treated as generic controls, but reserving the id
	FUTURE_GROW_R	,		//! not sure these will be treated as generic controls, but reserving the id
	FUTURE_GROW_UP	,		//! not sure these will be treated as generic controls, but reserving the id
	FUTURE_GROW_DN	,		//! not sure these will be treated as generic controls, but reserving the id
	RESERVED1x		,		//! progress bar?
	RESERVED2x		,		//! image?
	CUSTOM			,		//! usage TBD
} control_type;


typedef enum h_align_type
{
	H_ALIGN_LEFT	= 0,
	H_ALIGN_CENTER	= 1,
	H_ALIGN_RIGHT	= 2
} h_align_type;

typedef enum v_align_type
{
	V_ALIGN_TOP		= 0,
	V_ALIGN_CENTER	= 1,
	V_ALIGN_BOTTOM	= 2
} v_align_type;


/*****************************************************************************/
/*                                 Structs                                   */
/*****************************************************************************/

//! This structure describes an instantiated control on a window.
struct Control
{
	int16_t					group_id_;							//! group number, if this control is part of a group of controls. -1 (no group) is default. Typical use case: radio buttons.
	int16_t					id_;							//! used to identify the control in the control list, etc. 
	control_type			type_;							//! button vs checkbox vs radio button, etc. 
	Control*				next_;							//! next control in the list
	Window*					parent_win_;					//! parent window
	Rectangle*				parent_rect_;					//! parent rectangle (the window segment it belongs to: titlebar, contentarea, iconbar
	Rectangle				rect_;							//! coordinates relative to the parent window (ie, these are not global coordinates)
	h_align_type			h_align_;						//! whether the control should be positioned relative to the left side, right side, or centered
	v_align_type			v_align_;						//! whether the control should be positioned relative to the top edge, bottom edge, or centered
	int16_t					x_offset_;						//! horizontal coordinate relative to the parent window's left or right edge. If h_align_ is H_ALIGN_CENTER, this value will be ignored.
	int16_t					y_offset_;						//! vertical coordinate relative to the parent window's top or bottom edge. If v_align_ is V_ALIGN_CENTER, this value will be ignored.
	int16_t					width_;							//! width of the control
	int16_t					height_;						//! height of the control
	bool					visible_;
	bool					active_;						//! is the control activated or not (in appearance). Does not affect ability to receive events.
	bool					enabled_;						//! is the control enabled or not. If not enabled, it will not receive events.
	bool					pressed_;						//! is the control currently in a clicked/pushed/depressed state or not. Drives rendering choice. Does not affect ability to receive events.
	bool					invalidated_;					// if true, the control needs to be re-drawn and re-blitted in the next render pass
	int16_t					value_;							//! current value of the control
	int16_t					min_;							//! minimum allowed value
	int16_t					max_;							//! maximum allowed value
	Bitmap*					image_[2][2];					//! 4 image state bitmaps: [active yes/no][pushed down yes/no]
	char*					caption_;						//! optional string to draw centered horizontally and vertically on the control. Typical use cases include buttons and labels.
	int16_t					avail_text_width_;				//! number of pixels available for writing text. For flexible width buttons, etc., this excludes the left/right segments. 
// 	char*					hover_text_;					//! optional string to show in help/hover-text situations
};




/*****************************************************************************/
/*                             Global Variables                              */
/*****************************************************************************/


/*****************************************************************************/
/*                       Public Function Prototypes                          */
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
Control* Control_New(ControlTemplate* the_template, Window* the_window, Rectangle* the_parent_rect, int16_t control_id, int16_t group_id);

// destructor
// frees all allocated memory associated with the passed object, and the object itself
//! @param	the_control -- pointer to the pointer for the Control object to be destroyed
//! @return	Returns false if the pointer to the passed control was NULL
bool Control_Destroy(Control** the_control);






// **** xxx functions *****




// **** Set xxx functions *****

// 	uint16_t				id_;							//! used to identify the control in the control list, etc. 
// 	control_type			type_;							//! button vs checkbox vs radio button, etc. 
// 	int8_t					group_;							//! group number, if this control is part of a group of controls. -1 (no group) is default. Typical use case: radio buttons.
// 	Control*				next_;							//! next control in the list
// 	Window*					parent_;						//! parent window
// 	Rectangle				rect_;							//! coordinates relative to the parent window (ie, these are not global coordinates)
// 	bool					visible_;
// 	bool					active_;						//! is the control activated or not (in appearance). Does not affect ability to receive events.
// 	bool					enabled_;						//! is the control enabled or not. If not enabled, it will not receive events.
// 	int16_t					value_;							//! current value of the control
// 	int16_t					min_;							//! minimum allowed value
// 	int16_t					max_;							//! maximum allowed value
// 	Bitmap*					image_inactive_;				//! image of the control in inactive state. size must match the length and width defined in the rect_. If not supplied, the control will be effectively invisible.
// 	Bitmap*					image_active_up_;				//! image of the control when active, and not clicked/pressed. size must match the length and width defined in the rect_. If not supplied, the control will be effectively invisible.
// 	Bitmap*					image_active_down_;				//! image of the control when active, and clicked/depressed. size must match the length and width defined in the rect_. If not supplied, the control will be effectively invisible.
// 	char*					caption_;						//! optional string to draw centered horizontally and vertically on the control. Typical use cases include buttons and labels.


//! Get the ID of the control
//! @param	the_control -- a valid Control object
//! @return	Returns the ID of the passed control, or -1 in any error condition
int16_t Control_GetID(Control* the_control);

//! Get the control type
//! @param	the_control -- a valid Control object
//! @return	Returns the control_type value of the passed control, or CONTROL_TYPE_ERROR (-1) on any error
control_type Control_GetType(Control* the_control);

//! Get the pressed/not pressed state
//! @param	the_control -- a valid Control object
//! @return	Returns true if control is pressed (down), false if up
bool Control_GetPressed(Control* the_control);

int8_t Control_GetGroup(Control* the_control);

//! Get the next control in the chain
//! @param	the_control -- a valid Control object
//! @return	Returns the control object linked as the next control of the passed control. Returns NULL if there is no next control, or if the next control is itself NULL, or on any error. The last control in a chain will return NULL.
Control* Control_GetNextControl(Control* the_control);

Window* Control_GetParent(Control* the_control);
Rectangle Control_GetRect(Control* the_control);
bool Control_GetVisible(Control* the_control);
bool Control_GetActive(Control* the_control);
bool Control_GetEnabled(Control* the_control);
int16_t Control_GetValue(Control* the_control);
int16_t Control_GetMinValue(Control* the_control);
int16_t Control_GetMaxValue(Control* the_control);
Bitmap* Control_GetImageUp(Control* the_control);
Bitmap* Control_GetImageDown(Control* the_control);
Bitmap* Control_GetImageInactive(Control* the_control);
char* Control_GetCaption(Control* the_control);

bool Control_SetID(Control* the_control, uint16_t the_new_id);
bool Control_SetType(Control* the_control, control_type the_type);
bool Control_SetGroup(Control* the_control, int16_t the_group_id);

//! Links the control to the next control passed
//! @param	the_control -- a valid Control object
//! @param	the_next_control -- a valid Control object to be set as the next control from the_control. Can be NULL.
//! @return	Returns false on any error
bool Control_SetNextControl(Control* the_control, Control* the_next_control);

bool Control_SetParent(Control* the_control, Window* the_window);
bool Control_SetRect(Control* the_control, Rectangle the_rect);

//! Set the control's active/inactive state
//! @param	the_control -- a valid Control object
//! @param	is_active -- set to true to set control to active, false to set to inactive
void Control_SetActive(Control* the_control, bool is_active);

//! Set the control's pressed/unpressed state
//! @param	the_control -- a valid Control object
//! @param	is_pressed -- set to true to set control state to pressed, false to set to not-pressed
void Control_SetPressed(Control* the_control, bool is_pressed);

//! Mark the specified control is invalidated or validated
//! Note: Marking a control as invalidated causes it to be added to the parent window's list of rects to be reblitted to the screen in next render pass
//! @param	the_control -- a valid Control object
//! @param	invalidated -- set to true to set control state to invalidated (will need redraw), false to set to not-invalidated
void Control_MarkInvalidated(Control* the_control, bool invalidated);

bool Control_SetEnabled(Control* the_control, bool is_enabled);
bool Control_SetValue(Control* the_control, int16_t the_value);
bool Control_SetMinValue(Control* the_control, int16_t the_value);
bool Control_SetMaxValue(Control* the_control, int16_t the_value);
bool Control_SetImageUp(Control* the_control, Bitmap* the_image);
bool Control_SetImageDown(Control* the_control, Bitmap* the_image);
bool Control_SetImageInactive(Control* the_control, Bitmap* the_image);
bool Control_SetCaption(Control* the_control, char* the_text);

//! Set or uppdate the control's position and/or size as appropriate to the control's parent rect
//! Call when parent window size has changed, when control is first created, etc.
//! @param	the_control -- a valid Control object
void Control_AlignToParentRect(Control* the_control);



//! Compare the control's right-edge coordinate to the passed value
//! If the control is more to the right than the passed value, the passed value is updated with the control's right edge
//! @param	the_control -- a valid Control object
//! @param	x -- a pointer to a bitmap horizontal position. May be changed by the function.
//! @return	Returns true if the control is further to the right than the passed value.
bool Control_IsRighter(Control* the_control, int16_t* x);

//! Compare the control's left-edge coordinate to the passed value
//! If the control is more to the left than the passed value, the passed value is updated with the control's left edge
//! @param	the_control -- a valid Control object
//! @param	x -- a pointer to a bitmap horizontal position. May be changed by the function.
//! @return	Returns true if the control is further to the left than the passed value.
bool Control_IsLefter(Control* the_control, int16_t* x);


// **** Get xxx functions *****





// **** xxx functions *****

//! Updates the passed control with new theme info from the passed control template
//! Call this when the theme has been changed
//! It allows existing controls to be updated in place, without having to free them and create new theme controls
//! @param	the_control -- a valid Control object
//! @param	the_template -- a valid ControlTemplate object to update the Control object from.
//! @return	Returns false on any error
bool Control_UpdateFromTemplate(Control* the_control, ControlTemplate* the_template);



// **** Render functions *****

//! Blits the control to the control's parent window if the control is visible, and if it is listed as invalidated
//! Controls that are visible but not invalidated do not need redrawing.
//! Also draws the caption of the control, if it has one.
//! Marks control as no longer invalidated.
//! @param	the_control -- a valid Control object
void Control_Render(Control* the_control);




#endif /* CONTROL_H_ */


