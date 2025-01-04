/*
 * general.c
 *
 *  Created on: Feb 19, 2022
 *      Author: micahbly
 *
 *  - adapted from my Amiga WB2K project's general.c
 */

// This is a cut-down, semi-API-compatible version of the OS/f general.c file from Lich King (Foenix)
// adapted for Foenix F256 Jr starting November 29, 2022
// adapted for Foenix F256Jr/K "extended" (flat memory map, no MMU) starting June 2024
//   debug-functions removed to separate file, July 2024
// adapted (back-ported) for A2560K starting Dec 26, 2024


/*****************************************************************************/
/*                                Includes                                   */
/*****************************************************************************/

// project includes
#include "debug.h"
#include "general.h"

// C includes
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

// A2560 includes
#include <mcp/syscalls.h>
#include "a2560k.h"


/*****************************************************************************/
/*                               Definitions                                 */
/*****************************************************************************/



/*****************************************************************************/
/*                          File-scoped Variables                            */
/*****************************************************************************/



/*****************************************************************************/
/*                             Global Variables                              */
/*****************************************************************************/


extern char*			global_string_buff1;
extern char*			global_string_buff2;





/*****************************************************************************/
/*                       Private Function Prototypes                         */
/*****************************************************************************/

//! \cond PRIVATE

// Convert a (positive-only) string integer to an unsigned long integer. returns false in event of error
bool General_StringToUnsignedLong(const char* the_string_value, uint32_t* the_conversion);

//! \endcond


/*****************************************************************************/
/*                       Private Function Definitions                        */
/*****************************************************************************/


//! \cond PRIVATE

// Convert a (positive-only) string integer to an unsigned long integer. returns false in event of error
bool General_StringToUnsignedLong(const char* the_string_value, uint32_t* the_conversion)
{
	char*			temp;
	uint32_t		val = 0;
	bool			safe_conversion = true;
	
	errno = 0;
	// LOGIC: errno will be changed by strtol if an overrun or other error occurs
	

	// do conversion... carefully

	val = strtol((char*)the_string_value, (char**)&temp, 0);

	if (temp == the_string_value || *temp != '\0' || ((val == LONG_MIN || val == LONG_MAX) && errno == ERANGE))
	{
		//printf("General_StringToUnsignedLong: Could not convert '%s' to long and leftover string is: '%s' \n", the_string_value, temp);
		val = LONG_MAX;
		safe_conversion = false;
	}

	if (val > INT_MAX)
	{
		//printf("General_StringToUnsignedLong: Could not convert '%s' to int: greater than allowable value.\n", the_string_value);
		val = LONG_MAX;
		safe_conversion = false;
	}

	*the_conversion = (uint32_t)val;
	return safe_conversion;
}


//! \endcond



/*****************************************************************************/
/*                        Public Function Definitions                        */
/*****************************************************************************/







// **** MATH UTILITIES *****

//! Round a float to the nearest integer value
//! THINK C's and SAS/C's math.h don't include round()
//! from: https://stackoverflow.com/questions/4572556/concise-way-to-implement-round-in-c
//! @param	the_float - a double value to round up/down
//! @return	Returns an int with the rounded value
int32_t General_Round(double the_float)
{
    if (the_float < 0.0)
        return (int)(the_float - 0.5);
    else
        return (int)(the_float + 0.5);
}


//! min() implementation
int32_t General_LongMin(int32_t a, int32_t b)
{
	return (a > b ? b : a);
}


//! max() implementation
int32_t General_LongMax(int32_t a, int32_t b)
{
	return (a > b ? a : b);
}


//! min() implementation
int16_t General_ShortMin(int16_t a, int16_t b)
{
	return (a > b ? b : a);
}


//! max() implementation
int16_t General_ShortMax(int16_t a, int16_t b)
{
	return (a > b ? a : b);
}







// **** NUMBER<>STRING UTILITIES *****



// convert a file size in bytes to a human readable format using "10 bytes", "1.4 kb", "1 MB", etc. 
//   NOTE: formatted_file_size string must have been allocated before passing here
void General_MakeFileSizeReadable(unsigned long size_in_bytes, char* formatted_file_size)
{
	double			final_size;
	
	// convert to float before doing any operations on it, to prevent integer weirdness
	final_size = (double)size_in_bytes;
	
	if (size_in_bytes < 1024) // 1.0k
	{
		// show size in bytes, precisely
		sprintf((char*)formatted_file_size, "%lu b", size_in_bytes);
	}
	else if (size_in_bytes < 10240) // 10k
	{
		// show size in .1k chunks (eg, 9.4k)
		final_size /= 1024;
		sprintf((char*)formatted_file_size, "%.1f kb", final_size);
	}
	else if (size_in_bytes < 1048576) // 1 MB
	{
		// show size in 1k chunks
		final_size /= 1024;
		size_in_bytes = General_Round(final_size);
		sprintf((char*)formatted_file_size, "%lu kb", size_in_bytes);
	}
	else if (size_in_bytes < 10485760) // 10MB
	{
		// show size in .1M chunks (eg, 1.4MB)
		final_size /= 1048576;
		sprintf((char*)formatted_file_size, "%.1f Mb", final_size);
	}
	else
	{
		// show size in 1M chunks (eg, 1536 MB)
		final_size /= 1048576;
		size_in_bytes = General_Round(final_size);
		sprintf((char*)formatted_file_size, "%lu Mb", size_in_bytes);
	}
	
	return;
}


