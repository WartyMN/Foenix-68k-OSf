//! @file gfx_memory.h

/*
 * gfx_memory.h
 *
 *  Created on: Mar 21, 2022
 *      Author: micahbly
 *
 *  Updated for OS/f 2026 and renamed (was: memory_manager)
 *
 */

#ifndef GFX_MEMORY_H_
#define GFX_MEMORY_H_


/* about this class: Memory Manager
 *
 * Provides wrapper functions for the BGET memory manager
 * All of the WORK of this code IS the BGET code.
 * I made 2 very minor hacks:
 *  I removed the select best fit and auto memory manager functions to make it easier for (me) to read the code.
 *  I made it so that the "freelist" global property is not global, and is passed to the functions by my wrappers. This lets me have it manage separate pools for SDRAM and other RAM, if I want it (currently removed)
 *
 *** things this class needs to be able to do
 * Allocate and free memory in SDRAM space
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
#include <stdio.h>

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

typedef struct bfhead MemoryPool;



/*****************************************************************************/
/*                             Global Variables                              */
/*****************************************************************************/


/*****************************************************************************/
/*                       Public Function Prototypes                          */
/*****************************************************************************/



//! Initialize the SDRAM memory pool and allocation system
//! @return Returns true on success
bool GfxMem_Init(void);



// **** BGET wrapper functions *****

//! Allocate and initialize num*size bytes of storage from the specified memory segment
//! Use this as you would a call to the standard C calloc() function
//! @param	num: number of objects
//! @param	size: size of each object
//! @return	On success, returns the pointer to the beginning of newly allocated memory. On failure, returns a null pointer.
void* GfxMem_Calloc(size_t num, size_t size);

//! Allocate size bytes of uninitialized storage from the specified memory segment
//! Use this as you would a call to the standard C malloc() function
//! @param	size: number of bytes to allocate
//! @return	On success, returns the pointer to the beginning of newly allocated memory. On failure, returns a null pointer.
void* GfxMem_Malloc(size_t size);

//! Deallocate the storage previously allocated by f_calloc() or f_malloc()
//! Use this as you would a call to the standard C free() function
//! @param	the_ptr: pointer to the memory to deallocate
void GfxMem_Free(void* the_ptr);



// **** xxx functions *****




// **** xxx functions *****


/*****************************************************************************/
/*                       BGET Function Prototypes                            */
/*****************************************************************************/

/*

    Interface definitions for bget.c, the memory management package.

*/

// NOTE: I moved all of the BGET function prototypes out of the header, and into gfx_memory.c, because I don't want them to be the interface





#endif /* GFX_MEMORY_H_ */


