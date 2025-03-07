/*
 * bitmap.c
 *
 *  Created on: Mar 10, 2022
 *      Author: micahbly
 */





/*****************************************************************************/
/*                                Includes                                   */
/*****************************************************************************/

// project includes
#include "bitmap.h"
#include "debug.h"

// C includes
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// A2560 includes
#include "a2560k.h"
#include <mcp/syscalls.h>


/*****************************************************************************/
/*                               Definitions                                 */
/*****************************************************************************/



/*****************************************************************************/
/*                               Enumerations                                */
/*****************************************************************************/



/*****************************************************************************/
/*                             Global Variables                              */
/*****************************************************************************/



/*****************************************************************************/
/*                       Private Function Prototypes                         */
/*****************************************************************************/

//! \cond PRIVATE

// validate the coordinates are within the bounds of the specified screen
bool Bitmap_ValidateXY(Bitmap* the_bitmap, int16_t x, int16_t y);

//! Calculate the VRAM location of the specified coordinate within the bitmap
//! @param	the_bitmap -- reference to a valid Bitmap object.
//! @param	x -- the horizontal position, between 0 and bitmap width - 1
//! @param	y -- the vertical position, between 0 and bitmap height - 1
//! @return Returns a pointer to the VRAM location that corresponds to the passed X, Y, or NULL on any error condition
unsigned char* Bitmap_GetMemLocForXY(Bitmap* the_bitmap, int16_t x, int16_t y);

//! Calculate the VRAM location of the specified coordinate within the bitmap
//! @param	the_bitmap -- reference to a valid Bitmap object.
//! @param	x -- the horizontal position, between 0 and bitmap width - 1
//! @param	y -- the vertical position, between 0 and bitmap height - 1
//! @return Returns an unsigned long that can be converted to the VRAM location that corresponds to the passed X, Y, or NULL on any error condition
uint32_t Bitmap_GetMemLocIntForXY(Bitmap* the_bitmap, int16_t x, int16_t y);

//! Draw 1 to 4 quadrants of a circle
//! Only the specified quadrants will be drawn. This makes it possible to use this to make round rects, by only passing 1 quadrant.
//! Based on http://rosettacode.org/wiki/Bitmap/Midpoint_circle_algorithm#C
bool Bitmap_DrawCircleQuadrants(Bitmap* the_bitmap, int16_t x1, int16_t y1, int16_t radius, uint8_t the_color, bool ne, bool se, bool sw, bool nw);

//! Perform a flood fill starting at the coordinate passed. 
bool Bitmap_Fill(Bitmap* the_bitmap, int16_t x, int16_t y, uint8_t the_color);

// **** Debug functions *****

void Bitmap_Print(Bitmap* the_bitmap);

//! \endcond


/*****************************************************************************/
/*                       Private Function Definitions                        */
/*****************************************************************************/

// **** NOTE: all functions in private section REQUIRE pre-validated parameters. 
// **** NEVER call these from your own functions. Always use the public interface. You have been warned!


//! \cond PRIVATE

//! Validate the coordinates are within the bounds of the specified bitmap. 
bool Bitmap_ValidateXY(Bitmap* the_bitmap, int16_t x, int16_t y)
{
	int16_t			max_row;
	int16_t			max_col;
	
	max_col = the_bitmap->width_ - 1;
	max_row = the_bitmap->height_ - 1;
	
	if (x < 0 || x > max_col || y < 0 || y > max_row)
	{
		return false;
	}

	return true;
}


//! Draw 1 to 4 quadrants of a circle
//! Only the specified quadrants will be drawn. This makes it possible to use this to make round rects, by only passing 1 quadrant.
//! NO VALIDATION PERFORMEND ON PARAMETERS. CALLING METHOD MUST VALIDATE.
//! Based on http://rosettacode.org/wiki/Bitmap/Midpoint_circle_algorithm#C
bool Bitmap_DrawCircleQuadrants(Bitmap* the_bitmap, int16_t x1, int16_t y1, int16_t radius, uint8_t the_color, bool ne, bool se, bool sw, bool nw)
{
    int16_t	f;
    int16_t	ddF_x;
    int16_t	ddF_y;
    int16_t	x;
    int16_t	y;
 
	f = 1 - radius;
	ddF_x = 0;
	ddF_y = -2 * radius;
	x = 0;
	y = radius;

	if (se || sw)
	{
		Bitmap_SetPixelAtXY(the_bitmap, x1, y1 + radius, the_color);
	}
	if (ne || nw)
	{
		Bitmap_SetPixelAtXY(the_bitmap, x1, y1 - radius, the_color);
	}
	if (se || ne)
	{
		Bitmap_SetPixelAtXY(the_bitmap, x1 + radius, y1, the_color);
	}
	if (nw || sw)
	{
		Bitmap_SetPixelAtXY(the_bitmap, x1 - radius, y1, the_color);
	}
 
    while(x < y) 
	{
		if(f >= 0) 
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}

		x++;
		ddF_x += 2;
		f += ddF_x + 1;  
 
 		if (se)
        {
			Bitmap_SetPixelAtXY(the_bitmap, x1 + x, y1 + y, the_color);
			Bitmap_SetPixelAtXY(the_bitmap, x1 + y, y1 + x, the_color);
        }
 
 		if (sw)
        {
			Bitmap_SetPixelAtXY(the_bitmap, x1 - x, y1 + y, the_color);
			Bitmap_SetPixelAtXY(the_bitmap, x1 - y, y1 + x, the_color);
        }
 
 		if (ne)
        {
			Bitmap_SetPixelAtXY(the_bitmap, x1 + x, y1 - y, the_color);
			Bitmap_SetPixelAtXY(the_bitmap, x1 + y, y1 - x, the_color);
        }
 
 		if (nw)
        {
			Bitmap_SetPixelAtXY(the_bitmap, x1 - x, y1 - y, the_color);
			Bitmap_SetPixelAtXY(the_bitmap, x1 - y, y1 - x, the_color);
        }
    }
    
    return true;
}


//! Perform a flood fill starting at the coordinate passed. 
//! WARNING: this function is recursive, and if applied to a size even 1/10th the size of the screen, it can eat the stack. Either do not use this, or control its usage to just situations you can control. Or set an enormous stack size when building your app.
//! @param	the_color -- a 1-byte index to the current LUT
bool Bitmap_Fill(Bitmap* the_bitmap, int16_t x, int16_t y, uint8_t the_color)
{   
	int16_t	height;
	int16_t	width;
	
	width = the_bitmap->width_;
	height = the_bitmap->height_;

    if ( 0 <= y && y < height && 0 <= x && x < width && Bitmap_GetPixelAtXY(the_bitmap, x, y) != the_color )
    {
        Bitmap_SetPixelAtXY(the_bitmap, x, y, the_color);
        Bitmap_Fill(the_bitmap, x-1, y, the_color);
        Bitmap_Fill(the_bitmap, x+1, y, the_color);
        Bitmap_Fill(the_bitmap, x, y-1, the_color);
        Bitmap_Fill(the_bitmap, x, y+1, the_color);
    }    
    
    return true;
}



// **** Debug functions *****

void Bitmap_Print(Bitmap* the_bitmap)
{
	DEBUG_OUT(("Bitmap print out:"));
	DEBUG_OUT(("  address: %p",			the_bitmap));
	DEBUG_OUT(("  width_: %i",			the_bitmap->width_));	
	DEBUG_OUT(("  height_: %i",			the_bitmap->height_));	
	DEBUG_OUT(("  x_: %i",				the_bitmap->x_));	
	DEBUG_OUT(("  y_: %i",				the_bitmap->y_));	
	DEBUG_OUT(("  color_: %u",			the_bitmap->color_));	
	DEBUG_OUT(("  reserved_: %u",		the_bitmap->reserved_));	
	DEBUG_OUT(("  font_: %p",			the_bitmap->font_));	
	DEBUG_OUT(("  addr_: %p",			the_bitmap->addr_));
	DEBUG_OUT(("  addr_int_: %lx",		the_bitmap->addr_int_));
}

//! \endcond



/*****************************************************************************/
/*                        Public Function Definitions                        */
/*****************************************************************************/

// **** CONSTRUCTOR AND DESTRUCTOR *****

// constructor

