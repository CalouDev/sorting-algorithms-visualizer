#include "../include/sorting_algorithms.h"
#include "../include/utils.h"

void sortInsertion(Ushort arr[], Ushort size) {
        for (Ushort i = 1; i < size; ++i) {
                Ushort currentElement = arr[i];
                short currentIndex = i - 1;

                while (currentIndex >= 0 && currentElement < arr[currentIndex]) {
                        arr[currentIndex + 1] = arr[currentIndex];
                        currentIndex--;
                }

                arr[currentIndex + 1] = currentElement;
        }
}

void sortSelection(Ushort arr[], Ushort size) {
        Ushort mini, index;

        for (Ushort i = 0; i < size - 1; ++i) {
                index = i;
                mini = arr[index];

                for (Ushort j = i + 1; j < size; ++j) {
                        if (arr[j] < mini) {
                                mini = arr[j];
                                index = j;
                        }
                }

                swap(&arr[i], &arr[index]);
        }
}
