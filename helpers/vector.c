#include "vector.h"

void rvisor_vec_init(rvisor_vec *vec, size_t initial_capacity) {
    vec->size = 0;
    vec->capacity = initial_capacity;
    vec->data = (uint64_t *)malloc(initial_capacity * sizeof(uint64_t));
}

// Add element to vector
void rvisor_vec_push_back(rvisor_vec *vec, uint64_t value) {
    if (vec->size == vec->capacity) { // Resize if full
        vec->capacity *= 2;
        vec->data = (uint64_t *)realloc(vec->data, vec->capacity * sizeof(uint64_t));
    }
    vec->data[vec->size++] = value;
}

// Get element at index
static inline uint64_t rvisor_vec_get(rvisor_vec *vec, size_t index) {
    if (index >= vec->size) {
        fprintf(stderr, "Index out of bounds\n");
        exit(1);
    }
    return vec->data[index];
}

// Free vector memory
void rvisor_vec_free(rvisor_vec *vec) {
    free(vec->data);
}