//! Create a new bitmap object by allocating space for the bitmap struct in regular memory, and for the graphics, in VRAM
//! NOTE: when creating a bitmap to represent something actually in VRAM, pass true to in_vram, and manually assign a known VRAM location afterwards.
//! @param	width -- width, in pixels, of the bitmap to be created
//! @param	height -- height, in pixels, of the bitmap to be created
//! @param	the_font -- optional font object to associate with the Bitmap. 
//! @param	in_vram -- if true, no space will be allocated for the bitmap graphics. If false, width * height area of memory will be allocated in standard memory.
Bitmap* Bitmap_New(int16_t width, int16_t height, Font* the_font, bool in_vram)
{
	Bitmap*		the_bitmap;

	//DEBUG_OUT(("%s %d: start bitmap creation... (%i x %i, %p)", __func__, __LINE__, width, height, the_font));

	// check width/height for some maximum??
	// TODO
	if ( (width < 2 || width > VICKY_BITMAP_MAX_H_RES) || (height < 2 || height > VICKY_BITMAP_MAX_V_RES) )
	{
		LOG_ERR(("%s %d: Illegal width (%i) and/or height (%i)", __func__, __LINE__, width, height));
		goto error;
	}

	//DEBUG_OUT(("%s %d: allocating struct in normal memory...", __func__, __LINE__));
	
	// LOGIC:
	//   we have 2 kinds of memory: VRAM and standard RAM
	//   A bitmap object needs a struct which can and should be allocated in normal memory
	//   If the bitmap actually represents something on the screen, it needs to point to VRAM, not normal memory
	
	if ((the_bitmap = calloc(1, sizeof(Bitmap))) == NULL)
	{
		LOG_ERR(("%s %d: Couldn't allocate space for bitmap struc", __func__, __LINE__));
		goto error;
	}
	LOG_ALLOC(("%s %d:	__ALLOC__	the_bitmap struct	%p	size	%i", __func__ , __LINE__, the_bitmap, sizeof(Bitmap)));
	TRACK_ALLOC((sizeof(Bitmap)));

	if (in_vram == false)
	{
		//DEBUG_OUT(("%s %d: Allocating a screen-sized bitmap in standard RAM...", __func__, __LINE__));

		if ((the_bitmap->addr_ = calloc(sizeof(uint8_t), width * height)) == NULL)
		{
			LOG_ERR(("%s %d: Couldn't instantiate a bitmap", __func__, __LINE__));
			goto error;
		}
		LOG_ALLOC(("%s %d:	__ALLOC__	the_bitmap data	%p	size	%i", __func__ , __LINE__, the_bitmap, sizeof(uint8_t) * width * height));
		TRACK_ALLOC((sizeof(uint8_t) * width * height));
		
		the_bitmap->addr_int_ = (uint32_t)the_bitmap->addr_;
	}
	else
	{
		the_bitmap->addr_ = NULL;
		the_bitmap->addr_int_ = 0;
	}

	the_bitmap->width_ = width;
	the_bitmap->height_ = height;
	the_bitmap->in_vram_ = in_vram;
	
	//DEBUG_OUT(("%s %d: Bitmap allocated! p=%p, addr=%p, width=%i, height=%i", __func__, __LINE__, the_bitmap, the_bitmap->addr_, the_bitmap->width_, the_bitmap->height_));

	if (the_font)
	{
		if (Bitmap_SetFont(the_bitmap, the_font) == false)
		{
			LOG_ERR(("%s %d: Couldn't assign the font to the bitmap", __func__, __LINE__));
			goto error;
		}
	}
		
	//Bitmap_Print(the_bitmap);
	
	return the_bitmap;
	
error:
	return NULL;
}


// destructor
// frees all allocated memory associated with the passed object, and the object itself
bool Bitmap_Destroy(Bitmap** the_bitmap)
{
	if (*the_bitmap == NULL)
	{
		LOG_ERR(("%s %d: passed class object was null", __func__ , __LINE__));
		return false;
	}

	// LOGIC: 
	//   Bitmaps optionally have fonts associated with them.
	//   We do not want to destroy these, because other objects could be using them.
	//   TODO: notify the system that we are no longer using this font (system needs to be tallying usage count for that to work)
	
	if ((*the_bitmap)->font_)
	{
		(*the_bitmap)->font_ = NULL;
	}

	if ((*the_bitmap)->addr_)
	{
		if ((*the_bitmap)->in_vram_ == false)
		{
			LOG_ALLOC(("%s %d:	__FREE__	the_bitmap->addr_	%p	size	%i", __func__ , __LINE__, (*the_bitmap)->addr_, (*the_bitmap)->width_ * (*the_bitmap)->height_));
			TRACK_ALLOC((0 - (*the_bitmap)->width_ * (*the_bitmap)->height_));
			free((*the_bitmap)->addr_);
		}
	}

	LOG_ALLOC(("%s %d:	__FREE__	*the_bitmap	%p	size	%i", __func__ , __LINE__, *the_bitmap, sizeof(Bitmap)));
	TRACK_ALLOC((0 - sizeof(Bitmap)));
	free(*the_bitmap);
	*the_bitmap = NULL;
	
	return true;
}


//! Resize and existing bitmap by setting new width/height and allocating bigger storage if necessary
//! NOTE: if the bitmap is held in VRAM, storage will not be reallocated
//! NOTE: if the new size for the bitmap is smaller than the previous size, storage will not be reallocated - extra bytes will simply not be used
//! @param	width -- the new width, in pixels, to resize the bitmap to
//! @param	height -- the new height, in pixels, to resize the bitmap to
//! @return	Returns false in any error condition
bool Bitmap_Resize(Bitmap* the_bitmap, int16_t width, int16_t height)
{
	size_t	old_size;
	size_t	new_size;
	
	if (the_bitmap == NULL)
	{
		LOG_ERR(("%s %d: passed class object was null", __func__ , __LINE__));
		return false;
	}

	DEBUG_OUT(("%s %d: start bitmap resizing; old = %i x %i; new=%i x %i", __func__, __LINE__, the_bitmap->width_, the_bitmap->height_, width, height));

	old_size = the_bitmap->width_ * the_bitmap->height_;
	new_size = width * height;
	
	the_bitmap->width_ = width;
	the_bitmap->height_ = height;
	
	// Reallocate bitmap only if both are true:
	//   window is not in VRAM. we only store backdrop in VRAM, and it shares same bitmap as the screen.
	//   window got LARGER. if it got smaller, we can just keep reusing same bitmap and not worry about the extra space.
	if (the_bitmap->in_vram_ == false && new_size > old_size)
	{
		if (the_bitmap->addr_)
		{
			LOG_ALLOC(("%s %d:	__FREE__	the_bitmap->addr_	%p	size	%i", __func__ , __LINE__, the_bitmap->addr_, old_size));
			TRACK_ALLOC((0 - old_size));
			free(the_bitmap->addr_);
		}
		
		if ((the_bitmap->addr_ = calloc(sizeof(uint8_t), new_size)) == NULL)
		{
			LOG_ERR(("%s %d: Couldn't instantiate a bitmap", __func__, __LINE__));
			return false;
		}
		LOG_ALLOC(("%s %d:	__ALLOC__	the_bitmap->addr_	%p	size	%i", __func__ , __LINE__, the_bitmap->addr_, new_size));
		TRACK_ALLOC((new_size));
		
		the_bitmap->addr_int_ = (uint32_t)the_bitmap->addr_;
	}
	
	return true;
}




// **** Block copy functions ****

//! Blit a rect from source bitmap to distination bitmap
//! This is a wrapper around Bitmap_Blit()
//! The source and destination bitmaps can be the same: you can use this to copy a chunk of pixels from one part of a screen to another. If the destination location cannot fit the entirety of the copied rectangle, the copy will be truncated, but will not return an error. 
//! @param	src_bm -- the source bitmap. It must have a valid address within the VRAM memory space.
//! @param	dst_bm -- the destination bitmap. It must have a valid address within the VRAM memory space. It can be the same bitmap as the source.
//! @param	src_rect -- the rectangle from the source bitmap to be blitted to the target bitmap
//! @param	dst_x -- the location within the destination bitmap to copy pixels to. May be negative.
//! @param	dst_y -- the location within the destination bitmap to copy pixels to. May be negative.
bool Bitmap_BlitRect(Bitmap* src_bm, Rectangle* src_rect, Bitmap* dst_bm, int16_t dst_x, int16_t dst_y)
{
	int16_t	width;
	int16_t	height;
	
	// LOGIC: validation checks will be done by Bitmap_Blit, no need to check them here too
	
	width = src_rect->MaxX - src_rect->MinX + 1;
	height = src_rect->MaxY - src_rect->MinY + 1;

	//DEBUG_OUT(("%s %d: incoming parameters: MinX=%i, MinY=%i, MaxX=%i, MaxY=%i", __func__, __LINE__, src_rect->MinX, src_rect->MinY, src_rect->MaxX, src_rect->MaxY));
	//DEBUG_OUT(("%s %d: final parameters: src_x=%i, src_y=%i, dst_x=%i, dst_y=%i, width=%i, height=%i.", __func__, __LINE__, src_rect->MinX, src_rect->MinY, dst_x, dst_y, width, height));
	//DEBUG_OUT(("%s %d: src_bm=%p, dst_bm=%p", __func__, __LINE__, src_bm->addr_, dst_bm->addr_));
	
	return Bitmap_Blit(src_bm, src_rect->MinX, src_rect->MinY, dst_bm, dst_x, dst_y, width, height);
}