// Convert a positive or negative string integer to a signed long integer. returns false in event of error
bool General_StringToSignedLong(const char* the_string_value, int32_t* the_conversion)
{
	int32_t			signed_val = 0;
	uint32_t		unsigned_val = 0;
	bool			safe_conversion;
	const char*		start_of_number = the_string_value;
	
	// is this a negative number string?
	if (*the_string_value == '-')
	{
		start_of_number++;
	}
	
	safe_conversion = General_StringToUnsignedLong(start_of_number, &unsigned_val);

	signed_val = (signed long)unsigned_val;
	
	if (*the_string_value == '-')
	{
		signed_val = -signed_val;
	}
	
	*the_conversion = signed_val;

	return safe_conversion;
}



// **** MISC STRING UTILITIES *****


//! Convert a string, in place, to lower case
//! This overwrites the string with a lower case version of itself.
//! Warning: no length check is in place. Calling function must verify string is well-formed (terminated).
//! @param	the_string - the string to convert to lower case.
//! @return	Returns true if the string was modified by the process.
bool General_StrToLower(char* the_string)
{
    int16_t	i;
    int16_t	len = General_Strnlen(the_string, MAX_STRING_COMP_LEN);
    bool	change_made = false;
    
	for (i = 0; i < len; i++)
	{
	    uint8_t	this_char;
		
		this_char = the_string[i];
		the_string[i] = General_ToLower(the_string[i]);
		
		if (this_char != the_string[i])
		{
			change_made = true;
		}
	}

	return change_made;
}


//! Change the case of the passed character from upper to lower (if necessary)
//! Scope is limited to characters A-Z, ascii.
//! replacement for tolower() in c library, which doesn't seem to work [in Amiga WB2K] for some reason.
//! @return	a character containing the lowercase version of the passed character.
uint8_t General_ToLower(uint8_t the_char)
{
    uint8_t	lowered_value;
    
    lowered_value = (the_char >='A' && the_char<='Z') ? (the_char + 32) : (the_char);
    
    return lowered_value;
}


//! Allocates memory for a new string and copies up to max_len - 1 characters from the NUL-terminated string src to the new string, NUL-terminating the result
//! This is meant to be a one stop shop for getting a copy of a string
//! @param	src - The string to copy
//! @param	max_len - The maximum number of bytes to use in the destination string, including the terminator. If this is shorter than the length of the source string + 1, the resulting copy string will be capped at max_len - 1.
//! @return	a copy of the source string to max_len, or NULL on any error condition
char* General_StrlcpyWithAlloc(const char* src, size_t max_len)
{
	char*	dst;
	size_t	alloc_len;
	
	if (max_len < 1)
	{
		return NULL;
	}
	
	alloc_len = General_Strnlen(src, max_len) + 1;
	
	if ( (dst = (char*)calloc(alloc_len, sizeof(char)) ) == NULL)
	{
		return NULL;
	}
	else
	{
		General_Strlcpy(dst, src, max_len);
	}
	LOG_ALLOC(("%s %d:	__ALLOC__	dst	%p	size	%i, string='%s'", __func__ , __LINE__, dst, alloc_len, dst));
	TRACK_ALLOC((alloc_len));

	return dst;
}


//! Copies up to max_len - 1 characters from the NUL-terminated string src to dst, NUL-terminating the result
//! @param	src - The string to copy
//! @param	dst - The string to copy into. Calling function is responsible for ensuring this string is allocated, and has at least as much storage as max_len.
//! @param	max_len - The maximum number of bytes to use in the destination string, including the terminator. If this is shorter than the length of the source string + 1, the resulting copy string will be capped at max_len - 1.
//! @return	Returns the length of the source string, or -1 on any error condition
int16_t General_Strlcpy(char* dst, const char* src, size_t max_len)
{
    const int16_t	src_len = strlen(src);
 	
	if (max_len < 1)
	{
		return -1;
	}

    if (src_len + 1 < max_len)
    {
        memcpy(dst, src, src_len + 1);
    }
    else
    {
    	memcpy(dst, src, max_len - 1);
        dst[max_len - 1] = '\0';
    }
    
    return src_len;
}


