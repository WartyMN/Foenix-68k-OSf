//! @file general.h

/*
 * general.h
 *
 *  Created on: Feb 19, 2022
 *      Author: micahbly
 */

// This is a cut-down, semi-API-compatible version of the OS/f general.c file from Lich King (Foenix)
// adapted for Foenix F256 Jr starting November 29, 2022
// Adapted for F256jr/k E fpga builds from F256jr/k version of FileManager starting June 2, 2024
// Debug-related functionality extracted to debug.h/.c July 2024
// adapted (back-ported) for A2560K starting Dec 26, 2024


#ifndef GENERAL_H_
#define GENERAL_H_


/* about this class
 *
 *
 *
 *** things this class needs to be able to do
 * various utility functions that any app could find useful.
 * intended to be re-usable across apps, with at most minimal differences.
 * this file should contain only cross-platform code. platform-specific code should go into general_[platformname].h/.c
 *
 *** things objects of this class have
 *
 *
 */


/*****************************************************************************/
/*                                Includes                                   */
/*****************************************************************************/

// project includes
//#include "text.h"

// C includes
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

// A2560 includes
#include "a2560k.h"


/*****************************************************************************/
/*                            Macro Definitions                              */
/*****************************************************************************/



/*****************************************************************************/
/*                               Enumerations                                */
/*****************************************************************************/



/*****************************************************************************/
/*                                 Structs                                   */
/*****************************************************************************/

typedef struct Coordinate
{
    uint8_t x;
    uint8_t y;
} Coordinate;

/*****************************************************************************/
/*                             Global Variables                              */
/*****************************************************************************/


/*****************************************************************************/
/*                       Public Function Prototypes                          */
/*****************************************************************************/



// **** WORD-WRAP UTILITIES *****





// **** MATH UTILITIES *****

//! Round a float to the nearest integer value
//! THINK C's and SAS/C's math.h don't include round()
//! from: https://stackoverflow.com/questions/4572556/concise-way-to-implement-round-in-c
//! @param	the_float - a double value to round up/down
//! @return	Returns an int with the rounded value
int32_t General_Round(double the_float);




// **** NUMBER<>STRING UTILITIES *****

// convert a file size in bytes to a human readable format using "10 bytes", "1.4 kb", "1 MB", etc. 
//   NOTE: formatted_file_size string must have been allocated before passing here
void General_MakeFileSizeReadable(unsigned long size_in_bytes, char* formatted_file_size);






// **** MISC STRING UTILITIES *****

//! Convert a string, in place, to lower case
//! This overwrites the string with a lower case version of itself.
//! Warning: no length check is in place. Calling function must verify string is well-formed (terminated).
//! @param	the_string - the string to convert to lower case.
//! @return	Returns true if the string was modified by the process.
bool General_StrToLower(char* the_string);

//! Change the case of the passed character from upper to lower (if necessary)
//! Scope is limited to characters A-Z, ascii.
//! replacement for tolower() in c library, which doesn't seem to work [in Amiga WB2K] for some reason.
//! @return	a character containing the lowercase version of the passed character.
uint8_t General_ToLower(uint8_t the_char);

//! Allocates memory for a new string and copies up to max_len - 1 characters from the NUL-terminated string src to the new string, NUL-terminating the result
//! This is meant to be a one stop shop for getting a copy of a string
//! @param	src - The string to copy
//! @param	max_len - The maximum number of bytes to use in the destination string, including the terminator. If this is shorter than the length of the source string + 1, the resulting copy string will be capped at max_len - 1.
//! @return	a copy of the source string to max_len, or NULL on any error condition
char* General_StrlcpyWithAlloc(const char* src, size_t max_len);

//! Copies up to max_len - 1 characters from the NUL-terminated string src to dst, NUL-terminating the result
//! @param	src - The string to copy
//! @param	dst - The string to copy into. Calling function is responsible for ensuring this string is allocated, and has at least as much storage as max_len.
//! @param	max_len - The maximum number of bytes to use in the destination string, including the terminator. If this is shorter than the length of the source string + 1, the resulting copy string will be capped at max_len - 1.
//! @return	Returns the length of the source string, or -1 on any error condition
int16_t General_Strlcpy(char* dst, const char* src, size_t max_len);

