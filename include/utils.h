#ifndef UTILS
#define UTILS

#include "globals.h"

SDL_AppResult checkAllocation(const void* ptr);
void swap(short *a, short *b);
void shuffle(short arr[], uint16_t size);
void mergeSubArr(short arr[], uint16_t l, uint16_t r, uint16_t m);

#endif
