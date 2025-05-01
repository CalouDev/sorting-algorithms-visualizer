#include "../include/utils.h"

SDL_AppResult checkAllocation(const void* ptr) {
	SDL_AppResult state;

	state = SDL_APP_CONTINUE;

	if (NULL == ptr) state = SDL_APP_FAILURE;

	return state;
}

void swap(short* a, short* b) {
	uint16_t temp;
	
	temp = *a;
	*a = *b;
	*b = temp;
}

void shuffle(short arr[], uint16_t size) {
	int i, randomIndex;

    for (i = size - 1; i > 0; i--) {
        randomIndex = SDL_rand(size) % (i + 1);
        swap(&arr[i], &arr[randomIndex]);
    }
}

void mergeSubArr(short arr[], uint16_t l, uint16_t r, uint16_t m) {
	uint16_t i, j, k, leftSize, rightSize;
	uint16_t *leftArr, *rightArr;

	leftSize = m - l + 1;
	rightSize = r - m;
	leftArr = SDL_malloc(leftSize * sizeof(uint16_t));
	rightArr = SDL_malloc(rightSize * sizeof(uint16_t));

	checkAllocation(leftArr);
	checkAllocation(rightArr);
	
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

void setupArr(SortData** sort_data, size_t sz) {
	SDL_free(*sort_data);
	*sort_data = SDL_malloc(sizeof(SortData) + sz * sizeof(short));
	checkAllocation(*sort_data);

	(*sort_data)->size = sz;
	(*sort_data)->index = 0;

	for (uint16_t i = 1; i <= sz; ++i) {
		(*sort_data)->arr[i-1] = i;
	}

	shuffle((*sort_data)->arr, sz);
}


bool isFRectHovered(SDL_FRect box, float mouse_x, float mouse_y) {
	return ((mouse_x >= box.x) && (mouse_x <= box.x + box.w) && (mouse_y >= box.y) && (mouse_y <= box.y + box.h));
}

size_t intLen(size_t n) {
	size_t length;

	length = 0;

	while (n != 0) {
		n /= 10;
		length++;
	}

	return length;
}
