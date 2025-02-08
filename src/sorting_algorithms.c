#include "../include/sorting_algorithms.h"
#include "../include/utils.h"


void sortInsertion(Ushort arr[], Ushort size, Ushort index) {
	Ushort currentElement = arr[index];
	short currentIndex = index - 1;

	while (currentIndex >= 0 && currentElement < arr[currentIndex]) {
		arr[currentIndex + 1] = arr[currentIndex];
		currentIndex--;
	}

	arr[currentIndex + 1] = currentElement;
}

void sortSelection(Ushort arr[], Ushort size, Ushort index) {
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
}


void sortBubble(Ushort arr[], Ushort size, Ushort index) {
	for (Ushort j = 0; j < size - index - 1; ++j)
		if (arr[j] > arr[j+1]) swap(&arr[j], &arr[j+1]);
}