//! Blit from source bitmap to distination bitmap. 
//! The source and destination bitmaps can be the same: you can use this to copy a chunk of pixels from one part of a screen to another. If the destination location cannot fit the entirety of the copied rectangle, the copy will be truncated, but will not return an error. 
//! @param	src_bm -- the source bitmap. It must have a valid address within the VRAM memory space.
//! @param	dst_bm -- the destination bitmap. It must have a valid address within the VRAM memory space. It can be the same bitmap as the source.
//! @param	src_x -- the upper left coordinate within the source bitmap, for the rectangle you want to copy. May be negative.
//! @param	src_y -- the upper left coordinate within the source bitmap, for the rectangle you want to copy. May be negative.
//! @param	dst_x -- the location within the destination bitmap to copy pixels to. May be negative.
//! @param	dst_y -- the location within the destination bitmap to copy pixels to. May be negative.
//! @param	width -- the scope of the copy, in pixels.
//! @param	height -- the scope of the copy, in pixels.
bool Bitmap_Blit(Bitmap* src_bm, int16_t src_x, int16_t src_y, Bitmap* dst_bm, int16_t dst_x, int16_t dst_y, int16_t width, int16_t height)
{
	uint32_t		the_read_loc_int;
	uint32_t		the_write_loc_int;
	uint8_t*		the_write_loc;
	uint8_t*		the_read_loc;
	uint32_t		copy_size;
	int16_t			j;
	
	// TODO: move the 2 checks below to a private common function if other blit functions are added
	
	if (src_bm == NULL || dst_bm == NULL)
	{
		LOG_ERR(("%s %d: passed source or destination bitmap was NULL", __func__, __LINE__));
		return false;
	}
	
	if (src_bm->addr_ == NULL || dst_bm->addr_ == NULL)
	{
		LOG_ERR(("%s %d: passed source or destination bitmap had a NULL address", __func__, __LINE__));
		return false;
	}
	
	// LOGIC:
	//   check if any part of the desired rectangle is within visible space in the source.
	//   We want to allow negative starting locations as long as height/width means some part of the rectange is actually in the bitmap. 
	
	if (src_x + width < 0 || src_y + height < 0)
	{
		LOG_INFO(("%s %d: No part of the copy-from rectangle was on screen. No copy performed. src_x=%i, src_y=%i, width=%i, height=%i.", __func__, __LINE__, src_x, src_y, width, height));
		return false;
	}
	
	// LOGIC:
	//   check if any part of the desired rectangle is within visible space in the target.
	//   We want to allow near-right-edge/bottom-edge starting locations as long as some part of the rectange is actually in the bitmap. 
	
	if (dst_x >= dst_bm->width_ || dst_y >= dst_bm->height_)
	{
		LOG_INFO(("%s %d: No part of the copy-to rectangle was on screen. No copy performed. dst_x=%i, dst_y=%i, width=%i, height=%i.", __func__, __LINE__, dst_x, dst_y, width, height));
		return false;
	}

	// adjust copy width/height if the whole image wouldn't fit on target bitmap anyway
	width = (dst_x + width >= dst_bm->width_) ? dst_bm->width_ - dst_x : width;
	height = (dst_y + height >= dst_bm->height_) ? dst_bm->height_ - dst_y : height;
	
	// adjust copy width/height if the starting x,y are offscreen
	if (src_x < 0)
	{
		width -= src_x;
		src_x = 0;
	}

	if (src_y < 0)
	{
		height -= src_y;
		src_y = 0;
	}

	//DEBUG_OUT(("%s %d: final parameters: src_x=%i, src_y=%i, dst_x=%i, dst_y=%i, width=%i, height=%i.", __func__, __LINE__, src_x, src_y, dst_x, dst_y, width, height));

	// checks complete. ready to copy.
	copy_size = (uint32_t)width;
	the_read_loc_int = src_bm->addr_int_ + ((uint32_t)src_bm->width_ * (uint32_t)src_y) + (uint32_t)src_x;
	the_write_loc_int = dst_bm->addr_int_ + ((uint32_t)dst_bm->width_ * (uint32_t)dst_y) + (uint32_t)dst_x;
	//DEBUG_OUT(("%s %d: the_read_loc_int=%i, the_write_loc_int=%i, copy_size=%lu", __func__, __LINE__, the_read_loc_int, the_write_loc_int, copy_size));
	
	for (j = 0; j < height; j++)
	{
		the_write_loc = (uint8_t*)the_write_loc_int;
		the_read_loc = (uint8_t*)the_read_loc_int;
		//DEBUG_OUT(("%s %d: the_read_loc=%p, the_write_loc=%p, copy_size=%lu", __func__, __LINE__, the_read_loc, the_write_loc, copy_size));

		#ifdef _DO_NOT_TRUST_MEMCPY_
			uint32_t i;
			
			for (i=0; i < copy_size; i++)
			{
				*the_write_loc = *the_read_loc;
				the_write_loc++;
				the_read_loc++;
// 				the_write_loc_int++;			
// 				the_read_loc_int++;			
// 				the_write_loc = (uint8_t*)the_write_loc_int;
// 				the_read_loc = (uint8_t*)the_read_loc_int;
			}
		#else
			memcpy(the_write_loc, the_read_loc, copy_size);
		#endif	
		
		the_write_loc_int += (uint32_t)dst_bm->width_;
		the_read_loc_int += (uint32_t)src_bm->width_;
	}

	return true;
}


//! Tile the source bitmap into the destination bitmap, filling it
//! The source and destination bitmaps can be the same: you can use this to copy a chunk of pixels from one part of a screen to another. If the destination location cannot fit the entirety of the copied rectangle, the copy will be truncated, but will not return an error. 
//! @param	src_bm -- the source bitmap. It must have a valid address within the VRAM memory space.
//! @param	dst_bm -- the destination bitmap. It must have a valid address within the VRAM memory space. It can be the same bitmap as the source.
//! @param	src_x -- the upper left coordinate within the source bitmap, for the tile you want to copy. Must be non-negative.
//! @param	src_y -- the upper left coordinate within the source bitmap, for the tile you want to copy. Must be non-negative.
//! @param	width -- the size of the tile to be derived from the source bitmap, in pixels.
//! @param	height -- the size of the tile to be derived from the source bitmap, in pixels.
bool Bitmap_TileV3(Bitmap* src_bm, int16_t src_x, int16_t src_y, Bitmap* dst_bm, int16_t width, int16_t height)
{
	unsigned char*		the_starting_read_loc;
	unsigned char*		the_read_loc;
	unsigned char*		the_write_loc;
	int16_t				i;
	int16_t				h_tiles;
	int16_t				h_rem;
	int16_t				v_tiles;
	int16_t				v_rem;
	int16_t				j;
	int16_t				bytes;
	
	
	// TODO: move the 2 checks below to a private common function if other blit functions are added
	
	if (src_bm == NULL || dst_bm == NULL)
	{
		LOG_ERR(("%s %d: passed source or destination bitmap was NULL", __func__, __LINE__));
		return false;
	}
	
	if (src_bm->addr_ == NULL || dst_bm->addr_ == NULL)
	{
		LOG_ERR(("%s %d: passed source or destination bitmap had a NULL address", __func__, __LINE__));
		return false;
	}
	
	// LOGIC:
	//   The entire width and height of the tile must be within the source bitmap. 
	
	if (src_x < 0 || src_x + width > src_bm->width_ || src_y < 0 || src_y + height > src_bm->height_)
	{
		LOG_INFO(("%s %d: Tile operations require the entire height and width of the tile to be defined within the bounds of the source bitmap. No tiling performed. src_x=%i, src_y=%i, width=%i, height=%i.", __func__, __LINE__, src_x, src_y, width, height));
		return false;
	}

	//DEBUG_OUT(("%s %d: starting parameters: src_x=%i, src_y=%i, width=%i, height=%i.", __func__, __LINE__, src_x, src_y, width, height));
	//DEBUG_OUT(("%s %d: dst_bm=%p, dst_bm=%p", __func__, __LINE__, src_bm, dst_bm));
	
	// LOGIC:
	//   Unlike a blit operation, a tile operation always starts in the upper left corner of the target bitmap, and fills it entirely
	//   As an optimization, each row of one tile worth will be copied to the right to fill one horizontal row of the dest. bitmap
	//   From that point, will loop through the required number of remaining bands, copying the full width of the dest bitmap.
	
	// adjust copy width/height if the prescribed tile height or width wouldn't fit on target bitmap
	width = (width >= dst_bm->width_) ? dst_bm->width_ : width;
	height = (height >= dst_bm->height_) ? dst_bm->height_ : height;

	// checks complete. ready to tile. 
	the_starting_read_loc = src_bm->addr_ + (src_bm->width_ * src_y) + src_x;
	
	h_tiles = dst_bm->width_ / width;
	h_rem = dst_bm->width_ % width;
	
	// first loop is to create 1 pixel row worth extending across the entire target bitmap
	// and loop that for each row in the source tile
	for (i = 0; i < height; i++)
	{
		the_write_loc = dst_bm->addr_ + dst_bm->width_ * i; // x=0, row = i

		for (j = 0; j < h_tiles; j++)
		{
			the_read_loc = the_starting_read_loc + src_bm->width_ * i;
			for (bytes = 0; bytes < width; bytes++)
			{
				*the_write_loc++ = *the_read_loc++;
			}
// 			memcpy(the_write_loc, the_read_loc, width);
// 	
// 			the_write_loc += width;
		}
		
		// the last horizontal span, if any, will have less width
		if (h_rem)
		{
			//memcpy(the_write_loc, the_read_loc, h_rem);
			the_read_loc = the_starting_read_loc + src_bm->width_ * i;
			
			for (bytes = 0; bytes < h_rem; bytes++)
			{
				*the_write_loc++ = *the_read_loc++;
			}
		}
	}		
	
	
	// have now filled the entire width of the dest. bitmap, to a height of 'height'. tile downwards...
	
	v_tiles = dst_bm->height_ / height;
	v_rem = dst_bm->height_ % height;

	//DEBUG_OUT(("%s %d: h_tiles=%i, h_rem=%i, v_tiles=%i, v_rem=%i", __func__, __LINE__, h_tiles, h_rem, v_tiles, v_rem));

	// now we are reading from the destination bitmap, from the area we prepared. 
	// we will keep reading the same blob over and over again, so read loc is now permanent.
	
	uint32_t write_size = dst_bm->width_ * height;
	the_write_loc = dst_bm->addr_ + (dst_bm->width_ * height * 1); // right after the first band of tile
	
	// each loop is horizontal band of tiles running full length of target bitmap, writing top to bottom, from 2nd vertical band
	for (j = 1; j < v_tiles; j++)	 // 1 because we just wrote one band worth's above.
	{
		the_read_loc = dst_bm->addr_;
		
		for (bytes = 0; bytes < write_size; bytes++)
		{
			*the_write_loc++ = *the_read_loc++;
		}
// 		memcpy(the_write_loc, the_read_loc, write_size);
// 	
// 		the_write_loc += write_size;
	}
	
	// the last tile, if any, will have less height
	if (v_rem)
	{
		height = v_rem;
		write_size = dst_bm->width_ * height;
		the_read_loc = dst_bm->addr_;
		
// 		memcpy(the_write_loc, the_read_loc, write_size);
		for (bytes = 0; bytes < write_size; bytes++)
		{
			*the_write_loc++ = *the_read_loc++;
		}
	}
	
	//DEBUG_OUT(("%s %d: final parameters: src_x=%i, src_y=%i, width=%i, height=%i.", __func__, __LINE__, src_x, src_y, width, height));
	
	return true;
}


