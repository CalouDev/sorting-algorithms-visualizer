#include "../include/utils.h"
#include <stdlib.h>

void swap(Ushort *a, Ushort *b) {
        Ushort temp = *a;
        *a = *b;
        *b = temp;
}

void shuffle(Ushort arr[], Ushort size) {
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        swap(&arr[i], &arr[j]);
    }
}
