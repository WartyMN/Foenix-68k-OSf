//! @file event.h

/*
 * event.h
 *
*  Created on: Apr 16, 2022
 *      Author: micahbly
 */

#ifndef EVENT_H_
#define EVENT_H_


/* about this class: Event Manager
 *
 * Provides structures and functions for queueing events

 * NOTE: Event structures and style are largely based on Apple's old (pre-OS X) events.h
 *   I have adapted for Foenix realities, and for my style, and added a couple of conveniences
 *     A couple of conveniences added in style of Amiga Intuition: the window and control are available directly from the event record
 *   There is no expectation that somebody's old mac code would work
 *   however, it should be familiar in feel to anyone who programmed macs before OS X

 *
 *** things this class needs to be able to do
 * Provide interrupt handlers that turn mouse and keyboard actions into events
 * Provide a global event queue that apps can access
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

#define EVENT_QUEUE_SIZE	128		//! number of event records in the circular buffer


/*****************************************************************************/
/*                               Enumerations                                */
/*****************************************************************************/

typedef enum event_kind 
{
	nullEvent				= 0,
	mouseDown				,	// left mouse button
	mouseUp					,	// left mouse button
	rMouseDown				,
	rMouseUp				,
	mMouseDown				,	// middle mouse button
	mMouseUp				,
	mouseMoved				,
	keyDown					,	// key event
	keyUp					,	// key event
	autoKey					,	// key event
	windowChanged			,	// window event
	updateEvt				,	// window event
	activateEvt				,	// window event
	inactivateEvt			,	// window event
	controlClicked			,	// window event
	menuOpened				,	// menu event
	menuSelected			,	// menu event
	menuCanceled			,	// menu event
	diskEvt					,	// DOS event
} event_kind;


typedef enum event_mask 
{
	mouseDownMask			= 1 << mouseDown,		// mouse button pressed
	mouseUpMask				= 1 << mouseUp,			// mouse button released
	keyDownMask				= 1 << keyDown,			// key pressed
	keyUpMask				= 1 << keyUp,			// key released
	autoKeyMask				= 1 << autoKey,			// key repeatedly held down
	updateMask				= 1 << updateEvt,		// window needs updating
	diskEvtMask				= 1 << diskEvt,			// disk inserted
	activateEvtMask			= 1 << activateEvt,		// activate window
	inactivateEvtMask		= 1 << inactivateEvt,	// deactivate window
	rMouseDownMask			= 1 << rMouseDown,		// right mouse button pressed
	rMouseUpMask			= 1 << rMouseUp,		// right mouse button released
	menuOpenedMask			= 1 << menuOpened,		// contextual menu opened
	menuSelectedMask		= 1 << menuSelected,	// item from the contextual menu selected
	menuCanceledMask		= 1 << menuCanceled,	// contextual menu closed without an item being selected
	controlClickedMask		= 1 << controlClicked,	// a clickable (2 state) control has been clicked
	mouseMovedMask			= 1 << mouseMoved,		// mouse has been moved
	windowChangedMask		= 1 << windowChanged,	// a window has changed size and/or position
	mMouseDownMask			= 1 << mMouseDown,		// middle mouse button pressed
	mMouseUpMask			= 1 << mMouseUp,		// middle mouse button released
	everyEvent				= 0xFFFF				// all of the above
} event_mask;


typedef enum event_modifiers
{
	noneFlagBit				= 0,    // no modifier
	activeFlagBit			= 1,    // activate? (activateEvt and mouseDown)
	btnStateBit				= 7,    // state of button?
	foenixKeyBit			= 8,    // foenix key down?
	shiftKeyBit				= 9,    // shift key down?
	alphaLockBit			= 10,   // alpha lock down?
	optionKeyBit			= 11,   // option key down?
	controlKeyBit			= 12,   // control key down?
	rightShiftKeyBit		= 13,   // right shift key down?
	rightOptionKeyBit		= 14,   // right Option key down?
	rightControlKeyBit		= 15    // right Control key down?
} event_modifiers;

typedef enum event_modifier_flags
{
	activeFlag				= 1 << activeFlagBit,
	btnState				= 1 << btnStateBit,
	foenixKey				= 1 << foenixKeyBit,
	shiftKey				= 1 << shiftKeyBit,
	alphaLock				= 1 << alphaLockBit,
	optionKey				= 1 << optionKeyBit,
	controlKey				= 1 << controlKeyBit,
	rightShiftKey			= 1 << rightShiftKeyBit,
	rightOptionKey			= 1 << rightOptionKeyBit,
	rightControlKey			= 1 << rightControlKeyBit
} event_modifier_flags;


// TODO: localize this for A2560
enum
{
	kNullCharCode                 = 0,
	kHomeCharCode                 = 1,
	kEnterCharCode                = 3,
	kEndCharCode                  = 4,
	kHelpCharCode                 = 5,
	kBellCharCode                 = 7,
	kBackspaceCharCode            = 8,
	kTabCharCode                  = 9,
	kLineFeedCharCode             = 10,
	kVerticalTabCharCode          = 11,
	kPageUpCharCode               = 11,
	kFormFeedCharCode             = 12,
	kPageDownCharCode             = 12,
	kReturnCharCode               = 13,
	kFunctionKeyCharCode          = 16,
	kEscapeCharCode               = 27,
	kClearCharCode                = 27,
	kLeftArrowCharCode            = 28,
	kRightArrowCharCode           = 29,
	kUpArrowCharCode              = 30,
	kDownArrowCharCode            = 31,
	kDeleteCharCode               = 127,
	kNonBreakingSpaceCharCode     = 202
};



