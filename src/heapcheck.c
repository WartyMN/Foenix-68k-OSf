//! @file heapcheck.c
/*
 * heapcheck.c
 *
 * Debug-only validator for the Calypsi C library heap (68000 target).
 * Calypsi's malloc/free is the NuttX "mm" allocator (see Calypsi's
 * src/lib/stdlib/heap.h). This walks every chunk in physical order and every
 * free list, and reports the first inconsistency it finds.
 *
 * Call Heap_Check("label") from normal (non-interrupt) code, e.g. after each
 * demo step. The first label that reports a problem names the step that
 * damaged the heap.
 *
 * The struct layouts below mirror heap.h for a 68000 build:
 *   _CONFIG_MM_REGIONS = 1, 32-bit sizes, min chunk 16 bytes, max 4 MB.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "debug.h"

#define HC_ALLOC_BIT	0x80000000UL
#define HC_NNODES		(22 - 4 + 1)	// _MM_MAX_SHIFT - _MM_MIN_SHIFT + 1
#define HC_MAX_WALK		200000L			// stop runaway walks on a corrupted list

struct hc_alloc
{
	uint32_t			size;
	uint32_t			preceding;
};

struct hc_free
{
	uint32_t			size;
	uint32_t			preceding;
	struct hc_free*		flink;
	struct hc_free*		blink;
};

struct hc_heap
{
	size_t				heapsize;
	struct hc_alloc*	heapstart[1];
	struct hc_alloc*	heapend[1];
	struct hc_free		nodelist[HC_NNODES];
};

extern struct hc_heap __default_heap;


bool Heap_Check(const char* where)
{
	struct hc_heap*		h = &__default_heap;
	char*				lo = (char*)h->heapstart[0];
	char*				hi = (char*)h->heapend[0];
	char*				p;
	long				count;
	int16_t				i;

	if (lo == NULL || hi == NULL || hi <= lo)
	{
		DEBUG_OUT(("HEAPCHECK %s: heap bounds look wrong: start=%p end=%p", where, lo, hi));
		return false;
	}

	// 1. walk every chunk in address order: each chunk's size must lead exactly to the next chunk,
	//    and the next chunk's 'preceding' field must agree with that size
	p = lo;
	count = 0;

	while (p < hi)
	{
		struct hc_alloc*	node = (struct hc_alloc*)p;
		struct hc_alloc*	next;
		uint32_t			size = node->size;

		if (size < sizeof(struct hc_alloc) || (p + size) > hi || ++count > HC_MAX_WALK)
		{
			DEBUG_OUT(("HEAPCHECK %s: bad chunk size at %p: size=%lx preceding=%lx", where, p, (unsigned long)size, (unsigned long)node->preceding));
			return false;
		}

		next = (struct hc_alloc*)(p + size);

		if ((next->preceding & ~HC_ALLOC_BIT) != size)
		{
			DEBUG_OUT(("HEAPCHECK %s: chunk at %p (size %lx) disagrees with next chunk at %p (preceding=%lx)", where, p, (unsigned long)size, next, (unsigned long)next->preceding));
			return false;
		}

		p = (char*)next;
	}

	// 2. walk every free list: each node must be inside the heap, marked free, and properly back-linked
	for (i = 0; i < HC_NNODES; i++)
	{
		struct hc_free*		prev = &h->nodelist[i];
		struct hc_free*		node = prev->flink;

		count = 0;

		// NuttX links all the list heads into one chain; a node with size 0 is the next list's head, i.e. the end of this list
		while (node != NULL && node->size != 0)
		{
			if ((char*)node < lo || (char*)node >= hi || ++count > HC_MAX_WALK)
			{
				DEBUG_OUT(("HEAPCHECK %s: free list %i: node %p (from %p) is outside the heap", where, i, node, prev));
				return false;
			}

			if (node->preceding & HC_ALLOC_BIT)
			{
				DEBUG_OUT(("HEAPCHECK %s: free list %i: node %p is marked allocated", where, i, node));
				return false;
			}

			if (node->blink != prev)
			{
				DEBUG_OUT(("HEAPCHECK %s: free list %i: node %p blink=%p, expected %p", where, i, node, node->blink, prev));
				return false;
			}

			prev = node;
			node = node->flink;
		}
	}

	return true;
}
