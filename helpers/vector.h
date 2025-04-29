#ifndef VECTOR_H
#define VECTOR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
    uint64_t *data;  // Pointer to array
    size_t size;     // Current number of elements
    size_t capacity; // Allocated capacity
} rvisor_vec;

// Initialize vector
void rvisor_vec_init(rvisor_vec *vec, size_t initial_capacity);

// Add element to vector
void rvisor_vec_push_back(rvisor_vec *vec, uint64_t value);

// Get element at index
static inline uint64_t rvisor_vec_get(rvisor_vec *vec, size_t index);
// Free vector memory
void rvisor_vec_free(rvisor_vec *vec);


#endif