//! Copies up to max_len - 1 characters from the NUL-terminated string src and appends to the end of dst, NUL-terminating the result
//! @param	src - The string to copy
//! @param	dst - The string to append to. Calling function is responsible for ensuring this string is allocated, and has at least as much storage as max_len.
//! @param	max_len - The maximum number of bytes to use in the destination string, including the terminator. If this is shorter than the length of src + length of dst + 1, the resulting copy string will be capped at max_len - 1.
//! @return	Returns the length of the attempted concatenated string: initial length of dst plus the length of src.
int16_t General_Strlcat(char* dst, const char* src, size_t max_len);

//! Makes a case sensitive comparison of the specified number of characters of the two passed strings
//! Stops processing once max_len has been reached, or when one of the two strings has run out of characters.
//! http://home.snafu.de/kdschem/c.dir/strings.dir/strncmp.c
//! TODO: compare this to other implementations, see which is faster. eg, https://opensource.apple.com/source/Libc/Libc-167/gen.subproj/i386.subproj/strncmp.c.auto.html
//! @param	string_1 - the first string to compare.
//! @param	string_2 - the second string to compare.
//! @param	max_len - the maximum number of characters to compare. Even if both strings are larger than this number, only this many characters will be compared.
//! @return	Returns 0 if the strings are equivalent (at least up to max_len). Returns a negative or positive if the strings are different.
int16_t General_Strncmp(const char* string_1, const char* string_2, size_t max_len);

//! Makes a case insensitive comparison of the specified number of characters of the two passed strings
//! Stops processing once max_len has been reached, or when one of the two strings has run out of characters.
//! Inspired by code from slashdot and apple open source
//! https://stackoverflow.com/questions/5820810/case-insensitive-string-comparison-in-c
//! https://opensource.apple.com/source/tcl/tcl-10/tcl/compat/strncasecmp.c.auto.html
//! @param	string_1 - the first string to compare.
//! @param	string_2 - the second string to compare.
//! @param	max_len - the maximum number of characters to compare. Even if both strings are larger than this number, only this many characters will be compared.
//! @return	Returns 0 if the strings are equivalent (at least up to max_len). Returns a negative or positive if the strings are different.
int16_t General_Strncasecmp(const char* string_1, const char* string_2, size_t max_len);

//! Measure the length of a fixed-size string
//! Safe(r) strlen function: will stop processing if no terminator found before max_len reached
// Inspired by apple/bsd strnlen.
//! @return	Returns strlen(the_string), if that is less than max_len, or max_len if there is no null terminating ('\0') among the first max_len characters pointed to by the_string.
int16_t General_Strnlen(const char *the_string, size_t max_len);




// **** RECTANGLE UTILITIES *****


//! Test if one rectangle is entirely within the bounds of another Rectangle
//! @param	r1: the rectangle being tested
//! @param	r2: the rectangle being measured to determine if r1 fits entirely within it
//! @return:	returns true if r1 is within bounds of r2. 
bool General_RectWithinRect(Rectangle r1, Rectangle r2);

// test if 2 rectangles intersect
bool General_RectIntersect(struct Rectangle r1, struct Rectangle r2);

// test if a point is within a rectangle
bool General_PointInRect(int16_t x, int16_t y, Rectangle r);

// Position one rect within the bounds of another. Horizontally: centers the hero rect within the left/right of the frame rect; Vertically: centers or or puts at 25% line
// put the frame coords into the frame_rect, and the object to be centered into the hero_rect. ON return, the frame rect will hold the coords to be used.
void General_CenterRectWithinRect(Rectangle* the_frame_rect, Rectangle* the_hero_rect, bool at_25_percent_v);

//! Copy values of one rect to another
//! @param	r1: the rectangle to be overwritten (copied into)
//! @param	r2: the rectangle to copy
void General_CopyRect(Rectangle* r1, Rectangle* r2);