//! Copies up to max_len - 1 characters from the NUL-terminated string src and appends to the end of dst, NUL-terminating the result
//! @param	src - The string to copy
//! @param	dst - The string to append to. Calling function is responsible for ensuring this string is allocated, and has at least as much storage as max_len.
//! @param	max_len - The maximum number of bytes to use in the destination string, including the terminator. If this is shorter than the length of src + length of dst + 1, the resulting copy string will be capped at max_len - 1.
//! @return	Returns the length of the attempted concatenated string: initial length of dst plus the length of src.
int16_t General_Strlcat(char* dst, const char* src, size_t max_len)
{  	
	const int16_t	src_len = strlen(src);
	const int16_t 	dst_len = General_Strnlen(dst, max_len);
 	
	if (max_len > 0)
	{
		if (dst_len == max_len)
		{
			return max_len + src_len;
		}

		if (src_len < max_len - dst_len)
		{
			memcpy(dst + dst_len, src, src_len + 1);
		}
		else
		{
			memcpy(dst + dst_len, src, max_len - dst_len - 1);
			dst[max_len - 1] = '\0';
		}
	}

    return dst_len + src_len;
}


//! Makes a case sensitive comparison of the specified number of characters of the two passed strings
//! Stops processing once max_len has been reached, or when one of the two strings has run out of characters.
//! http://home.snafu.de/kdschem/c.dir/strings.dir/strncmp.c
//! TODO: compare this to other implementations, see which is faster. eg, https://opensource.apple.com/source/Libc/Libc-167/gen.subproj/i386.subproj/strncmp.c.auto.html
//! @param	string_1 - the first string to compare.
//! @param	string_2 - the second string to compare.
//! @param	max_len - the maximum number of characters to compare. Even if both strings are larger than this number, only this many characters will be compared.
//! @return	Returns 0 if the strings are equivalent (at least up to max_len). Returns a negative or positive if the strings are different.
int16_t General_Strncmp(const char* string_1, const char* string_2, size_t max_len)
{
	register uint8_t	u;
	
	do ; while( (u = (uint8_t)*string_1++) && (u == (uint8_t)*string_2++) && --max_len );

	if (u)
	{
		string_2--;
	}
	
	return (u - (uint8_t)*string_2);
}


//! Makes a case insensitive comparison of the specified number of characters of the two passed strings
//! Stops processing once max_len has been reached, or when one of the two strings has run out of characters.
//! Inspired by code from slashdot and apple open source
//! https://stackoverflow.com/questions/5820810/case-insensitive-string-comparison-in-c
//! https://opensource.apple.com/source/tcl/tcl-10/tcl/compat/strncasecmp.c.auto.html
//! @param	string_1 - the first string to compare.
//! @param	string_2 - the second string to compare.
//! @param	max_len - the maximum number of characters to compare. Even if both strings are larger than this number, only this many characters will be compared.
//! @return	Returns 0 if the strings are equivalent (at least up to max_len). Returns a negative or positive if the strings are different.
int16_t General_Strncasecmp(const char* string_1, const char* string_2, size_t max_len)
{
	uint8_t	u1;
	uint8_t	u2;
	//DEBUG_OUT(("%s %d: s1='%s'; s2='%s'; max_len=%i", __func__ , __LINE__, string_1, string_2, max_len));

	for (; max_len != 0; max_len--, string_1++, string_2++)
	{
		u1 = (uint8_t)*string_1;
		u2 = (uint8_t)*string_2;
			
		if (General_ToLower(u1) != General_ToLower(u2))
		{
			return General_ToLower(u1) - General_ToLower(u2);
		}
		
		if (u1 == '\0')
		{
			return 0;
		}
	}
	
	return 0;	
}


//! Measure the length of a fixed-size string
//! Safe(r) strlen function: will stop processing if no terminator found before max_len reached
// Inspired by apple/bsd strnlen.
//! @return	Returns strlen(the_string), if that is less than max_len, or max_len if there is no null terminating ('\0') among the first max_len characters pointed to by the_string.
int16_t General_Strnlen(const char* the_string, size_t max_len)
{
	int16_t	len;
 	
	for (len = 0; len < max_len; len++, the_string++)
	{
		if (!*the_string)
		{
			break;
		}
	}

	return (len);
}