//! Tile the source bitmap into the destination bitmap, filling it
//! The source and destination bitmaps can be the same: you can use this to copy a chunk of pixels from one part of a screen to another. If the destination location cannot fit the entirety of the copied rectangle, the copy will be truncated, but will not return an error. 
//! @param	src_bm -- the source bitmap. It must have a valid address within the VRAM memory space.
//! @param	dst_bm -- the destination bitmap. It must have a valid address within the VRAM memory space. It can be the same bitmap as the source.
//! @param	src_x -- the upper left coordinate within the source bitmap, for the tile you want to copy. Must be non-negative.
//! @param	src_y -- the upper left coordinate within the source bitmap, for the tile you want to copy. Must be non-negative.
//! @param	width -- the size of the tile to be derived from the source bitmap, in pixels.
//! @param	height -- the size of the tile to be derived from the source bitmap, in pixels.
bool Bitmap_Tile(Bitmap* src_bm, int16_t src_x, int16_t src_y, Bitmap* dst_bm, int16_t width, int16_t height)
{
	unsigned char*		the_starting_read_loc;
	unsigned char*		the_read_loc;
	unsigned char*		the_write_loc;
	int16_t				i;
	int16_t				h_tiles;
	int16_t				h_rem;
	int16_t				v_tiles;
	int16_t				v_rem;
	int16_t				j;
	
	
	// TODO: move the 2 checks below to a private common function if other blit functions are added
	
	if (src_bm == NULL || dst_bm == NULL)
	{
		LOG_ERR(("%s %d: passed source or destination bitmap was NULL", __func__, __LINE__));
		return false;
	}
	
	if (src_bm->addr_ == NULL || dst_bm->addr_ == NULL)
	{
		LOG_ERR(("%s %d: passed source or destination bitmap had a NULL address", __func__, __LINE__));
		return false;
	}
	
	// LOGIC:
	//   The entire width and height of the tile must be within the source bitmap. 
	
	if (src_x < 0 || src_x + width > src_bm->width_ || src_y < 0 || src_y + height > src_bm->height_)
	{
		LOG_INFO(("%s %d: Tile operations require the entire height and width of the tile to be defined within the bounds of the source bitmap. No tiling performed. src_x=%i, src_y=%i, width=%i, height=%i.", __func__, __LINE__, src_x, src_y, width, height));
		return false;
	}

	//DEBUG_OUT(("%s %d: starting parameters: src_x=%i, src_y=%i, width=%i, height=%i.", __func__, __LINE__, src_x, src_y, width, height));
	//DEBUG_OUT(("%s %d: dst_bm=%p, dst_bm=%p", __func__, __LINE__, src_bm, dst_bm));
	
	// LOGIC:
	//   Unlike a blit operation, a tile operation always starts in the upper left corner of the target bitmap, and fills it entirely
	//   As an optimization, each row of one tile worth will be copied to the right to fill one horizontal row of the dest. bitmap
	//   From that point, will loop through the required number of remaining bands, copying the full width of the dest bitmap.
	
	// adjust copy width/height if the prescribed tile height or width wouldn't fit on target bitmap
	width = (width >= dst_bm->width_) ? dst_bm->width_ : width;
	height = (height >= dst_bm->height_) ? dst_bm->height_ : height;

	// checks complete. ready to tile. 
	the_starting_read_loc = src_bm->addr_ + (src_bm->width_ * src_y) + src_x;
	
	h_tiles = dst_bm->width_ / width;
	h_rem = dst_bm->width_ % width;
	
	// first loop is to create 1 pixel row worth extending across the entire target bitmap
	// and loop that for each row in the source tile
	for (i = 0; i < height; i++)
	{
		the_read_loc = the_starting_read_loc + src_bm->width_ * i;
		the_write_loc = dst_bm->addr_ + dst_bm->width_ * i; // x=0, row = i

		for (j = 0; j < h_tiles; j++)
		{
			memcpy(the_write_loc, the_read_loc, width);
	
			the_write_loc += width;
		}
		
		// the last horizontal span, if any, will have less width
		if (h_rem)
		{
			memcpy(the_write_loc, the_read_loc, h_rem);
		}
	}		
	
	
	// have now filled the entire width of the dest. bitmap, to a height of 'height'. tile downwards...
	
	v_tiles = dst_bm->height_ / height;
	v_rem = dst_bm->height_ % height;

	//DEBUG_OUT(("%s %d: h_tiles=%i, h_rem=%i, v_tiles=%i, v_rem=%i", __func__, __LINE__, h_tiles, h_rem, v_tiles, v_rem));

	// now we are reading from the destination bitmap, from the area we prepared. 
	// we will keep reading the same blob over and over again, so read loc is now permanent.
	the_read_loc = dst_bm->addr_;
	
	uint32_t write_size = dst_bm->width_ * height;
	the_write_loc = dst_bm->addr_ + (dst_bm->width_ * height * 1); // right after the first band of tile
	
	// each loop is horizontal band of tiles running full length of target bitmap, writing top to bottom, from 2nd vertical band
	for (j = 1; j < v_tiles; j++)	 // 1 because we just wrote one band worth's above.
	{
		memcpy(the_write_loc, the_read_loc, write_size);
	
		the_write_loc += write_size;
	}
	
	// the last tile, if any, will have less height
	if (v_rem)
	{
		height = v_rem;
		write_size = dst_bm->width_ * height;
		
		memcpy(the_write_loc, the_read_loc, write_size);
	}
	
	//DEBUG_OUT(("%s %d: final parameters: src_x=%i, src_y=%i, width=%i, height=%i.", __func__, __LINE__, src_x, src_y, width, height));
	
	return true;
}


