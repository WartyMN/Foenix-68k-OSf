# Text Class
Provides basic functions for the A2560's text mode

## Status
95% complete. Next phase: catch up with F256K and K2 f/manager text library: add concept of continuously increasing cursor position and end of line wrap to start of next.  

## Why you might want this:
If you have, or are considering acquiring an A2560K or GenX (68040 + 65816), and want a simple way to put text on the screen in your C programs.  

## Expected final functionality
 * work with either channel A or channel B
 * clear / fill an entire screen of text characters
 * clear / fill an entire screen of text attributes
 * invert the colors of a screen
 * clear / fill a smaller-than-screen rectangular area of text/attrs
 * Draw a char to a specified x, y coord
 * Get the currently displayed character at the specified coord
 * Set the foreground and background colors at the specified coord
 * Set the attribute value at the specified coord
 * Get the attribute value at the specified coord
 * Get the foreground or background color at the specified coord
 * draw a line using "graphic" characters
 * draw a box using "graphic" characters
 * copy a full screen of text or attr from an off-screen buffer
 * copy a full screen of text or attr TO an off-screen buffer
 * copy a full screen of text and attr between channel A and B
 * copy a rectangular area of text or attr TO/FROM an off-screen buffer
 * display a pre-formatted string in a rectangular block on the screen, breaking on \n characters
 * Draw a string to a specified x, y coord (no wrap)
 * format a string for a given height and width, wrapping words as necessary
 * display a formatted string in a rectangular block on the screen, with wrap
 * display a string in a rectangular block on the screen, with wrap, taking a hook for a "display more" event, and scrolling text vertically up after hook func returns 'continue' (or exit, returning control to calling func, if hook returns 'stop')
 * replace current text font with another, loading from specified ram loc.
 * configure at compile time for use in various Foenix machines
 * Auto recognize the screen resolution, num text cols, size of borders, etc. 
 * ability to change screen resolution
 
## Completed
 * work with either channel A or channel B
 * clear / fill an entire screen of text characters
 * clear / fill an entire screen of text attributes
 * invert the colors of a screen
 * clear / fill a smaller-than-screen rectangular area of text/attrs
 * Draw a char to a specified x, y coord
 * Get the currently displayed character at the specified coord
 * Set the foreground and background colors at the specified coord
 * Set the attribute value at the specified coord
 * Get the attribute value at the specified coord
 * Get the foreground or background color at the specified coord
 * draw a line using "graphic" characters
 * draw a box using "graphic" characters
 * copy a full screen of text or attr from an off-screen buffer
 * copy a full screen of text or attr TO an off-screen buffer
 * copy a rectangular area of text or attr TO/FROM an off-screen buffer
 * Draw a string to a specified x, y coord (no wrap)
 * format a string for a given height and width, wrapping words as necessary
 * display a formatted string in a rectangular block on the screen, with wrap
 * display a string in a rectangular block on the screen, with wrap, taking a hook for a "display more" event, and scrolling text vertically up after hook func returns 'continue' (or exit, returning control to calling func, if hook returns 'stop')
 * replace current text font with another, loading from specified ram loc.
 * documentation (for text lib, have not started on general lib)
 * prep and clean up to be a standalone static library
 * Auto recognize the screen resolution, num text cols, size of borders, etc. 
 * ability to change screen resolution

## ToDo
 * copy a full screen of text and attr between channel A and B
 * optimization
 * document lib_general.c
 