//! Compare the length of two strings, returning true if the first is longer than the second.
//! This function accepts void* instead of char*, to be compatible with List_MergeSortedList().
//! NOTE: compares to a maximum of MAX_STRING_COMP_LEN
//! @param	first_payload - the first string to compare, passed as a void pointer.
//! @param	second_payload - the second string to compare, passed as a void pointer.
//! @return	Returns true if the first string is longer than the second. Returns false if the strings are equivalent in length, or if second is longer. 
bool General_CompareStringLength(void* first_payload, void* second_payload)
{
	char*		string_1 = (char*)first_payload;
	char*		string_2 = (char*)second_payload;

	if (General_Strnlen(string_1, MAX_STRING_COMP_LEN) > General_Strnlen(string_2, MAX_STRING_COMP_LEN))
	{
		return true;
	}
	else
	{
		return false;
	}
}


// **** RECTANGLE UTILITIES *****










//! Test if one rectangle is entirely within the bounds of another Rectangle
//! @param	r1 - the rectangle being tested
//! @param	r2 - the rectangle being measured to determine if r1 fits entirely within it
//! @return:	returns true if r1 is within bounds of r2. 
bool General_RectWithinRect(Rectangle r1, Rectangle r2)
{
	if	(
		(r1.MinX >= r2.MinX) &&
		(r1.MaxX <= r2.MaxX) &&
		(r1.MinY >= r2.MinY) &&
		(r1.MaxY <= r2.MaxY)
		)
	{
		DEBUG_OUT(("%s %d: r1 (%i, %i : %i, %i) IS entirely within r2 (%i, %i : %i, %i)", __func__, __LINE__, r1.MinX, r1.MinY, r1.MaxX, r1.MaxY, r2.MinX, r2.MinY, r2.MaxX, r2.MaxY));
		return true;
	}

	DEBUG_OUT(("%s %d: r1 (%i, %i : %i, %i) is NOT entirely within r2 (%i, %i : %i, %i)", __func__, __LINE__, r1.MinX, r1.MinY, r1.MaxX, r1.MaxY, r2.MinX, r2.MinY, r2.MaxX, r2.MaxY));

	return false;
}


// test if 2 rectangles intersect
bool General_RectIntersect(Rectangle r1, Rectangle r2)
{
	if	(
		(r1.MinX > r2.MaxX) ||
		(r1.MaxX < r2.MinX) ||
		(r1.MinY > r2.MaxY) ||
		(r1.MaxY < r2.MinY)
		)
	{
		return false;
	}

	return true;
}


// test if a point is within a rectangle
bool General_PointInRect(int16_t x, int16_t y, Rectangle r)
{
	//DEBUG_OUT(("%s %d: x=%i, y=%i, r.MinX=%i, r.MinY=%i, r.MaxX=%i, r.MaxY=%i", __func__, __LINE__, x, y,  r.MinX, r.MinY, r.MaxX, r.MaxY));
	if	(
		(x > r.MaxX) ||
		(x < r.MinX) ||
		(y > r.MaxY) ||
		(y < r.MinY)
		)
	{
		return false;
	}

	//DEBUG_OUT(("%s %d: x and y were in this rect", __func__, __LINE__));
	
	return true;
}


// Position one rect within the bounds of another. Horizontally: centers the hero rect within the left/right of the frame rect; Vertically: centers or or puts at 25% line
// put the frame coords into the frame_rect, and the object to be centered into the hero_rect. ON return, the frame rect will hold the coords to be used.
void General_CenterRectWithinRect(Rectangle* the_frame_rect, Rectangle* the_hero_rect, bool at_25_percent_v)
{
	int16_t			hero_height = the_hero_rect->MaxY - the_hero_rect->MinY;
	int16_t			hero_width = the_hero_rect->MaxX - the_hero_rect->MinX;
	int16_t			frame_height = the_frame_rect->MaxY - the_frame_rect->MinY;
	
	// horizontal: center left/right
	the_frame_rect->MinX = (the_frame_rect->MaxX - the_frame_rect->MinX - hero_width) / 2 + the_frame_rect->MinX;
	the_frame_rect->MaxX = the_frame_rect->MinX + hero_width;

	if (at_25_percent_v == true)
	{
		int16_t			proposed_top;

		// set at 25% of vertical (good for showing an about window, for example)
		proposed_top = frame_height / 4;
		
		// make sure there was actually enough space
		if ((proposed_top + hero_height) > the_frame_rect->MaxY)
		{
			// commented out code below works, but it would be better to just center it vertically at this point
			//the_frame_rect->MinY = proposed_top - ((proposed_top + hero_height) - the_frame_rect->bottom);
			at_25_percent_v = false; // let fall through
		}
		else
		{
			the_frame_rect->MinY = proposed_top;
		}
	}

	if (at_25_percent_v == false)
	{
		// vertical: center top/bottom
		the_frame_rect->MinY = (frame_height - hero_height) / 2 + the_frame_rect->MinY;
	}

	the_frame_rect->MaxY = the_frame_rect->MinY + hero_height;

	//DEBUG_OUT(("%s %d: coords=%i, %i / %i, %i", __func__ , __LINE__, the_frame_rect->MinX, the_frame_rect->MinY, the_frame_rect->MaxX, the_frame_rect->MaxY));
}