//! Tile the source bitmap into the destination bitmap, filling it
//! The source and destination bitmaps can be the same: you can use this to copy a chunk of pixels from one part of a screen to another. If the destination location cannot fit the entirety of the copied rectangle, the copy will be truncated, but will not return an error. 
//! @param	src_bm -- the source bitmap. It must have a valid address within the VRAM memory space.
//! @param	dst_bm -- the destination bitmap. It must have a valid address within the VRAM memory space. It can be the same bitmap as the source.
//! @param	src_x -- the upper left coordinate within the source bitmap, for the tile you want to copy. Must be non-negative.
//! @param	src_y -- the upper left coordinate within the source bitmap, for the tile you want to copy. Must be non-negative.
//! @param	width -- the size of the tile to be derived from the source bitmap, in pixels.
//! @param	height -- the size of the tile to be derived from the source bitmap, in pixels.
bool Bitmap_TileV2(Bitmap* src_bm, int16_t src_x, int16_t src_y, Bitmap* dst_bm, int16_t width, int16_t height)
{
	unsigned char*		the_starting_read_loc;
	unsigned char*		the_read_loc;
	unsigned char*		the_write_loc;
	int16_t				i;
	int16_t				h_tiles;
	int16_t				h_rem;
	int16_t				v_tiles;
	int16_t				v_rem;
	int16_t				j;
	
	
	// TODO: move the 2 checks below to a private common function if other blit functions are added
	
	if (src_bm == NULL || dst_bm == NULL)
	{
		LOG_ERR(("%s %d: passed source or destination bitmap was NULL", __func__, __LINE__));
		return false;
	}
	
	if (src_bm->addr_ == NULL || dst_bm->addr_ == NULL)
	{
		LOG_ERR(("%s %d: passed source or destination bitmap had a NULL address", __func__, __LINE__));
		return false;
	}
	
	// LOGIC:
	//   The entire width and height of the tile must be within the source bitmap. 
	
	if (src_x < 0 || src_x + width > src_bm->width_ || src_y < 0 || src_y + height > src_bm->height_)
	{
		LOG_INFO(("%s %d: Tile operations require the entire height and width of the tile to be defined within the bounds of the source bitmap. No tiling performed. src_x=%i, src_y=%i, width=%i, height=%i.", __func__, __LINE__, src_x, src_y, width, height));
		return false;
	}

	//DEBUG_OUT(("%s %d: starting parameters: src_x=%i, src_y=%i, width=%i, height=%i.", __func__, __LINE__, src_x, src_y, width, height));
	//DEBUG_OUT(("%s %d: dst_bm=%p, dst_bm=%p", __func__, __LINE__, src_bm, dst_bm));
	
	// LOGIC:
	//   Unlike a blit operation, a tile operation always starts in the upper left corner of the target bitmap, and fills it entirely
	//   As an optimization, each row of one tile worth will be copied to the right to fill one horizontal row of the dest. bitmap
	//   From that point, will loop through the required number of remaining bands, copying the full width of the dest bitmap.
	
	// adjust copy width/height if the prescribed tile height or width wouldn't fit on target bitmap
	width = (width >= dst_bm->width_) ? dst_bm->width_ : width;
	height = (height >= dst_bm->height_) ? dst_bm->height_ : height;

	// checks complete. ready to tile. 
	the_starting_read_loc = src_bm->addr_ + (src_bm->width_ * src_y) + src_x;
	
	h_tiles = dst_bm->width_ / width;
	h_rem = dst_bm->width_ % width;
	
	// first loop is to create 1 pixel row worth extending across the entire target bitmap
	// and loop that for each row in the source tile
	for (i = 0; i < height; i++)
	{
		the_read_loc = the_starting_read_loc + src_bm->width_ * i;
		the_write_loc = dst_bm->addr_ + dst_bm->width_ * i; // x=0, row = i

		for (j = 0; j < h_tiles; j++)
		{
			memcpy(the_write_loc, the_read_loc, width);
	
			the_write_loc += width;
		}
		
		// the last horizontal span, if any, will have less width
		if (h_rem)
		{
			memcpy(the_write_loc, the_read_loc, h_rem);
		}
	}		
	
	
	// have now filled the entire width of the dest. bitmap, to a height of 'height'. tile downwards...
	
	v_tiles = dst_bm->height_ / height;
	v_rem = dst_bm->height_ % height;

	//DEBUG_OUT(("%s %d: h_tiles=%i, h_rem=%i, v_tiles=%i, v_rem=%i", __func__, __LINE__, h_tiles, h_rem, v_tiles, v_rem));
	
	// each loop is one tile, writing top to bottom, from 2nd vertical band
	for (j = 1; j < v_tiles; j++)	 // 1 because we just wrote one band worth's above.
	{
		the_read_loc = dst_bm->addr_; // now we are reading from the destination bitmap, from the area we prepared
		the_write_loc = dst_bm->addr_ + (dst_bm->width_ * height * j);
		
		for (i = 0; i < height; i++)
		{
			memcpy(the_write_loc, the_read_loc, dst_bm->width_);
		
			the_write_loc += dst_bm->width_;
			the_read_loc += dst_bm->width_;
		}		
	}
	
	// the last tile, if any, will have less height
	if (v_rem)
	{
		the_read_loc = dst_bm->addr_; // now we are reading from the destination bitmap, from the area we prepared
		the_write_loc = dst_bm->addr_ + (dst_bm->width_ * height * j);
		height = v_rem;

		for (i = 0; i < height; i++)
		{
			memcpy(the_write_loc, the_read_loc, dst_bm->width_);
		
			the_write_loc += dst_bm->width_;
			the_read_loc += dst_bm->width_;
		}		
	}
	
	//DEBUG_OUT(("%s %d: final parameters: src_x=%i, src_y=%i, width=%i, height=%i.", __func__, __LINE__, src_x, src_y, width, height));
	
	return true;
}


//! Tile the source bitmap into the destination bitmap, filling it
//! The source and destination bitmaps can be the same: you can use this to copy a chunk of pixels from one part of a screen to another. If the destination location cannot fit the entirety of the copied rectangle, the copy will be truncated, but will not return an error. 
//! @param	src_bm -- the source bitmap. It must have a valid address within the VRAM memory space.
//! @param	dst_bm -- the destination bitmap. It must have a valid address within the VRAM memory space. It can be the same bitmap as the source.
//! @param	src_x -- the upper left coordinate within the source bitmap, for the tile you want to copy. Must be non-negative.
//! @param	src_y -- the upper left coordinate within the source bitmap, for the tile you want to copy. Must be non-negative.
//! @param	width -- the size of the tile to be derived from the source bitmap, in pixels.
//! @param	height -- the size of the tile to be derived from the source bitmap, in pixels.
bool Bitmap_TileV1(Bitmap* src_bm, int16_t src_x, int16_t src_y, Bitmap* dst_bm, int16_t width, int16_t height)
{
	unsigned char*		the_starting_read_loc;
	unsigned char*		the_read_loc;
	unsigned char*		the_write_loc;
	int16_t				i;
	int16_t				h_tiles;
	int16_t				h_rem;
	int16_t				v_tiles;
	int16_t				v_rem;
	int16_t				j;
	
	
	// TODO: move the 2 checks below to a private common function if other blit functions are added
	
	if (src_bm == NULL || dst_bm == NULL)
	{
		LOG_ERR(("%s %d: passed source or destination bitmap was NULL", __func__, __LINE__));
		return false;
	}
	
	if (src_bm->addr_ == NULL || dst_bm->addr_ == NULL)
	{
		LOG_ERR(("%s %d: passed source or destination bitmap had a NULL address", __func__, __LINE__));
		return false;
	}
	
	// LOGIC:
	//   The entire width and height of the tile must be within the source bitmap. 
	
	if (src_x < 0 || src_x + width > src_bm->width_ || src_y < 0 || src_y + height > src_bm->height_)
	{
		LOG_INFO(("%s %d: Tile operations require the entire height and width of the tile to be defined within the bounds of the source bitmap. No tiling performed. src_x=%i, src_y=%i, width=%i, height=%i.", __func__, __LINE__, src_x, src_y, width, height));
		return false;
	}

	//DEBUG_OUT(("%s %d: starting parameters: src_x=%i, src_y=%i, width=%i, height=%i.", __func__, __LINE__, src_x, src_y, width, height));
	//DEBUG_OUT(("%s %d: dst_bm=%p, dst_bm=%p", __func__, __LINE__, src_bm, dst_bm));
	
	// LOGIC:
	//   Unlike a blit operation, a tile operation always starts in the upper left corner of the target bitmap, and fills it entirely
	//   As an optimization, one tile worth will be copied first, then copied to the right to fill one horizontal band of the dest. bitmap
	//   From that point, will loop through the required number of remaining bands, copying the full width of the dest bitmap.
	
	// adjust copy width/height if the prescribed tile height or width wouldn't fit on target bitmap
	width = (width >= dst_bm->width_) ? dst_bm->width_ : width;
	height = (height >= dst_bm->height_) ? dst_bm->height_ : height;

	// checks complete. ready to tile. 
	the_starting_read_loc = src_bm->addr_ + (src_bm->width_ * src_y) + src_x;
	
	h_tiles = dst_bm->width_ / width;
	h_rem = dst_bm->width_ % width;
	
	// each loop is one tile, writing left to right
	for (j = 0; j < h_tiles; j++)
	{
		the_read_loc = the_starting_read_loc;
		the_write_loc = dst_bm->addr_ + width * j; // row 0, and x pos of width * num times already tiled

		for (i = 0; i < height; i++)
		{
			memcpy(the_write_loc, the_read_loc, width);
		
			the_write_loc += dst_bm->width_;
			the_read_loc += src_bm->width_;
		}		
	}
	
	// the last tile, if any, will have same height, but less width
	if (h_rem)
	{
		the_read_loc = the_starting_read_loc;
		the_write_loc = dst_bm->addr_ + width * j; // row 0, and x pos of width * num times already tiled
		width = h_rem;

		for (i = 0; i < height; i++)
		{
			memcpy(the_write_loc, the_read_loc, width);
		
			the_write_loc += dst_bm->width_;
			the_read_loc += src_bm->width_;
		}		
	}
	
	// have now filled the entire width of the dest. bitmap, to a height of 'height'. tile downwards...
	
	v_tiles = dst_bm->height_ / height;
	v_rem = dst_bm->height_ % height;

	//DEBUG_OUT(("%s %d: h_tiles=%i, h_rem=%i, v_tiles=%i, v_rem=%i", __func__, __LINE__, h_tiles, h_rem, v_tiles, v_rem));
	
	// each loop is one tile, writing top to bottom, from 2nd vertical band
	for (j = 1; j < v_tiles; j++)	 // 1 because we just wrote one band worth's above.
	{
		the_read_loc = dst_bm->addr_; // now we are reading from the destination bitmap, from the area we prepared
		the_write_loc = dst_bm->addr_ + (dst_bm->width_ * height * j);
		
		for (i = 0; i < height; i++)
		{
			memcpy(the_write_loc, the_read_loc, dst_bm->width_);
		
			the_write_loc += dst_bm->width_;
			the_read_loc += dst_bm->width_;
		}		
	}
	
	// the last tile, if any, will have less height
	if (v_rem)
	{
		the_read_loc = dst_bm->addr_; // now we are reading from the destination bitmap, from the area we prepared
		the_write_loc = dst_bm->addr_ + (dst_bm->width_ * height * j);
		height = v_rem;

		for (i = 0; i < height; i++)
		{
			memcpy(the_write_loc, the_read_loc, dst_bm->width_);
		
			the_write_loc += dst_bm->width_;
			the_read_loc += dst_bm->width_;
		}		
	}
	
	//DEBUG_OUT(("%s %d: final parameters: src_x=%i, src_y=%i, width=%i, height=%i.", __func__, __LINE__, src_x, src_y, width, height));
	
	return true;
}



// **** Block fill functions ****


