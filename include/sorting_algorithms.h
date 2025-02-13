#ifndef SORTING_ALGORITHMS
#define SORTING_ALGORITHMS

typedef unsigned short Ushort;

enum sortingState{SORTING_STOP, SORTING_CONTINUE};

int sortInsertion(short arr[], Ushort size, Ushort index);
int sortSelection(short arr[], Ushort size, Ushort index);
int sortBubble(short arr[], Ushort size, Ushort index);

#endif
