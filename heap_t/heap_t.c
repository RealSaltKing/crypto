#include "heap_t.h"

// Macros for calculating parent and child indices (1-indexed array)
#define PARENT(i) ((i) / 2)
#define LEFT(i) ((i) * 2)
#define RIGHT(i) ((i) * 2 + 1)

// Helper function to get the address of an element at a given index
static void *get_element(heap_t h, size_t index) {
    // Adjusted for 1-indexed array (index 0 is not used)
    return (char *)h.eles + (index * h.ele_size);
}

// Helper function to swap two elements in the heap
static void swap_elements(heap_t h, size_t i, size_t j) {
    void *temp = malloc(h.ele_size);
    if (!temp) {
        fprintf(stderr, "Failed to allocate memory for swap\n");
        exit(EXIT_FAILURE);
    }
    
    void *ei = get_element(h, i);
    void *ej = get_element(h, j);
    
    memcpy(temp, ei, h.ele_size);
    memcpy(ei, ej, h.ele_size);
    memcpy(ej, temp, h.ele_size);
    
    free(temp);
}

// Helper function to heapify downward from a given index
static void heapify_down(heap_t h, size_t index) {
    size_t largest = index;
    size_t left = LEFT(index);
    size_t right = RIGHT(index);
    
    // Check if left child is larger than the current largest
    if (left <= h.size && h.gt(get_element(h, left), get_element(h, largest))) {
        largest = left;
    }
    
    // Check if right child is larger than the current largest
    if (right <= h.size && h.gt(get_element(h, right), get_element(h, largest))) {
        largest = right;
    }
    
    // If largest is not the current index, swap and continue heapifying
    if (largest != index) {
        swap_elements(h, index, largest);
        heapify_down(h, largest);
    }
}

// Helper function to heapify upward from a given index
static void heapify_up(heap_t h, size_t index) {
    // If we're at the root or the parent is greater, stop
    if (index <= 1 || !h.gt(get_element(h, index), get_element(h, PARENT(index)))) {
        return;
    }
    
    // Swap with parent and continue heapifying upward
    swap_elements(h, index, PARENT(index));
    heapify_up(h, PARENT(index));
}

// Initialize a new heap
heap_t heap_new(size_t ele_size, bool (*gt)(void *, void *)) {
    heap_t h;
    h.ele_size = ele_size;
    h.gt = gt;
    h.size = 0;
    h.capacity = 10;  // Initial capacity
    
    // Allocate memory for elements (+1 for 1-indexed)
    h.eles = malloc((h.capacity + 1) * h.ele_size);
    if (!h.eles) {
        fprintf(stderr, "Failed to allocate memory for heap\n");
        exit(EXIT_FAILURE);
    }
    
    return h;
}

// Free the heap and its elements
void heap_free(heap_t h) {
    if (h.eles) {
        free(h.eles);
    }
}

// Insert an element into the heap
void heap_insert(heap_t h, void *ele) {
    // Check if we need to resize
    if (h.size >= h.capacity) {
        h.capacity *= 2;
        h.eles = realloc(h.eles, (h.capacity + 1) * h.ele_size);
        if (!h.eles) {
            fprintf(stderr, "Failed to reallocate memory for heap\n");
            exit(EXIT_FAILURE);
        }
    }
    
    // Insert the new element at the end of the heap
    h.size++;
    memcpy(get_element(h, h.size), ele, h.ele_size);
    
    // Restore the heap property by heapifying up
    heapify_up(h, h.size);
}

// Remove and return the maximum element from the heap
void *heap_maxpop(heap_t h) {
    if (h.size == 0) {
        return NULL;  // Heap is empty
    }
    
    // Get the maximum element (root)
    void *max_ele = malloc(h.ele_size);
    if (!max_ele) {
        fprintf(stderr, "Failed to allocate memory for maxpop\n");
        exit(EXIT_FAILURE);
    }
    memcpy(max_ele, get_element(h, 1), h.ele_size);
    
    // Replace the root with the last element
    memcpy(get_element(h, 1), get_element(h, h.size), h.ele_size);
    h.size--;
    
    // Restore the heap property by heapifying down
    if (h.size > 0) {
        heapify_down(h, 1);
    }
    
    return max_ele;
}

// Helper function for pointer comparison in the heap (treats pointers as integers)
static bool pointer_greater_than(void *a, void *b) {
    return *(void**)a > *(void**)b;
}

heap_t l_to_h(list_t l) {
    // Count elements in the list
    size_t count = 0;
    list_t current = l;
    while (current != NULL && current[0] != NULL) {
        count++;
        current = (list_t)current[1];
    }
    
    // Create a new heap with a comparison function for pointers
    heap_t h = heap_new(sizeof(void*), pointer_greater_than);
    
    // Insert all elements from the list into the heap
    current = l;
    while (current != NULL && current[0] != NULL) {
        void *element = current[0];
        heap_insert(h, &element);
        current = (list_t)current[1];
    }
    
    return h;
}

void h_sort(list_t l) {
    // Return if the list is empty or has only one element
    if (l == NULL || l[0] == NULL || l[1] == NULL) {
        return;
    }
    
    // Create a heap from the list
    heap_t h = l_to_h(l);
    
    // Clear the original list
    list_clear(l);
    
    // Extract elements from the heap in sorted order (largest to smallest)
    // and append them to the list
    void *element;
    while ((element = heap_maxpop(h)) != NULL) {
        // For a max heap, we get elements in descending order
        // So we prepend to the list to get ascending order
        list_insert(l, 0, *(void**)element);
        free(element);  // Free the memory allocated by heap_maxpop
    }
    
    // Free the heap structure
    heap_free(h);
}