// Fill graphics memory with specified value
//! @param	the_color -- a 1-byte index to the current LUT
//! @return	returns false on any error/invalid input.
bool Bitmap_FillMemory(Bitmap* the_bitmap, uint8_t the_color)
{
	uint32_t	the_write_loc_int;
	uint8_t*	the_write_loc;
	uint32_t	the_write_len;
	
	if (the_bitmap == NULL)
	{
		LOG_ERR(("%s %d: passed bitmap was NULL", __func__, __LINE__));
		return false;
	}

	the_write_loc_int = Bitmap_GetMemLocIntForXY(the_bitmap, 0, 0);
	the_write_loc = (uint8_t*)the_write_loc_int;
	the_write_len = (uint32_t)the_bitmap->width_ * (uint32_t)the_bitmap->height_;
		
	#ifdef _DO_NOT_TRUST_MEMCPY_
		uint32_t i;
		for (i=0; i < the_write_len; i++)
		{
			*the_write_loc = the_color;
			the_write_loc_int++;			
			the_write_loc = (uint8_t*)the_write_loc_int;
		}
	#else
		memset(the_write_loc, the_color, the_write_len);
	#endif

	return true;
}


//! Fill pixel values for the passed Rectangle object, using the specified LUT value
//! @param	the_color -- a 1-byte index to the current LUT
//! @return	returns false on any error/invalid input.
bool Bitmap_FillBoxRect(Bitmap* the_bitmap, Rectangle* the_coords, uint8_t the_color)
{
	return Bitmap_FillBox(the_bitmap, the_coords->MinX, the_coords->MinY, the_coords->MaxX - the_coords->MinX, the_coords->MaxY - the_coords->MinY, the_color);
}


//! Fill pixel values for a specific box area
//! calling function must validate screen id, coords!
//! @param	width -- width, in pixels, of the rectangle to be filled
//! @param	height -- height, in pixels, of the rectangle to be filled
//! @param	the_color -- a 1-byte index to the current LUT
//! @return	returns false on any error/invalid input.
bool Bitmap_FillBox(Bitmap* the_bitmap, int16_t x, int16_t y, int16_t width, int16_t height, uint8_t the_color)
{
	uint32_t	the_write_loc_int;
	uint8_t*	the_write_loc;
	uint32_t	fat_bmap_width;
	uint16_t	short_color = (uint16_t)the_color;
	int16_t		max_row;

	if (the_bitmap == NULL)
	{
		LOG_ERR(("%s %d: passed bitmap was NULL", __func__, __LINE__));
		return false;
	}

// 	DEBUG_OUT(("%s %d: x=%i, y=%i, width=%i, height=%i, the_color=%i, addr=%p, addr_int_=%lx", __func__, __LINE__, x, y, width, height, the_color, the_bitmap->addr_, the_bitmap->addr_int_));
	
	// set up initial loc
	the_write_loc_int = Bitmap_GetMemLocIntForXY(the_bitmap, x, y);
	
// 	DEBUG_OUT(("%s %d: the_write_loc_int=%lx, (char*)the_write_loc_int=%p", __func__, __LINE__, the_write_loc_int, (char*)the_write_loc_int));
	
	fat_bmap_width = (uint32_t)the_bitmap->width_;
// 	DEBUG_OUT(("%s %d: fat_bmap_width=%lu, the_bitmap->width_=%i", __func__, __LINE__, fat_bmap_width, the_bitmap->width_));
// 	DEBUG_OUT(("%s %d: width=%i, write_len=%lu (unsigned), write_len=%li", __func__, __LINE__, width, write_len, write_len));

	max_row = y + height;
	
	for (; y <= max_row; y++)
	{
		the_write_loc = (uint8_t*)the_write_loc_int;
		
		#ifdef _DO_NOT_TRUST_MEMCPY_
			uint16_t i;
			for (i=0; i < width; i++)
			{
				*(the_write_loc + i) = the_color;
			}
		#else
			memset(the_write_loc, short_color, (size_t)width);
		#endif
		
		the_write_loc_int += fat_bmap_width;
		//DEBUG_OUT(("%s %d: after write: the_write_loc_int=%lx, before: the_write_loc=%p, len=%lu", __func__, __LINE__, the_write_loc_int, the_write_loc, write_len));
	}
	
	return true;
}





// **** Bitmap functions *****

//! Set the font
//! This is the font that will be used for all font drawing in this bitmap
//! @param	the_bitmap -- reference to a valid Bitmap object.
//! @param	the_font -- reference to a complete, loaded Font object.
//! @return Returns false on any error condition
bool Bitmap_SetFont(Bitmap* the_bitmap, Font* the_font)
{
	if (the_bitmap == NULL)
	{
		LOG_ERR(("%s %d: passed bitmap was NULL", __func__, __LINE__));
		return false;
	}

	if (the_font == NULL)
	{
		LOG_ERR(("%s %d: passed font was NULL", __func__, __LINE__));
		return false;
	}

	the_bitmap->font_ = the_font;
	
	return true;
}


//! Set the "pen" color
//! This is the color that the next pen-based graphics function will use
//! This only affects functions that use the pen: any graphics function that specifies a color will use that instead
//! @param	the_bitmap -- reference to a valid Bitmap object.
//! @param	the_color -- a 1-byte index to the current LUT
//! @return Returns false on any error condition
bool Bitmap_SetColor(Bitmap* the_bitmap, uint8_t the_color)
{
	if (the_bitmap == NULL)
	{
		LOG_ERR(("%s %d: passed bitmap was NULL", __func__, __LINE__));
		return false;
	}

	the_bitmap->color_ = the_color;
	
	return true;
}


//! Set the "pen" position
//! This is the location that the next pen-based graphics function will use for a starting location
//! NOTE: you are allowed to set negative values, but not values greater than the height/width of the screen. This is to allow for functions that may have portions visible on the screen, such as a row of text that starts 2 pixels to the left of the bitmap's left edge. 
//! This only affects functions that use the pen: any graphics function that specifies an X, Y coordinate will use that instead
//! @param	the_bitmap -- reference to a valid Bitmap object.
//! @param	x -- the horizontal position, between 0 and bitmap width - 1
//! @param	y -- the vertical position, between 0 and bitmap height - 1
//! @return Returns false on any error condition
bool Bitmap_SetXY(Bitmap* the_bitmap, int16_t x, int16_t y)
{
	if (the_bitmap == NULL)
	{
		LOG_ERR(("%s %d: passed bitmap was NULL", __func__, __LINE__));
		return false;
	}

	if (x >= the_bitmap->width_ || y >= the_bitmap->height_)
	{
		LOG_ERR(("%s %d: invalid coordinates passed (%i, %i)", __func__, __LINE__, x, y));
		return false;
	}
	
	the_bitmap->x_ = x;
	the_bitmap->y_ = y;
	
	return true;
}


//! Get the current color of the pen
//! @param	the_bitmap -- reference to a valid Bitmap object.
//! @return Returns a 1-byte index to the current LUT, or 0 on any error
uint8_t Bitmap_GetColor(Bitmap* the_bitmap)
{
	if (the_bitmap == NULL)
	{
		LOG_ERR(("%s %d: passed bitmap was NULL", __func__, __LINE__));
		return 0;
	}

	return the_bitmap->color_;
}


//! Get the current X position of the pen
//! @param	the_bitmap -- reference to a valid Bitmap object.
//! @return Returns -1 on any error
int16_t Bitmap_GetX(Bitmap* the_bitmap)
{
	if (the_bitmap == NULL)
	{
		LOG_ERR(("%s %d: passed bitmap was NULL", __func__, __LINE__));
		return -1;
	}

	return the_bitmap->x_;
}

//! Get the current Y position of the pen
//! @param	the_bitmap -- reference to a valid Bitmap object.
//! @return Returns -1 on any error
int16_t Bitmap_GetY(Bitmap* the_bitmap)
{
	if (the_bitmap == NULL)
	{
		LOG_ERR(("%s %d: passed bitmap was NULL", __func__, __LINE__));
		return -1;
	}

	return the_bitmap->y_;
}


//! Get the current font of the pen
//! @param	the_bitmap -- reference to a valid Bitmap object.
//! @return Returns NULL on any error
Font* Bitmap_GetFont(Bitmap* the_bitmap)
{
	if (the_bitmap == NULL)
	{
		LOG_ERR(("%s %d: passed bitmap was NULL", __func__, __LINE__));
		return NULL;
	}

	return the_bitmap->font_;
}



//! Calculate the VRAM location of the specified coordinate within the bitmap
//! @param	the_bitmap -- reference to a valid Bitmap object.
//! @param	x -- the horizontal position, between 0 and bitmap width - 1
//! @param	y -- the vertical position, between 0 and bitmap height - 1
//! @return Returns a pointer to the VRAM location that corresponds to the passed X, Y, or NULL on any error condition
unsigned char* Bitmap_GetMemLocForXY(Bitmap* the_bitmap, int16_t x, int16_t y)
{
	if (the_bitmap == NULL)
	{
		LOG_ERR(("%s %d: passed bitmap was NULL", __func__, __LINE__));
		return NULL;
	}
	
	if (the_bitmap->addr_ == NULL)
	{
		LOG_ERR(("%s %d: passed bitmap had a NULL address", __func__, __LINE__));
		return NULL;
	}
	
	// LOGIC:
	//   check that x and y are within the bitmap's coordinate box. if not, we can't calculate a memory loc for them.
	
	if (0 > x >= the_bitmap->width_ || 0 > y >= the_bitmap->height_)
	{
		LOG_ERR(("%s %d: invalid coordinates passed (%i, %i)", __func__, __LINE__, x, y));
		return NULL;
	}
	
	return the_bitmap->addr_ + (the_bitmap->width_ * y) + x;
}


