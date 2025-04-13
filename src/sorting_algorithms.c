#include "../include/sorting_algorithms.h"
#include "../include/utils.h"

SortingState sortInsertion(SortData* sort_data) {
	short currentElement, currentIndex;	

	currentElement = sort_data->arr[sort_data->index];
	currentIndex = sort_data->index - 1;

	while (currentIndex >= 0 && currentElement < sort_data->arr[currentIndex]) {
		sort_data->arr[currentIndex + 1] = sort_data->arr[currentIndex];
		currentIndex--;
	}

	sort_data->arr[currentIndex + 1] = (sort_data->index < sort_data->size - 1 ? -1 : 1) * currentElement;

	return SORTING_CONTINUE;
}

SortingState sortSelection(SortData* sort_data) {
	uint16_t index_min, mini, j;

	index_min = sort_data->index;
	mini = sort_data->arr[index_min];

	for (j = sort_data->index + 1; j < sort_data->size; ++j) {
		if (sort_data->arr[j] < mini) {
			mini = sort_data->arr[j];
			index_min = j;
		}
	}

	swap(&sort_data->arr[sort_data->index], &sort_data->arr[index_min]);
	sort_data->arr[sort_data->index] = -sort_data->arr[sort_data->index];
	sort_data->arr[index_min] = -sort_data->arr[index_min];

	return SORTING_CONTINUE;
}


SortingState sortBubble(SortData* sort_data) {
	SortingState swapped;
	uint16_t j;
	
	swapped = SORTING_STOP;

	for (j = 0; j < (sort_data->size - sort_data->index - 1); ++j) {
		if (SDL_abs(sort_data->arr[j]) > SDL_abs(sort_data->arr[j+1])) {
			swap(&sort_data->arr[j], &sort_data->arr[j+1]);
			sort_data->arr[j] = -sort_data->arr[j];
			sort_data->arr[j+1] = -sort_data->arr[j+1];
			swapped = SORTING_CONTINUE;
		}
	}

	return swapped;
}

SortingState sortMerge(SortData* sort_data) {
	uint16_t i, mid, right;


	for (i = 0; i < sort_data->size - 1; i += sort_data->index * 2) {
		mid = SDL_min(i + sort_data->index - 1, sort_data->size - 1);
		right = SDL_min(i + 2 * sort_data->index - 1, sort_data->size -1);

		mergeSubArr(sort_data->arr, i, right, mid);
	}

	return SORTING_CONTINUE;
}

