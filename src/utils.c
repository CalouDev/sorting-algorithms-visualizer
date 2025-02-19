#include "../include/utils.h"

void swap(Ushort *a, Ushort *b) {
	Ushort temp = *a;
	*a = *b;
	*b = temp;
}

void shuffle(Ushort arr[], Ushort size) {
    for (int i = size - 1; i > 0; i--) {
        int j = SDL_rand(size) % (i + 1);
        swap(&arr[i], &arr[j]);
    }
}

Ushort min(Ushort a, Ushort b) {
	return (a < b) ? a : b;
}

void mergeSubArr(short arr[], Ushort l, Ushort r, Ushort m) {
	Ushort i, j, k;
	Ushort leftSize = m - l + 1, rightSize = r - m;
	Ushort* leftArr = SDL_malloc(leftSize * sizeof(Ushort));
	Ushort* rightArr = SDL_malloc(rightSize * sizeof(Ushort));

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

	SDL_free(leftArr);
	SDL_free(rightArr);
}