//! Calculate the VRAM location of the specified coordinate within the bitmap
//! @param	the_bitmap -- reference to a valid Bitmap object.
//! @param	x -- the horizontal position, between 0 and bitmap width - 1
//! @param	y -- the vertical position, between 0 and bitmap height - 1
//! @return Returns an unsigned long that can be converted to the VRAM location that corresponds to the passed X, Y, or NULL on any error condition
uint32_t Bitmap_GetMemLocIntForXY(Bitmap* the_bitmap, int16_t x, int16_t y)
{
	if (the_bitmap == NULL)
	{
		LOG_ERR(("%s %d: passed bitmap was NULL", __func__, __LINE__));
		return 0;
	}
	
	if (the_bitmap->addr_ == NULL)
	{
		LOG_ERR(("%s %d: passed bitmap had a NULL address", __func__, __LINE__));
		return 0;
	}
	
	// LOGIC:
	//   check that x and y are within the bitmap's coordinate box. if not, we can't calculate a memory loc for them.
	
	if (0 > x >= the_bitmap->width_ || 0 > y >= the_bitmap->height_)
	{
		LOG_ERR(("%s %d: invalid coordinates passed (%i, %i)", __func__, __LINE__, x, y));
		return 0;
	}

	return the_bitmap->addr_int_ + ((uint32_t)the_bitmap->width_ * (uint32_t)y) + (uint32_t)x;
}


//! Calculate the VRAM location of the current coordinate within the bitmap
//! @param	the_bitmap -- reference to a valid Bitmap object.
//! @return Returns a pointer to the VRAM location that corresponds to the current "pen" X, Y, or NULL on any error condition
unsigned char* Bitmap_GetMemLoc(Bitmap* the_bitmap)
{
	uint32_t	as_int;
	
	as_int = Bitmap_GetMemLocIntForXY(the_bitmap, the_bitmap->x_, the_bitmap->y_);
	return (unsigned char*)as_int;
}


//! Calculate the VRAM location of the current coordinate within the bitmap
//! @param	the_bitmap -- reference to a valid Bitmap object.
//! @return Returns an unsigned long that can be converted to the VRAM location that corresponds to the current "pen" X, Y, or NULL on any error condition
uint32_t Bitmap_GetMemLocInt(Bitmap* the_bitmap)
{
	return Bitmap_GetMemLocIntForXY(the_bitmap, the_bitmap->x_, the_bitmap->y_);
}






// **** Set pixel functions *****


//! Set a char at a specified x, y coord
//! @param	the_color -- a 1-byte index to the current LUT
//! @return	returns false on any error/invalid input.
bool Bitmap_SetPixelAtXY(Bitmap* the_bitmap, int16_t x, int16_t y, uint8_t the_color)
{
	uint32_t	the_write_loc_int;
	char*		the_write_loc;
	
	if (the_bitmap == NULL)
	{
		LOG_ERR(("%s %d: passed bitmap was NULL", __func__, __LINE__));
		return false;
	}

	if (!Bitmap_ValidateXY(the_bitmap, x, y))
	{
		LOG_INFO(("%s %d: illegal coordinate %i, %i", __func__, __LINE__, x, y));
		return false;
	}
	
	the_write_loc_int = Bitmap_GetMemLocIntForXY(the_bitmap, x, y);	
	the_write_loc = (char*)the_write_loc_int;
 	*the_write_loc = the_color;
	
	//DEBUG_OUT(("%s %d: x=%i, y=%i, the_write_loc=%p, the_write_loc_int=%lx", __func__, __LINE__, x, y, the_write_loc, the_write_loc_int));
	
	return true;
}




// **** Get pixel functions *****


//! Get the pixel CLUT index at a specified x, y coord
//! @return	returns an 8-bit CLUT index
uint8_t Bitmap_GetPixelAtXY(Bitmap* the_bitmap, int16_t x, int16_t y)
{
	uint32_t	the_read_loc_int;
	uint8_t*	the_read_loc;
	uint8_t		the_color;
	
	if (the_bitmap == NULL)
	{
		LOG_ERR(("%s %d: passed bitmap was NULL", __func__, __LINE__));
		return false;
	}

	if (!Bitmap_ValidateXY(the_bitmap, x, y))
	{
		LOG_ERR(("%s %d: illegal coordinate", __func__, __LINE__));
		return false;
	}
	
	the_read_loc_int = Bitmap_GetMemLocIntForXY(the_bitmap, x, y);	
	the_read_loc = (uint8_t*)the_read_loc_int;
 	the_color = (uint8_t)*the_read_loc;
	
	return the_color;
}





// **** Drawing functions *****


//! Draws a line between 2 passed coordinates.
//! Use for any line that is not perfectly vertical or perfectly horizontal
//! Based on http://rosettacode.org/wiki/Bitmap/Bresenham%27s_line_algorithm#C. Used in C128 Lich King. 
bool Bitmap_DrawLine(Bitmap* the_bitmap, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t the_color)
{
	int16_t dx;
	int16_t sx;
	int16_t dy;
	int16_t sy;
	int16_t err;

	if (the_bitmap == NULL)
	{
		LOG_ERR(("%s %d: passed bitmap was NULL", __func__, __LINE__));
		return false;
	}

	if (!Bitmap_ValidateXY(the_bitmap, x1, y1))
	{
		LOG_ERR(("%s %d: illegal coordinate", __func__, __LINE__));
		return false;
	}
	
	dx = abs(x2 - x1);
	sx = x1 < x2 ? 1 : -1;
	dy = abs(y2 - y1);
	sy = y1 < y2 ? 1 : -1;
	err = (dx > dy ? dx : -dy)/2;

	for(;;)
	{
		int16_t e2;

		Bitmap_SetPixelAtXY(the_bitmap, x1, y1, the_color);

		if (x1==x2 && y1==y2)
		{
			break;
		}

		e2 = err;

		if (e2 >-dx)
		{
			err -= dy;
			x1 += sx;
		}

		if (e2 < dy)
		{
			err += dx;
			y1 += sy;
		}
	}
	
	return true;
}

//! Draws a horizontal line from specified coords, for n pixels, using the specified pixel value
//! @param	the_color -- a 1-byte index to the current LUT
//! @return	returns false on any error/invalid input.
bool Bitmap_DrawHLine(Bitmap* the_bitmap, int16_t x, int16_t y, int16_t the_line_len, uint8_t the_color)
{
	bool			result;
	
	// LOGIC: 
	//   an H line is just a box with 1 row, so we can re-use Bitmap_FillBox(). These routines use memset, so are quicker than for loops. 

	//DEBUG_OUT(("%s %d: x=%i, y=%i, the_line_len=%i, the_color=%i", __func__, __LINE__, x, y, the_line_len, the_color));
	
	if (the_bitmap == NULL)
	{
		LOG_ERR(("%s %d: passed bitmap was NULL", __func__, __LINE__));
		return false;
	}

	if (!Bitmap_ValidateXY(the_bitmap, x, y))
	{
		LOG_ERR(("%s %d: illegal coordinate", __func__, __LINE__));
		return false;
	}

	result = Bitmap_FillBox(the_bitmap, x, y, the_line_len, 0, the_color);

	return result;
}


//! Draws a vertical line from specified coords, for n pixels
//! @param	the_color -- a 1-byte index to the current LUT
//! @return	returns false on any error/invalid input.
bool Bitmap_DrawVLine(Bitmap* the_bitmap, int16_t x, int16_t y, int16_t the_line_len, uint8_t the_color)
{
	int16_t		dy;

	//DEBUG_OUT(("%s %d: x=%i, y=%i, the_line_len=%i, the_color=%i", __func__, __LINE__, x, y, the_line_len, the_color));
	
	if (the_bitmap == NULL)
	{
		LOG_ERR(("%s %d: passed bitmap was NULL", __func__, __LINE__));
		return false;
	}

	if (!Bitmap_ValidateXY(the_bitmap, x, y))
	{
		LOG_ERR(("%s %d: illegal coordinate", __func__, __LINE__));
		return false;
	}
	
	for (dy = 0; dy < the_line_len; dy++)
	{
		Bitmap_SetPixelAtXY(the_bitmap, x, y + dy, the_color);
	}
	
	return true;
}


//! Draws a rectangle based on the passed Rectangle object, using the specified LUT value
//! @param	the_color -- a 1-byte index to the current LUT
//! @return	returns false on any error/invalid input.
bool Bitmap_DrawBoxRect(Bitmap* the_bitmap, Rectangle* the_coords, uint8_t the_color)
{
	return Bitmap_DrawBoxCoords(the_bitmap, the_coords->MinX, the_coords->MinY, the_coords->MaxX, the_coords->MaxY, the_color);
}