//! Copy values of one rect to another
//! @param	r1 - the rectangle to be overwritten (copied into)
//! @param	r2 - the rectangle to copy
void General_CopyRect(Rectangle* r1, Rectangle* r2)
{
	memcpy(r1, r2, sizeof(Rectangle));
}


//! Calculate the difference between 2 rectangles and populate 0, 1, 2, 3, or 4 new rectangles with the difference
//! If Rect 1 is larger than Rect 2, no new rect will be populated
//! If Rect 1 is smaller than Rect 2 in one dimension (axis) only, 1 new rect will be populated
//! If Rect 1 is smaller than Rect 2 in two dimensions (axes), 3 new rect will be populated
//! If Rect 1 is same size as Rect 2 and moved in one dimension (axis) only, 1 new rect will be populated
//! If Rect 1 is same size as Rect 2 and moved in two dimensions (axes), 3 new rect will be populated
//! @param	r1 - the lead, or foreground rect. When calculating a damage rect, this would typically be the rect of the window after it is moved/resized.
//! @param	r2 - the secondary, or background rect. When calculating a damage rect, this would typically be the rect of the window before it is moved/resized.
//! @param	diff_r1 - valid pointer to a rect object that will be populated if there is 1 or 3 difference rects resulting from the operation
//! @param	diff_r2 - valid pointer to a rect object that will be populated if there are 2 or more difference rects resulting from the operation
//! @param	diff_r3 - valid pointer to a rect object that will be populated if there are 3 or more difference rects resulting from the operation
//! @param	diff_r4 - valid pointer to a rect object that will be populated if there are 4 difference rects resulting from the operation
//! @return	Returns number of new rects that represent the difference between the passed rectangles. This indicates how many, if any, of the diff_rects need to be evaluated. Returns -1 on any error condition.
int16_t General_CalculateRectDifference(Rectangle* r1, Rectangle* r2, Rectangle* diff_r1, Rectangle* diff_r2, Rectangle* diff_r3, Rectangle* diff_r4)
{
	int16_t		num_rects = 0;
	Rectangle*	diff_rects[4];
	
	// LOGIC:
	//   if the 2 rects don't intersect at all, diff_r1 can be r2
	//   if rect a is bigger than rect b in all dimensions, there will be no diff rects. 
	//   if they do intersect, use 4 zone rect difference algo to get the rects
	
	if (diff_r1 == NULL || diff_r2 == NULL || diff_r3 == NULL || diff_r4 == NULL)
	{
		return -1;
	}
	
	if (General_RectWithinRect(*r2, *r1) == true)
	{
		//DEBUG_OUT(("%s %d: r1 (%i, %i : %i, %i) ---- r2 (%i, %i : %i, %i)", __func__, __LINE__, r1->MinX, r1->MinY, r1->MaxX, r1->MaxY, r2->MinX, r2->MinY, r2->MaxX, r2->MaxY));
		return 0;
	}
	
	if (General_RectIntersect(*r1, *r2) == false)
	{
		General_CopyRect(diff_r1, r1);
		//DEBUG_OUT(("%s %d: r1 (%i, %i : %i, %i) ---- diff_r1 (%i, %i : %i, %i)", __func__, __LINE__, r1->MinX, r1->MinY, r1->MaxX, r1->MaxY, diff_r1->MinX, diff_r1->MinY, diff_r1->MaxX, diff_r1->MaxY));
		return 1;
	}

	DEBUG_OUT(("%s %d: r1 (%i, %i : %i, %i) ---- r2 (%i, %i : %i, %i)", __func__, __LINE__, r1->MinX, r1->MinY, r1->MaxX, r1->MaxY, r2->MinX, r2->MinY, r2->MaxX, r2->MaxY));
	
	// for any other case, we need to calculate 1-4 rects
	diff_rects[0] = diff_r1;
	diff_rects[1] = diff_r2;
	diff_rects[2] = diff_r3;
	diff_rects[3] = diff_r4;
	
	// top rectangle?
	if (r2->MinY < r1->MinY)
	{
		diff_rects[num_rects]->MinY = r2->MinY;
		diff_rects[num_rects]->MaxY = r1->MinY - 1;
		diff_rects[num_rects]->MinX = r1->MinX;
		diff_rects[num_rects]->MaxX = r1->MaxX;
		DEBUG_OUT(("%s %d: diff rect# %i (%i, %i : %i, %i)", __func__, __LINE__, num_rects, diff_rects[num_rects]->MinX, diff_rects[num_rects]->MinY, diff_rects[num_rects]->MaxX, diff_rects[num_rects]->MaxY));
		num_rects++;
		
	}
	
	// bottom rectangle?
	if (r2->MaxY > r1->MaxY)
	{
		diff_rects[num_rects]->MinY = r1->MaxY + 1;
		diff_rects[num_rects]->MaxY = r2->MaxY;
		diff_rects[num_rects]->MinX = r1->MinX;
		diff_rects[num_rects]->MaxX = r1->MaxX;
		DEBUG_OUT(("%s %d: diff rect# %i (%i, %i : %i, %i)", __func__, __LINE__, num_rects, diff_rects[num_rects]->MinX, diff_rects[num_rects]->MinY, diff_rects[num_rects]->MaxX, diff_rects[num_rects]->MaxY));
		num_rects++;
		
	}

	// left rectangle?
	if (r2->MinX < r1->MinX)
	{
		diff_rects[num_rects]->MinX = r2->MinX;
		diff_rects[num_rects]->MaxX = r1->MinX - 1;
		diff_rects[num_rects]->MinY = r2->MinY;
		diff_rects[num_rects]->MaxY = r2->MaxY;
		DEBUG_OUT(("%s %d: diff rect# %i (%i, %i : %i, %i)", __func__, __LINE__, num_rects, diff_rects[num_rects]->MinX, diff_rects[num_rects]->MinY, diff_rects[num_rects]->MaxX, diff_rects[num_rects]->MaxY));
		num_rects++;
		
	}
	
	// right rectangle?
	if (r2->MaxX > r1->MaxX)
	{
		diff_rects[num_rects]->MinX = r1->MaxX + 1;
		diff_rects[num_rects]->MaxX = r2->MaxX;
		diff_rects[num_rects]->MinY = r2->MinY;
		diff_rects[num_rects]->MaxY = r2->MaxY;
		DEBUG_OUT(("%s %d: diff rect# %i (%i, %i : %i, %i)", __func__, __LINE__, num_rects, diff_rects[num_rects]->MinX, diff_rects[num_rects]->MinY, diff_rects[num_rects]->MaxX, diff_rects[num_rects]->MaxY));
		num_rects++;
		
	}
	
	return num_rects;
}


