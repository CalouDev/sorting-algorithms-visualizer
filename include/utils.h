#ifndef UTILS
#define UTILS

#include <SDL3/SDL.h>

typedef unsigned short Ushort;

void swap(Ushort *a, Ushort *b);
void shuffle(Ushort arr[], Ushort size);
Ushort min(Ushort a, Ushort b);
void mergeSubArr(short arr[], Ushort l, Ushort r, Ushort m);

#endif