//! Draws a rectangle based on 2 sets of coords, using the specified LUT value
//! @param	the_color -- a 1-byte index to the current LUT
//! @return	returns false on any error/invalid input.
bool Bitmap_DrawBoxCoords(Bitmap* the_bitmap, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t the_color)
{
	int16_t		dy;
	int16_t		dx;

	//DEBUG_OUT(("%s %d: x1=%i, y1=%i, x2=%i, y2=%i, the_color=%i", __func__, __LINE__, x1, y1, x2, y2, the_color));
	
	if (the_bitmap == NULL)
	{
		LOG_ERR(("%s %d: passed bitmap was NULL", __func__, __LINE__));
		return false;
	}

	if (!Bitmap_ValidateXY(the_bitmap, x1, y1))
	{
		LOG_ERR(("%s %d: illegal coordinate", __func__, __LINE__));
		return false;
	}
	
	if (!Bitmap_ValidateXY(the_bitmap, x2, y2))
	{
		LOG_ERR(("%s %d: illegal coordinate %i, %i", __func__, __LINE__, x2, y2));
		return false;
	}

	if (x1 > x2 || y1 > y2)
	{
		LOG_ERR(("%s %d: illegal coordinates %i to %i, %i to %i", __func__, __LINE__, x1, x2, y1, y2));
		return false;
	}

	// add 1 to line len, because desired behavior is a box that connects fully to the passed coords
	dx = x2 - x1 + 0;
	dy = y2 - y1 + 0;
	
	if (!Bitmap_DrawHLine(the_bitmap, x1, y1, dx, the_color))
	{
		LOG_ERR(("%s %d: draw box failed", __func__, __LINE__));
		return false;
	}
	
	if (!Bitmap_DrawVLine(the_bitmap, x2, y1, dy, the_color))
	{
		LOG_ERR(("%s %d: draw box failed", __func__, __LINE__));
		return false;
	}
	
	if (!Bitmap_DrawHLine(the_bitmap, x1, y2, dx, the_color))
	{
		LOG_ERR(("%s %d: draw box failed", __func__, __LINE__));
		return false;
	}
	
	if (!Bitmap_DrawVLine(the_bitmap, x1, y1, dy, the_color))
	{
		LOG_ERR(("%s %d: draw box failed", __func__, __LINE__));
		return false;
	}
		
	return true;
}


//! Draws a rectangle based on start coords and width/height, and optionally fills the rectangle.
//! @param	width -- width, in pixels, of the rectangle to be drawn
//! @param	height -- height, in pixels, of the rectangle to be drawn
//! @param	the_color -- a 1-byte index to the current LUT
//! @param	do_fill -- If true, the box will be filled with the provided color. If false, the box will only draw the outline.
//! @return	returns false on any error/invalid input.
bool Bitmap_DrawBox(Bitmap* the_bitmap, int16_t x, int16_t y, int16_t width, int16_t height, uint8_t the_color, bool do_fill)
{	

	//DEBUG_OUT(("%s %d: x=%i, y=%i, width=%i, height=%i, the_color=%i", __func__, __LINE__, x, y, width, height, the_color));

	if (the_bitmap == NULL)
	{
		LOG_ERR(("%s %d: passed bitmap was NULL", __func__, __LINE__));
		return false;
	}

	if (!Bitmap_ValidateXY(the_bitmap, x, y))
	{
		LOG_ERR(("%s %d: illegal coordinate (%i, %i)", __func__, __LINE__, x, y));
		return false;
	}
	
	if (!Bitmap_ValidateXY(the_bitmap, x + width - 1, y + height - 1))
	{
		LOG_ERR(("%s %d: illegal coordinates. x2=%i, y2=%i", __func__, __LINE__,  x + width - 1, y + height - 1));
		return false;
	}

	// LOGIC:
	//   if fill is needed, it's faster to simply do one rect fill than draw the lines
	//   if fill is not needed, we need 4 line draw calls
	
	if (do_fill)
	{
		if (!Bitmap_FillBox(the_bitmap, x, y, width, height - 1, the_color))
		{
			LOG_ERR(("%s %d: draw filled box failed", __func__, __LINE__));
			return false;
		}
	}
	else
	{
		if (!Bitmap_DrawHLine(the_bitmap, x, y, width, the_color))
		{
			LOG_ERR(("%s %d: draw box failed", __func__, __LINE__));
			return false;
		}
	
		if (!Bitmap_DrawVLine(the_bitmap, x + width - 1, y, height, the_color))
		{
			LOG_ERR(("%s %d: draw box failed", __func__, __LINE__));
			return false;
		}
	
		if (!Bitmap_DrawHLine(the_bitmap, x, y + height - 1, width, the_color))
		{
			LOG_ERR(("%s %d: draw box failed", __func__, __LINE__));
			return false;
		}
	
		if (!Bitmap_DrawVLine(the_bitmap, x, y, height, the_color))
		{
			LOG_ERR(("%s %d: draw box failed", __func__, __LINE__));
			return false;
		}
	}
		
	return true;
}


//! Draws a rounded rectangle with the specified size and radius, and optionally fills the rectangle.
//! @param	width -- width, in pixels, of the rectangle to be drawn
//! @param	height -- height, in pixels, of the rectangle to be drawn
//! @param	radius -- radius, in pixels, of the arc to be applied to the rectangle's corners. Enforces a minimum of 3, maximum of 20.
//! @param	the_color -- a 1-byte index to the current color LUT
//! @param	do_fill -- If true, the box will be filled with the provided color. If false, the box will only draw the outline.
//! @return	returns false on any error/invalid input.
bool Bitmap_DrawRoundBox(Bitmap* the_bitmap, int16_t x, int16_t y, int16_t width, int16_t height, int16_t radius, uint8_t the_color, bool do_fill)
{	

	//DEBUG_OUT(("%s %d: x=%i, y=%i, width=%i, height=%i, the_color=%i", __func__, __LINE__, x, y, width, height, the_color));

	if (the_bitmap == NULL)
	{
		LOG_ERR(("%s %d: passed bitmap was NULL", __func__, __LINE__));
		return false;
	}

	if (!Bitmap_ValidateXY(the_bitmap, x, y))
	{
		LOG_ERR(("%s %d: illegal coordinate", __func__, __LINE__));
		return false;
	}
	
	if (!Bitmap_ValidateXY(the_bitmap, x + width - 1, y + height - 1))
	{
		LOG_ERR(("%s %d: illegal coordinate", __func__, __LINE__));
		return false;
	}

	radius = (radius < 3) ? 3 : radius;
	radius = (radius > 20) ? 20 : radius;
	
	
	// adjust box x, y, width, height values to line up with the edges of the arcs
	width -= radius * 2;
	height -= radius * 2;
	x += radius;
	y += radius;
	
	// Draw 4 circle quadrants
	Bitmap_DrawCircleQuadrants(the_bitmap, x, y, radius, the_color, PARAM_SKIP_NE, PARAM_SKIP_SE, PARAM_SKIP_SW, PARAM_DRAW_NW);
	Bitmap_DrawCircleQuadrants(the_bitmap, x + width, y, radius, the_color, PARAM_DRAW_NE, PARAM_SKIP_SE, PARAM_SKIP_SW, PARAM_SKIP_NW);
	Bitmap_DrawCircleQuadrants(the_bitmap, x, y + height, radius, the_color, PARAM_SKIP_NE, PARAM_SKIP_SE, PARAM_DRAW_SW, PARAM_SKIP_NW);
	Bitmap_DrawCircleQuadrants(the_bitmap, x + width, y + height, radius, the_color, PARAM_SKIP_NE, PARAM_DRAW_SE, PARAM_SKIP_SW, PARAM_SKIP_NW);
	
	// draw 4 shortened lines that will match up with the edges of the arcs
	if (!Bitmap_DrawHLine(the_bitmap, x, y - radius, width, the_color))
	{
		LOG_ERR(("%s %d: draw box failed", __func__, __LINE__));
		return false;
	}
	
	if (!Bitmap_DrawVLine(the_bitmap, x + width + radius, y, height, the_color))
	{
		LOG_ERR(("%s %d: draw box failed", __func__, __LINE__));
		return false;
	}
	
	if (!Bitmap_DrawHLine(the_bitmap, x, y + height + radius, width, the_color))
	{
		LOG_ERR(("%s %d: draw box failed", __func__, __LINE__));
		return false;
	}
	
	if (!Bitmap_DrawVLine(the_bitmap, x - radius, y, height, the_color))
	{
		LOG_ERR(("%s %d: draw box failed", __func__, __LINE__));
		return false;
	}
	
	// fill with same color as outline, if specified
	if (do_fill)
	{
		Bitmap_FillBox(the_bitmap, x + radius, y + 1, width - radius*2, radius, the_color);
		Bitmap_FillBox(the_bitmap, x + 1, y + radius, width - 1, height-radius*2, the_color);
		Bitmap_FillBox(the_bitmap, x + radius, y + height-radius*1, width - radius*2, radius-1, the_color);
		Bitmap_Fill(the_bitmap, x + radius - 1, y + 1, the_color);
		Bitmap_Fill(the_bitmap, x + (width - radius) + 1, y + 1, the_color);
		Bitmap_Fill(the_bitmap, x + radius - 1, y + (height - radius) + 1, the_color);
		Bitmap_Fill(the_bitmap, x + (width - radius) + 1, y + (height - radius) + 1, the_color);
	}
		
	return true;
}


//! Draw a circle
//! Based on http://rosettacode.org/wiki/Bitmap/Midpoint_circle_algorithm#C
bool Bitmap_DrawCircle(Bitmap* the_bitmap, int16_t x1, int16_t y1, int16_t radius, uint8_t the_color)
{
	if (the_bitmap == NULL)
	{
		LOG_ERR(("%s %d: passed bitmap was NULL", __func__, __LINE__));
		return false;
	}

	if (!Bitmap_ValidateXY(the_bitmap, x1, y1))
	{
		LOG_ERR(("%s %d: illegal coordinate", __func__, __LINE__));
		return false;
	}

	return Bitmap_DrawCircleQuadrants(the_bitmap, x1, y1, radius, the_color, PARAM_DRAW_NE, PARAM_DRAW_SE, PARAM_DRAW_SW, PARAM_DRAW_NW);
}


// **** Draw string functions *****



