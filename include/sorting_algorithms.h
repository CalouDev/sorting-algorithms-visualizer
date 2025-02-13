#ifndef SORTING_ALGORITHMS
#define SORTING_ALGORITHMS

typedef unsigned short Ushort;
typedef enum {SORTING_STOP, SORTING_CONTINUE} sortingState;

sortingState sortInsertion(short arr[], Ushort size, Ushort index);
sortingState sortSelection(short arr[], Ushort size, Ushort index);
sortingState sortBubble(short arr[], Ushort size, Ushort index);

#endif