//! Calculate the intersection between 2 rectangles, storing result in the 3rd rect passed
//! @param	r1 - valid pointer to a rect object
//! @param	r2 - valid pointer to a rect object
//! @param	intersect_r - valid pointer to a rect object that will contain the intersection rectangle, if any, at end of operation
//! @return:	Returns true if there is an intersecting rectangle between r1 and r2.
bool General_CalculateRectIntersection(Rectangle* r1, Rectangle* r2, Rectangle* intersect_r)
{
	if (r1 == NULL || r2 == NULL || intersect_r == NULL)
	{
		return false;
	}
	
	if (General_RectIntersect(*r1, *r2) == false)
	{
		return false;
	}

	DEBUG_OUT(("%s %d: r1 (%i, %i : %i, %i) ---- r2 (%i, %i : %i, %i)", __func__, __LINE__, r1->MinX, r1->MinY, r1->MaxX, r1->MaxY, r2->MinX, r2->MinY, r2->MaxX, r2->MaxY));
	
	intersect_r->MinX = General_ShortMax(r1->MinX, r2->MinX);
	intersect_r->MinY = General_ShortMax(r1->MinY, r2->MinY);
	intersect_r->MaxX = General_ShortMin(r1->MaxX, r2->MaxX);
	intersect_r->MaxY = General_ShortMin(r1->MaxY, r2->MaxY);

	DEBUG_OUT(("%s %d: intersect_r (%i, %i : %i, %i)", __func__, __LINE__, intersect_r->MinX, intersect_r->MinY, intersect_r->MaxX, intersect_r->MaxY));
	
	return true;
}





// **** FILENAME AND FILEPATH UTILITIES *****



