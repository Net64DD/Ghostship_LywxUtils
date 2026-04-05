#include "mod.h"
#include "array.h"

HM_API void Array_Init(GenericArray *arr, size_t initialCapacity, size_t element_size) {
    arr->data = malloc(initialCapacity * element_size);
    if (arr->data == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    arr->size = 0;
    arr->capacity = initialCapacity;
    arr->element_size = element_size;
}

HM_API void Array_Push(GenericArray *arr, void *element) {
    if (arr->size == arr->capacity) {
        arr->capacity *= 2;
        arr->data = realloc(arr->data, arr->capacity * arr->element_size);

        if (arr->data == NULL) {
            fprintf(stderr, "Memory reallocation failed\n");
            exit(1);
        }
    }

    void *target_address = (char *)arr->data + (arr->size * arr->element_size);
    memcpy(target_address, element, arr->element_size);
    arr->size++;
}

HM_API void *Array_Get(GenericArray *arr, size_t index) {
    if (index >= arr->size) {
        return NULL;
    }
    return (char *)arr->data + (index * arr->element_size);
}

HM_API void Array_Set(GenericArray *arr, size_t index, void *element) {
    if (index >= arr->size) {
        fprintf(stderr, "Index out of bounds\n");
        return;
    }
    void *target_address = (char *)arr->data + (index * arr->element_size);
    memcpy(target_address, element, arr->element_size);
}

HM_API void Array_Remove(GenericArray *arr, size_t index) {
    if (index >= arr->size) {
        fprintf(stderr, "Index out of bounds\n");
        return;
    }
    if (index < arr->size - 1) {
        void *target_address = (char *)arr->data + (index * arr->element_size);
        void *next_element_address = (char *)arr->data + ((index + 1) * arr->element_size);
        size_t bytes_to_move = (arr->size - index - 1) * arr->element_size;
        memmove(target_address, next_element_address, bytes_to_move);
    }
    arr->size--;
}

HM_API void Array_Free(GenericArray *arr) {
    free(arr->data);
    arr->data = NULL;
    arr->size = 0;
    arr->capacity = 0;
    arr->element_size = 0;
}