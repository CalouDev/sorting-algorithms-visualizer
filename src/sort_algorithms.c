#include "../include/sort_algorithms.h"

void swap(ushort *a, ushort *b) {
	ushort temp = *a;
	*a = *b;
	*b = temp;
}

void sortInsertion(ushort arr[], ushort len) {
	for (ushort i = 1; i < len; ++i) {
		ushort currentElement = arr[i];
		short currentIndex = i - 1;

		while (currentIndex >= 0 && currentElement < arr[currentIndex]) {
			arr[currentIndex + 1] = arr[currentIndex];
			currentIndex--;
		}

		arr[currentIndex + 1] = currentElement;
	}
}

void sortSelection(ushort arr[], ushort len) {
	ushort mini, index;

	for (ushort i = 0; i < len - 1; ++i) {
		index = i;
		mini = arr[index];

		for (ushort j = i + 1; j < len; ++j) {
			if (arr[j] < mini) {
				mini = arr[j];
				index = j;
			}
		}

		swap(&arr[i], &arr[index]);
	}
}