// allocate and return the portion of the path passed, minus the filename. In other words: return a path to the parent file.
// calling method must free the string returned
char* General_ExtractPathToParentFolderWithAlloc(const char* the_file_path)
{
	// LOGIC: 
	//   PathPart includes the : if non-name part is for a volume. but doesn't not include trailing / if not a volume
	//   we want in include the trailing : and /, so calling routine can always just append a file name and get a legit path
	
	signed long	path_len;
	char*			the_directory_path;

	// get a string for the directory portion of the filepath
	if ( (the_directory_path = (char*)calloc(FILE_MAX_PATHNAME_SIZE, sizeof(char)) ) == NULL)
	{
		LOG_ERR(("%s %d: could not allocate memory for the directory path", __func__ , __LINE__));
		return NULL;
	}
	LOG_ALLOC(("%s %d:	__ALLOC__	the_directory_path	%p	size	%i", __func__ , __LINE__, the_directory_path, FILE_MAX_PATHNAME_SIZE * sizeof(char)));
	TRACK_ALLOC((FILE_MAX_PATHNAME_SIZE * sizeof(char)));
	
	path_len = (General_PathPart(the_file_path) - the_file_path) - 1;
	
	//DEBUG_OUT(("%s %d: pathlen=%lu; last char='%c'", __func__ , __LINE__, path_len, the_file_path[path_len]));

	if (the_file_path[path_len] != ':')
	{
		// path wasn't to root of a volume, move 1 tick to the right to pick up the / that is already in the full path
		path_len++;
	}

	path_len++;

	General_Strlcpy(the_directory_path, the_file_path, path_len + 1);
	//DEBUG_OUT(("%s %d: pathlen=%lu; parent path='%s'", __func__ , __LINE__, path_len, the_directory_path));
	
	return the_directory_path;
}


// allocate and return the filename portion of the path passed.
// calling method must free the string returned
char* General_ExtractFilenameFromPathWithAlloc(const char* the_file_path)
{
	char*	the_file_name;

	// get a string for the file name portion of the filepath
	if ( (the_file_name = (char*)calloc(FILE_MAX_PATHNAME_SIZE, sizeof(char)) ) == NULL)
	{
		LOG_ERR(("%s %d: could not allocate memory for the filename", __func__ , __LINE__));
		return NULL;
	}
	else
	{
		char*	the_file_name_part = General_NamePart(the_file_path);
		int16_t	filename_len = General_Strnlen(the_file_name_part, FILE_MAX_PATHNAME_SIZE);

		if (filename_len == 0)
		{
			// FilePart() might return a string with no text: that would indicate the file path is for the root of a file system or virtual device
			// in that case, we just use the file path minus : as the name

			// copy the part of the path minus the last char into the file name
			int16_t		path_len = General_Strnlen(the_file_path, FILE_MAX_PATHNAME_SIZE);
			General_Strlcpy(the_file_name, the_file_path, path_len);
		}
		else
		{
			General_Strlcpy(the_file_name, the_file_name_part, filename_len + 1);
		}
		LOG_ALLOC(("%s %d:	__ALLOC__	the_file_name	%p	size	%i", __func__ , __LINE__, the_file_name, filename_len));
		TRACK_ALLOC((filename_len)); // close enough
	}

	return the_file_name;
}


// populates the passed string by safely combining the passed file path and name, accounting for cases where path is a disk root
void General_CreateFilePathFromFolderAndFile(char* the_combined_path, char* the_folder_path, char* the_file_name)
{
	
	General_Strlcpy(the_combined_path, the_folder_path, FILE_MAX_PATHNAME_SIZE);

	// if the filename passed was empty, just return the original folder path. 
	//   otherwise you end up with "mypath" and file "" = "mypath/", which is bad. 
	if (the_file_name[0] == '\0')
	{
		return;
	}
	
	General_Strlcat(the_combined_path, the_file_name, FILE_MAX_PATHNAME_SIZE);

	//DEBUG_OUT(("%s %d: file '%s' and folder '%s' produces path of '%s'", __func__ , __LINE__, the_file_name, the_folder_path, the_combined_path));
}


//! return the first char of the last part of a file path
//! if no path part detected, returns the original string
//! not guaranteed that this is a FILENAME, as if you passed a path to a dir, it would return the DIR name
//!  ex: General_NamePart("/hd/yyy/zzz/myfile.txt") would return a pointer to 'myfile.txt'.
//!  ex: General_NamePart("myfile.txt") would return a pointer to 'myfile.txt'.
//! amigaDOS compatibility function (see FilePart)
char* General_NamePart(const char* the_file_path)
{
	char*	last_slash;
	
	last_slash = strchr(the_file_path, '/');
	
	if (last_slash && ++last_slash)
	{
		return last_slash;
	}
	
	return (char*)the_file_path;
}


//! Returns a pointer to the end of the next-to-last component of a path.
//!  ex: General_PathPart("/hd/yyy/zzz/myfile.txt") would return a pointer to '/myfile.txt'.
//!  ex: General_PathPart("myfile.txt") would return a pointer to 'myfile.txt'.
//! amigaDOS compatibility function (see PathPart)
char* General_PathPart(const char* the_file_path)
{
	char*	the_directory_path;
	char*	this_point;
	
	this_point = (char*)the_file_path;
	the_directory_path = this_point; // default to returning start of the string
	
	while (*this_point)
	{
		if (*this_point == '/')
		{
			the_directory_path = this_point;
		}
		
		this_point++;
	}
	
	return the_directory_path;
}


