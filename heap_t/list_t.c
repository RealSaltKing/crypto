/* list_t.c */

#include "list_t.h"

/* Create a new list */
list_t list_new() {
    list_t new_list = (list_t)malloc(2 * sizeof(void *));
    if (new_list == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    new_list[0] = NULL;  /* Data element (initially NULL) */
    new_list[1] = NULL;  /* Next pointer (initially NULL - end of list) */
    return new_list;
}

/* Free the memory used by the list */
void list_free(list_t l) {
    if (l == NULL) {
        return;
    }
    
    /* Free the rest of the list first */
    if (l[1] != NULL) {
        list_free((list_t)l[1]);
    }
    
    /* Then free this node */
    free(l);
}

/* Print the list */
void list_print(list_t l) {
    printf("[");
    
    list_t current = l;
    int first = TRUE;
    
    while (current != NULL && current[0] != NULL) {
        if (!first) {
            printf(", ");
        }
        printf("%lu", (uint64_t)current[0]);
        first = FALSE;
        current = (list_t)current[1];
    }
    
    printf("]\n");
}

/* Append an element to the list */
void list_append(list_t l, void *x) {
    /* Find the last node */
    list_t current = l;
    
    /* Special case: empty list */
    if (current[0] == NULL && current[1] == NULL) {
        current[0] = x;
        return;
    }
    
    /* Navigate to the last node */
    while (current[1] != NULL) {
        current = (list_t)current[1];
    }
    
    /* Create a new node and link it */
    list_t new_node = list_new();
    new_node[0] = x;
    current[1] = new_node;
}

/* Extend the first list with the second list */
void list_extend(list_t l1, list_t l2) {
    list_t current_l2 = l2;
    
    /* Iterate through l2 and append each element to l1 */
    while (current_l2 != NULL && current_l2[0] != NULL) {
        list_append(l1, current_l2[0]);
        current_l2 = (list_t)current_l2[1];
    }
}

/* Insert an element at a specific index in the list */
void list_insert(list_t l, size_t i, void *x) {
    list_t current = l;
    list_t prev = NULL;
    size_t curr_index = 0;
    
    /* Special case: insert at beginning of empty list */
    if (i == 0 && current[0] == NULL && current[1] == NULL) {
        current[0] = x;
        return;
    }
    
    /* Special case: insert at beginning of non-empty list */
    if (i == 0) {
        /* Create a new node with the same data as the head */
        list_t new_node = list_new();
        new_node[0] = current[0];
        new_node[1] = current[1];
        
        /* Update the head node */
        current[0] = x;
        current[1] = new_node;
        return;
    }
    
    /* Navigate to the insertion point */
    while (current != NULL && curr_index < i) {
        prev = current;
        current = (list_t)current[1];
        curr_index++;
        
        /* If we've reached the end of the list */
        if (current == NULL) {
            break;
        }
    }
    
    /* If index is out of range (beyond end of list), append */
    if (curr_index < i) {
        list_t last = l;
        while (last[1] != NULL) {
            last = (list_t)last[1];
        }
        
        list_t new_node = list_new();
        new_node[0] = x;
        last[1] = new_node;
        return;
    }
    
    /* Insert in the middle of the list */
    list_t new_node = list_new();
    new_node[0] = x;
    new_node[1] = current;
    prev[1] = new_node;
}

/* Remove the first occurrence of an element from the list */
bool list_remove(list_t l, void *x) {
    list_t current = l;
    list_t prev = NULL;
    
    /* Find the node with the value x */
    while (current != NULL && current[0] != x) {
        prev = current;
        current = (list_t)current[1];
    }
    
    /* If x was not found */
    if (current == NULL) {
        return FALSE;
    }
    
    /* If x is in the first node */
    if (prev == NULL) {
        /* Special case: only one element in the list */
        if (current[1] == NULL) {
            current[0] = NULL;  /* Empty the list */
            return TRUE;
        }
        
        /* Move the second element to the first position */
        list_t next_node = (list_t)current[1];
        current[0] = next_node[0];
        current[1] = next_node[1];
        
        /* Free the second node */
        next_node[0] = NULL;
        next_node[1] = NULL;
        free(next_node);
    } else {
        /* Remove the node from the middle or end of the list */
        prev[1] = current[1];
        free(current);
    }
    
    return TRUE;
}

/* Remove and return the element at a specific index */
void *list_pop(list_t l, size_t i) {
    list_t current = l;
    list_t prev = NULL;
    size_t curr_index = 0;
    void *result;
    
    /* Navigate to the node at index i */
    while (current != NULL && curr_index < i) {
        prev = current;
        current = (list_t)current[1];
        curr_index++;
    }
    
    /* If index is out of range */
    if (current == NULL) {
        fprintf(stderr, "Index out of range\n");
        exit(1);
    }
    
    /* Get the value to return */
    result = current[0];
    
    /* If popping the first element */
    if (prev == NULL) {
        /* Special case: only one element in the list */
        if (current[1] == NULL) {
            current[0] = NULL;  /* Empty the list */
            return result;
        }
        
        /* Move the second element to the first position */
        list_t next_node = (list_t)current[1];
        current[0] = next_node[0];
        current[1] = next_node[1];
        
        /* Free the second node */
        next_node[0] = NULL;
        next_node[1] = NULL;
        free(next_node);
    } else {
        /* Remove the node from the middle or end of the list */
        prev[1] = current[1];
        free(current);
    }
    
    return result;
}

/* Clear the list */
void list_clear(list_t l) {
    /* Free all nodes except the head */
    if (l[1] != NULL) {
        list_free((list_t)l[1]);
        l[1] = NULL;
    }
    
    /* Clear the head node */
    l[0] = NULL;
}

/* Get the index of the first occurrence of an element */
size_t list_index(list_t l, void *x) {
    list_t current = l;
    size_t index = 0;
    
    /* Find the first occurrence of x */
    while (current != NULL && current[0] != x) {
        current = (list_t)current[1];
        index++;
        
        /* If we've reached the end of the list */
        if (current == NULL) {
            fprintf(stderr, "Value not found in list\n");
            exit(1);
        }
    }
    
    return index;
}

/* Count the occurrences of an element in the list */
uint64_t list_count(list_t l, void *x) {
    list_t current = l;
    uint64_t count = 0;
    
    /* Count occurrences of x */
    while (current != NULL) {
        if (current[0] == x) {
            count++;
        }
        current = (list_t)current[1];
    }
    
    return count;
}

/* Reverse the list */
void list_reverse(list_t l) {
    /* Empty or single-element list */
    if (l == NULL || l[0] == NULL || l[1] == NULL) {
        return;
    }
    
    /* Copy all elements to an array */
    size_t length = 0;
    list_t current = l;
    while (current != NULL && current[0] != NULL) {
        length++;
        current = (list_t)current[1];
    }
    
    void **elements = (void **)malloc(length * sizeof(void *));
    if (elements == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    
    current = l;
    for (size_t i = 0; i < length; i++) {
        elements[i] = current[0];
        current = (list_t)current[1];
    }
    
    /* Clear the list */
    list_clear(l);
    
    /* Rebuild the list in reverse order */
    for (size_t i = 0; i < length; i++) {
        list_append(l, elements[length - 1 - i]);
    }
    
    /* Free the temporary array */
    free(elements);
}

/* Create a copy of the list */
list_t list_copy(list_t l) {
    list_t new_list = list_new();
    list_t current = l;
    
    /* Copy each element */
    while (current != NULL && current[0] != NULL) {
        list_append(new_list, current[0]);
        current = (list_t)current[1];
    }
    
    return new_list;
}