//! Calculate the difference between 2 rectangles and populate 0, 1, 2, 3, or 4 new rectangles with the difference
//! If Rect 1 is larger than Rect 2, no new rect will be populated
//! If Rect 1 is smaller than Rect 2 in one dimension (axis) only, 1 new rect will be populated
//! If Rect 1 is smaller than Rect 2 in two dimensions (axes), 3 new rect will be populated
//! If Rect 1 is same size as Rect 2 and moved in one dimension (axis) only, 1 new rect will be populated
//! If Rect 1 is same size as Rect 2 and moved in two dimensions (axes), 3 new rect will be populated
//! @param	r1: the lead, or foreground rect. When calculating a damage rect, this would typically be the rect of the window after it is moved/resized.
//! @param	r2: the secondary, or background rect. When calculating a damage rect, this would typically be the rect of the window before it is moved/resized.
//! @param	diff_r1: valid pointer to a rect object that will be populated if there is 1 or 3 difference rects resulting from the operation
//! @param	diff_r2: valid pointer to a rect object that will be populated if there are 2 or more difference rects resulting from the operation
//! @param	diff_r3: valid pointer to a rect object that will be populated if there are 3 or more difference rects resulting from the operation
//! @param	diff_r4: valid pointer to a rect object that will be populated if there are 4 difference rects resulting from the operation
//! @return	Returns number of new rects that represent the difference between the passed rectangles. This indicates how many, if any, of the diff_rects need to be evaluated. Returns -1 on any error condition.
int16_t General_CalculateRectDifference(Rectangle* r1, Rectangle* r2, Rectangle* diff_r1, Rectangle* diff_r2, Rectangle* diff_r3, Rectangle* diff_r4);

//! Calculate the intersection between 2 rectangles, storing result in the 3rd rect passed
//! @param	r1: valid pointer to a rect object
//! @param	r2: valid pointer to a rect object
//! @param	intersect_r: valid pointer to a rect object that will contain the intersection rectangle, if any, at end of operation
//! @return:	Returns true if there is an intersecting rectangle between r1 and r2.
bool General_CalculateRectIntersection(Rectangle* r1, Rectangle* r2, Rectangle* intersect_r);





// **** FILENAME AND FILEPATH UTILITIES *****



// // allocate and return  the portion of the path passed, minus the filename. In other words: return a path to the parent file.
// // calling method must free the string returned
// char* General_ExtractPathToParentFolderWithAlloc(const char* the_file_path);

// // allocate and return the filename portion of the path passed.
// // calling method must free the string returned
// char* General_ExtractFilenameFromPathWithAlloc(const char* the_file_path);

// populates the passed string by safely combining the passed file path and name, accounting for cases where path is a disk root
void General_CreateFilePathFromFolderAndFile(char* the_combined_path, char* the_folder_path, char* the_file_name);

// return the first char of the last part of a file path
// if no path part detected, returns the original string
// not guaranteed that this is a FILENAME, as if you passed a path to a dir, it would return the DIR name
// amigaDOS compatibility function (see FilePart)
char* General_NamePart(const char* the_file_path);

// return everything to the left of the filename in a path. 
char* General_PathPart(const char* the_file_path);

//! Extract file extension into the passed char pointer, as new lowercased string pointer, if any found.
//! @param	the_file_name - the file name to extract an extension from
//! @param	the_extension - a pre-allocated buffer that will contain the extension, if any is detected. Must be large enough to hold the extension! No bounds checking is done. 
//! @return	Returns false if no file extension found.
bool General_ExtractFileExtensionFromFilename(const char* the_file_name, char* the_extension);

//! Extract core part of the file name, not including the extension
//! @param	the_file_name - the file name to extract an extension from
//! @param	the_core_part - a pre-allocated buffer that will contain the pre-extension part of the file name, if any is detected. Must be large enough to hold the entire file name! No bounds checking is done. If no extension is found, this will contain an empty string.
//! @return	Returns false if no file extension found (because what is "core" part without an extension)
bool General_ExtractCoreFilename(const char* the_file_name, char* the_core_part);




// **** TIME UTILITIES *****


//! Wait for the specified number of ticks before returning
//! In multi-tasking ever becomes a thing, this is not a multi-tasking-friendly operation. 
void General_DelayTicks(int32_t ticks);

//! Wait for the specified number of seconds before returning
//! In multi-tasking ever becomes a thing, this is not a multi-tasking-friendly operation. 
void General_DelaySeconds(uint16_t seconds);



// **** USER INPUT UTILITIES *****

// // Wait for one character from the keyboard and return it
// char General_GetChar(void);





// **** MISC UTILITIES *****


// get random number between 1 and the_range + 1
// must have seeded the number generator first
// if passed 0, returns 0.
uint16_t General_GetRandom(uint16_t the_range);



#endif /* GENERAL_H_ */
