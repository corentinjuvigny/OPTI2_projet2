/*

Copyright (c) 2019-2020, Yvan AUGÉ, Corentin JUVIGNY

Permission to use, copy, modify, and/or distribute this software
for any purpose with or without fee is hereby granted, provided
that the above copyright notice and this permission notice appear
in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR
CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

*/

/**
 * @file tools.h
 *
 * @brief Functions to allocate memory.
 *
 * The xmalloc function is similar to malloc but prints a fatal
 * message using the tool_spf_printf function when the memory
 * allocation failed.
 *
 * The xcalloc function is similar to tool_malloc but fills
 * up all the bytes of the memory block with low value ('\0').
 *
 * The xfree macro frees the memory location pointed to
 * by the p pointer if the p pointer is not NULL. Otherwise
 * it does nothing.
 */

#ifndef __TOOLS__
#define __TOOLS__

#ifdef __cplusplus
extern "C" {
#endif


#include "include.h"

/**
 * Allocate memory according to the size of an object.
 *
 * @param n 	The size to allocate.
 *
 * @return 		A void pointer.
 */
extern void* xmalloc(size_t n);

/**
 * Allocate memory according to the size and numeber of objects.
 *
 * @param nmemb 	The number of objects to allocate.
 * @param n 		The size to allocate.
 *
 * @return 			A void pointer.
 */
extern void* xcalloc(size_t nmemb,size_t size);

#define xfree(p) do { if ( (p) != NULL ) free(p); } while(0)

#ifdef __cplusplus
}
#endif

#endif