/*****************************************************************************/
/*                                 Structs                                   */
/*****************************************************************************/

struct EventKeyboard {
//	event_modifiers		modifiers_;	//! set for keyboard and mouse events
	uint8_t				key_;		//! the key code of the key pushed. eg, KEY_BKSP (0x92), not CH_BKSP (0x08). Most useful for handling action keys such as cursors, DEL, BS, ESC, etc.
	uint8_t				char_;		//! the character code resulting from the key, after mapping. e.g, 1 may return 49, ALT-1 may return 145, SHIFT-1 may return 33; backspace may return 8, etc.
	uint8_t				modifiers_;	//! bit flags for shift, ctrl, meta, etc. 
	uint8_t				source_;	//! 0=internal keyboard, 1=external ps/2 keyboard
};

struct EventMenu {
	int16_t				selection_;	//! for menu events: the selected menu item ID
	int16_t				x_;			//! for menu events: the global x position of mouse.
	int16_t				y_;			//! for menu events: the global y position of mouse.
};

struct EventMouse {
	event_modifiers		modifiers_;	//! set for keyboard and mouse events
	int16_t				x_;			//! for mouse events: the global x position of mouse.
	int16_t				y_;			//! for mouse events: the global y position of mouse.
	Control*			control_;	//! for mouse events: the effected control
};

struct EventWindow {
	event_modifiers		modifiers_;	//! set for keyboard and mouse events
	int16_t				x_;			//! for window events: the new global x position of the window.
	int16_t				y_;			//! for window events: the new global y position of the window.
	int16_t				width_;		//! for window events: the new width of the window.
	int16_t				height_;	//! for window events: the new height of the window.
};

struct EventRecord
{
	uint32_t			when_;		//! ticks
	event_kind			what_;
	Window*				window_;	//! The affected window (if any). May be NULL.
	Control*			control_;	//! The affected control (if any). May be NULL.
	union {
		EventKeyboard	keyinfo_;
		EventMouse		mouseinfo_;
		EventWindow		windowinfo_;
		EventMenu		menuinfo_;
	};
};

struct EventManager
{
	EventRecord*		queue_[EVENT_QUEUE_SIZE];	//! circular buffer for the event queue
	uint16_t			write_idx_;					//! index to queue_: where the next event record will be slotted
	uint16_t			read_idx_;					//! index to queue_: where the next event record will be read from
	MouseTracker*		mouse_tracker_;				//! tracks whether mouse is in drag mode, etc.
};




/*****************************************************************************/
/*                             Global Variables                              */
/*****************************************************************************/


/*****************************************************************************/
/*                       Public Function Prototypes                          */
/*****************************************************************************/


// **** events are pre-created in a fixed size array on system startup (circular buffer)
// **** as interrupts need to add more events, they take the next slot available in the array

// **** CONSTRUCTOR AND DESTRUCTOR *****

// constructor
//! Allocate an EventManager object
EventRecord* Event_New(void);

// destructor
// frees all allocated memory associated with the passed object, and the object itself
bool Event_Destroy(EventRecord** the_event);



// constructor
//! Allocate an EventManager object
EventManager* EventManager_New(void);

// destructor
// frees all allocated memory associated with the passed object, and the object itself
bool EventManager_Destroy(EventManager** the_event_manager);




// **** Queue Management functions *****


//! Nulls out any events associated with the window pointer passed
//! Call this when a window has been closed, to ensure that there are not future events that will try to recall the window after it is destroyed
void EventManager_RemoveEventsForWindow(Window* the_window);

//! Checks to see if there is an event in the queue
//! returns NULL if no event (not the same as returning an event of type nullEvent)
EventRecord* EventManager_NextEvent(void);

//! Add a new mouse event to the event queue
//! NOTE: this does not actually insert a new record, as the event queue is a circular buffer
//!   It overwrites whatever slot is next in line
//! This is designed to be called from mouse IRQ, with minimimal information available
//! @param	the_what -- specifies the type of event to add to the queue. only mouseDown/up/moved events supported
//! The function will query VICKY for x,y and determine if the mouse is over a window, a control, etc. 
void EventManager_AddMouseEvent(event_kind the_what);

//! Add a new window event to the event queue
//! NOTE: this does not actually insert a new record, as the event queue is a circular buffer
//!   It overwrites whatever slot is next in line
//! @param	the_what -- specifies the type of event to add to the queue. only window events such as windowChanged are supported
void EventManager_AddWindowEvent(event_kind the_what, int16_t x, int16_t y, int16_t width, int16_t height, Window* the_window, Control* the_control);

//! Add a new menu event to the event queue
//! NOTE: this does not actually insert a new record, as the event queue is a circular buffer
//!   It overwrites whatever slot is next in line
//! @param	the_what -- specifies the type of event to add to the queue. only menu events such as windowChanged are supported
void EventManager_AddMenuEvent(event_kind the_what, int16_t menu_selection,int16_t x, int16_t y, Window* the_window);

//! Wait for an event to happen, do system-processing of it, then if appropriate, give the window responsible for the event a chance to do something with it
void EventManager_WaitForEvent(void);





// **** Debug functions *****

void Event_Print(EventRecord* the_event);
void EventManager_Print(EventManager* the_event_manager);



#endif /* EVENT_H_ */