//! Extract file extension into the passed char pointer, as new lowercased string pointer, if any found.
//! @param	the_file_name - the file name to extract an extension from
//! @param	the_extension - a pre-allocated buffer that will contain the extension, if any is detected. Must be large enough to hold the extension! No bounds checking is done. 
//! @return	Returns false if no file extension found.
bool General_ExtractFileExtensionFromFilename(const char* the_file_name, char* the_extension)
{
	// LOGIC: 
	//   if the first char is the first dot from right, we'll count the whole thing as an extension
	//   if no dot char, then don't set extension, and return false
	
    char*	dot = strrchr((char*)the_file_name, '.');
    int16_t	i;

    // (re) set the file extension to "" in case we have to return. It may have a value from whatever previous use was
    the_extension[0] = '\0';

	if(!dot)
    {
    	return false;
    }

	for (i = 1; dot[i]; i++)
	{
		the_extension[i-1] = General_ToLower(dot[i]);
	}

	the_extension[i-1] = '\0';

	return true;
}


//! Extract core part of the file name, not including the extension
//! @param	the_file_name - the file name to extract an extension from
//! @param	the_core_part - a pre-allocated buffer that will contain the pre-extension part of the file name, if any is detected. Must be large enough to hold the entire file name! No bounds checking is done. If no extension is found, this will contain an empty string.
//! @return	Returns false if no file extension found (because what is "core" part without an extension)
bool General_ExtractCoreFilename(const char* the_file_name, char* the_core_part)
{
	// LOGIC: 
	//   if the first char is the first dot from right, we'll say there was no extension and return false
	//   if no dot char, then don't set extension, and return false
	
    char*	dot = strrchr((char*)the_file_name, '.');
    //int16_t	i;
    uint8_t		the_len;

	if(!dot)
    {
		the_core_part[0] = '\0';
    	return false;
    }

	the_len = (uint8_t)(dot - the_file_name);
	
	memcpy(the_core_part, the_file_name, the_len);
    the_core_part[the_len] = '\0';

	return true;
}




// **** TIME UTILITIES *****


//! Wait for the specified number of ticks before returning
//! In multi-tasking ever becomes a thing, this is not a multi-tasking-friendly operation. 
void General_DelayTicks(int32_t ticks)
{
	long	start_ticks = sys_time_jiffies();
	long	now_ticks = start_ticks;
	
	while ((now_ticks - start_ticks) < ticks)
	{
		now_ticks = sys_time_jiffies();
	}
}


//! Wait for the specified number of seconds before returning
//! In multi-tasking ever becomes a thing, this is not a multi-tasking-friendly operation. 
void General_DelaySeconds(uint16_t seconds)
{
	long	start_ticks = sys_time_jiffies();
	long	now_ticks = start_ticks;
	
	while ((now_ticks - start_ticks) / SYS_TICKS_PER_SEC < seconds)
	{
		now_ticks = sys_time_jiffies();
	}
}








// **** USER INPUT UTILITIES *****

// Wait for one character from the keyboard and return it
char General_GetChar(void)
{
	uint8_t		the_char;
	
	the_char = sys_chan_read_b(0);
	
	return the_char;
}





// **** MISC UTILITIES *****


//! Swap bytes big endian <> little endian, for a long
//! NOTE: for a word, see BSWAP() macro
uint32_t General_ByteSwapLong(uint32_t long_needing_swap)
{
	uint32_t	swapped = 0;
	uint32_t	b0;
	uint32_t	b1;
	uint32_t	b2;
	uint32_t	b3;
	
	b0 = (long_needing_swap & 0xff) << 24;			// least significant to most significant
	b1 = (long_needing_swap & 0xff00) << 8;			// 2nd least sig. to 2nd most sig.
	b2 = (long_needing_swap & 0xff0000) >> 8;		// 2nd most sig. to 2nd least sig.
	b3 = (long_needing_swap & 0xff000000) >> 24;	// most sig. to least sig.
	swapped = b0 | b1 | b2 | b3;
	
	return swapped;
}


// get random number between 1 and the_range + 1
// must have seeded the number generator first
// if passed 0, returns 0.
uint16_t General_GetRandom(uint16_t the_range)
{
	uint16_t the_num = 0;
	uint16_t the_random;

	// reason we have this check is that we'll get unexpected results if you do %0
	if (the_range == 0)
	{
		return 0;
	}

	the_random = R32(RANDOM_NUM_GEN) & 0xFFFF;
	the_num = the_random % the_range + 1;

	return the_num;
}






