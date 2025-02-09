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

void mergeSubArr(Ushort arr[], Ushort l, Ushort r, Ushort m) {
	Ushort i, j, k;
	Ushort leftSize = m - l + 1, rightSize = r - m;
	Ushort leftArr[leftSize], rightArr[rightSize];

	for (i = 0; i < leftSize; ++i) leftArr[i] = arr[l + i];
	for (j = 0; j < rightSize; ++j) rightArr[j] = arr[m + j + 1];

	i = 0;
	j = 0;
	k = l;

	while (i < leftSize && j < rightSize) {
		if (leftArr[i] <= rightArr[j]) arr[k++] = leftArr[i++];
		else arr[k++] = rightArr[j++];
	}

	while (i < leftSize)
		arr[k++] = leftArr[i++];

	while (j < rightSize)
		arr[k++] = rightArr[j++];
}
