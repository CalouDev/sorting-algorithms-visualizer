#include "../include/sorting_algorithms.h"
#include "../include/utils.h"

sortingState sortInsertion(short arr[], Ushort size, Ushort index) {
	short currentElement = arr[index];
	short currentIndex = index - 1;	

	while (currentIndex >= 0 && currentElement < arr[currentIndex]) {
		arr[currentIndex + 1] = arr[currentIndex];
		currentIndex--;
	}

	arr[currentIndex + 1] = (index < size - 1 ? -1 : 1) * currentElement;

	return SORTING_CONTINUE;
}

sortingState sortSelection(short arr[], Ushort size, Ushort index) {
	Ushort mini, indexMin;
	indexMin = index;
	mini = arr[indexMin];

	for (Ushort j = index + 1; j < size; ++j) {
		if (arr[j] < mini) {
			mini = arr[j];
			indexMin = j;
		}
	}

	swap(&arr[index], &arr[indexMin]);
	arr[index] = -arr[index];
	arr[indexMin] = -arr[indexMin];

	return SORTING_CONTINUE;
}


sortingState sortBubble(short arr[], Ushort size, Ushort index) {
	sortingState swapped = SORTING_STOP;
	for (Ushort j = 0; j < size - index - 1; ++j) {
		if (SDL_abs(arr[j]) > SDL_abs(arr[j+1])) {
			swap(&arr[j], &arr[j+1]);
			arr[j] = -arr[j];
			arr[j+1] = -arr[j+1];
			swapped = SORTING_CONTINUE;
		}
	}

	return swapped;
}

sortingState sortMerge(short arr[], Ushort size, Ushort index) {
	for (Ushort i = 0; i < size - 1; i += index * 2) {
		Ushort mid = min(i + index - 1, size - 1);
		Ushort right = min(i + 2 * index - 1, size -1);

		mergeSubArr(arr, i, right, mid);
	}

	return SORTING_CONTINUE;
}

