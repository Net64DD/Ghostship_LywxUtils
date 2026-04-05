#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    void *data;
    size_t size;
    size_t capacity;
    size_t element_size;
} GenericArray;

extern void Array_Init(GenericArray *arr, size_t initialCapacity, size_t element_size);
extern void Array_Push(GenericArray *arr, void *element);
extern void *Array_Get(GenericArray *arr, size_t index);
extern void Array_Set(GenericArray *arr, size_t index, void *element);
extern void Array_Remove(GenericArray *arr, size_t index);
extern void Array_Free(GenericArray *arr);

#define Array_InitSymbol "Array_Init"
#define Array_PushSymbol "Array_Push"
#define Array_GetSymbol "Array_Get"
#define Array_SetSymbol "Array_Set"
#define Array_RemoveSymbol "Array_Remove"
#define Array_FreeSymbol "Array_Free"

typedef void (*Array_InitFunc)(GenericArray *arr, size_t initialCapacity, size_t element_size);
typedef void (*Array_PushFunc)(GenericArray *arr, void *element);
typedef void *(*Array_GetFunc)(GenericArray *arr, size_t index);
typedef void (*Array_SetFunc)(GenericArray *arr, size_t index, void *element);
typedef void (*Array_RemoveFunc)(GenericArray *arr, size_t index);
typedef void (*Array_FreeFunc)(GenericArray *arr);