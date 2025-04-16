#ifndef _HEAP_T_H_
#define _HEAP_T_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "list_t.h"  // Include list_t.h to recognize list_t type

// We'll use the bool from list_t.h, not redefine it here

typedef struct heap_struct {
  size_t ele_size;        // Size of each element
  bool (*gt)(void *, void *); // Greater than comparison function
  void *eles;             // Array to store elements
  size_t capacity;        // Current capacity of the heap
  size_t size;            // Current number of elements in the heap
} heap_t;

// Initialize a new heap
heap_t heap_new(size_t ele_size, bool (*gt)(void *, void *));

// Free the heap and its elements
void heap_free(heap_t h);

// Insert an element into the heap
void heap_insert(heap_t h, void *ele);

// Remove and return the maximum element from the heap
void *heap_maxpop(heap_t h);

// Challenge problems:
// Convert a list to a heap
heap_t l_to_h(list_t l);

// Sort a list using heap sort
void h_sort(list_t l);

#endif /* _HEAP_T_H_ */
