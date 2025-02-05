#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "../include/sorting_algorithms.h"
#include "../include/utils.h"
#include "../include/components.h"

/*

   gcc src/main.c src/sorting_algorithms.c src/utils.c -o bin/app -I include -L lib -lmingw32 -lSDL3 -lSDL3_ttf -mwindows

*/

#define WIN_W 1280
#define WIN_H 640
#define INTERVALLE 75 // ms
#define ARR_SIZE 100
#define N_ALGOS 5

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

// Useful vars

Ushort arr[ARR_SIZE];

Uint64 lastTime;
bool sorting = false;
Ushort indexSorting;

// Sorting Algos vars

const char* strSortingFunctions[N_ALGOS] = {"Insertion Sort", "Selection Sort", "Bubble Sort", "Quick Sort", "Merge Sort"};
void (*sortingFunctions[N_ALGOS])(Ushort[], Ushort, Ushort) = {sortInsertion, sortSelection, sortSelection, sortSelection, sortSelection};

// GUI vars

Button buttons[N_ALGOS];
SDL_MouseButtonFlags mouseData;

float mouseX, mouseY;
SDL_Cursor *handCursor, *defaultCursor;
bool elementHovered = false;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv) {
	srand(time(NULL));

	// Filling the array

	for (Ushort i = 1; i <= ARR_SIZE; ++i)
		arr[i-1] = i;

	shuffle(arr, ARR_SIZE);

	// SDL Initialize
	
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Error initializing SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

	if (!TTF_Init()) {
		SDL_Log("Error initializing TTF: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

    window = SDL_CreateWindow("Sorting Algorithms Visualizer", WIN_W, WIN_H, SDL_WINDOW_OPENGL);

    if (!window) {
        SDL_Log("Error creating window: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    renderer = SDL_CreateRenderer(window, NULL);

    if (!renderer) {
        SDL_Log("Error creating renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

	handCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_POINTER);
	defaultCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_DEFAULT);

	lastTime = SDL_GetTicks();

	for (int i = 0; i < N_ALGOS; ++i) {
		buttons[i] = createButton(1050, 10 + 60 * i, 200, 50);
	}

	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
    SDL_DestroyWindow(window);
    window = NULL;
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
	SDL_Quit();
	TTF_Quit();
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
	switch (event->type) {
		case SDL_EVENT_QUIT:
			return SDL_APP_SUCCESS;
		case SDL_EVENT_MOUSE_MOTION:
			elementHovered = false;
			for (int i = 0; i < N_ALGOS; ++i) {
				buttons[i].hovered = isHovered(buttons[i].box, event->motion.x, event->motion.y); 
				if (buttons[i].hovered) elementHovered = true;
			}

			break;
		case SDL_EVENT_MOUSE_BUTTON_DOWN:
			if (event->button.button == SDL_BUTTON_LEFT) {
				;
			}
			break;
		case SDL_EVENT_KEY_DOWN:
			switch (event->key.scancode) {
				case SDL_SCANCODE_SPACE:
					indexSorting = 0;
					sorting = true;
					lastTime = SDL_GetTicks();
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
	mouseData = SDL_GetMouseState(&mouseX, &mouseY);	

	SDL_SetRenderDrawColor(renderer, 10, 10, 10, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 200, 200, 200, SDL_ALPHA_OPAQUE);

	for (Ushort i = 0; i < ARR_SIZE; ++i) {
		SDL_RenderLine(renderer, 10 + i * 10, WIN_H, 10 + i * 10, WIN_H - arr[i] * 6);
	}

	if (sorting && SDL_GetTicks() > lastTime + INTERVALLE) {
		lastTime = SDL_GetTicks();
		sortingFunctions[1](arr, ARR_SIZE, indexSorting);
		indexSorting++;
		if (indexSorting >= ARR_SIZE) sorting = false;
	}

	for (int i = 0; i < N_ALGOS; ++i)
		renderButton(renderer, &buttons[i]);

	if (elementHovered) SDL_SetCursor(handCursor);
	else SDL_SetCursor(defaultCursor);

	SDL_RenderPresent(renderer);

	return SDL_APP_CONTINUE;
}
