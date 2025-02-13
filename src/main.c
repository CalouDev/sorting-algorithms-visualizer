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

   gcc src/*.c -o bin/app -I include -L lib -lmingw32 -lSDL3 -lSDL3_ttf -mwindows

*/

#define WIN_W 1280
#define WIN_H 640
#define SORTING_INTERVAL 75
#define GREEN_EFFECT_INTERVAL 10 
#define ARR_SIZE 100
#define N_ALGOS 5

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

// Useful vars

short arr[ARR_SIZE];

Uint64 sortingTimer, greenTimer;
bool sorting = false;
Ushort indexSorting;

Ushort algoChoosen = 0;

char* fontPath;

sortingState isSorted;

// Sorting Algos vars

const char* strSortingFunctions[N_ALGOS] = {"Insertion Sort", "Selection Sort", "Bubble Sort", "Quick Sort", "Merge Sort"};
TTF_Text* buttonsText[N_ALGOS];
sortingState (*sortingFunctions[N_ALGOS])(short[], Ushort, Ushort) = {sortInsertion, sortSelection, sortBubble, sortSelection, sortSelection};

short greenPassingIndex = -1;

// GUI vars

Button buttons[N_ALGOS];
SDL_MouseButtonFlags mouseData;

float mouseX, mouseY;
SDL_Cursor *handCursor, *defaultCursor;
bool elementHovered = false;

TTF_Font *font;
TTF_TextEngine* textEngine;
TTF_Text* topLeftText;
char* algoTextName;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv) {
	srand(time(NULL));

	// Filling the array

	for (Ushort i = 1; i <= ARR_SIZE; ++i)
		arr[i-1] = i;

	shuffle(arr, ARR_SIZE);

	// SDL Initialize
	
    if (!SDL_Init(SDL_INIT_VIDEO)) return SDL_APP_FAILURE;

	if (!TTF_Init()) return SDL_APP_FAILURE;

    window = SDL_CreateWindow("Sorting Algorithms Visualizer", WIN_W, WIN_H, SDL_WINDOW_OPENGL);

    if (!window) return SDL_APP_FAILURE;

    renderer = SDL_CreateRenderer(window, NULL);

    if (!renderer) return SDL_APP_FAILURE;

	isSorted = SORTING_CONTINUE;

	textEngine = TTF_CreateRendererTextEngine(renderer);

	handCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_POINTER);
	defaultCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_DEFAULT);
	
	SDL_asprintf(&fontPath, "%s/../font/%s", SDL_GetBasePath(), "sans.ttf");
	font = TTF_OpenFont(fontPath, 25);
	
	algoTextName = SDL_malloc(strlen(strSortingFunctions[0]) + strlen(" - Delay 75 ms") + 1);
	if (algoTextName == NULL) return SDL_APP_FAILURE;
	strcpy(algoTextName, strSortingFunctions[0]);
	strcat(algoTextName, " - delay 75 ms");
	topLeftText = TTF_CreateText(textEngine, font, algoTextName, strlen(algoTextName));

	sortingTimer = SDL_GetTicks();
	greenTimer = SDL_GetTicks();

	for (int i = 0; i < N_ALGOS; ++i) {
		buttons[i] = createButton(1050, 10 + 60 * i, 200, 50);
		buttonsText[i] = TTF_CreateText(textEngine, font, strSortingFunctions[i], strlen(strSortingFunctions[i]));
	}

	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
    SDL_DestroyWindow(window);
    window = NULL;
	SDL_free(fontPath);
	SDL_free(algoTextName);
	for (int i = 0; i < N_ALGOS; ++i) TTF_DestroyText(buttonsText[i]);
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
	SDL_Quit();

	TTF_DestroyGPUTextEngine(textEngine);
	TTF_DestroyText(topLeftText);
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
				for (int i = 0; i < N_ALGOS; ++i) {
					if (buttons[i].hovered) {
						buttons[i].pressed = true;
						sorting = false;
						isSorted = SORTING_CONTINUE;
						greenPassingIndex = -1;
						algoChoosen = i;
						for (Ushort i = 0; i < ARR_SIZE; ++i) arr[i] = abs(arr[i]);
						algoTextName = SDL_realloc(algoTextName, strlen(strSortingFunctions[i]) + strlen(" - delay 75 ms") + 1);
						if (algoTextName == NULL) return SDL_APP_FAILURE;
						strcpy(algoTextName, strSortingFunctions[i]);
						strcat(algoTextName, " - delay 75 ms");
						topLeftText = TTF_CreateText(textEngine, font, algoTextName, strlen(algoTextName));	
						shuffle(arr, ARR_SIZE);
					}
				}
			}
			break;
		case SDL_EVENT_MOUSE_BUTTON_UP:
			if (event->button.button == SDL_BUTTON_LEFT) {
				for (int i = 0; i < N_ALGOS; ++i) 
					if (buttons[i].pressed) buttons[i].pressed = false;
			}
			break;
		case SDL_EVENT_KEY_DOWN:
			switch (event->key.scancode) {
				case SDL_SCANCODE_SPACE:
					if (!sorting) {
						if (algoChoosen == 0) indexSorting = 1;
						else if (algoChoosen == 1 || algoChoosen == 2) indexSorting = 0;
						sortingTimer = SDL_GetTicks();
						sorting = true;
					}
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

	TTF_SetTextColor(topLeftText, 255, 255, 255, SDL_ALPHA_OPAQUE);
	TTF_DrawRendererText(topLeftText, 10, 10);

	SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
	for (short i = 0; i <= greenPassingIndex; ++i) SDL_RenderLine(renderer, 10 + i * 10, WIN_H, 10 + i * 10, WIN_H - abs(arr[i]) * 6);
		

	for (Ushort i = greenPassingIndex + 1; i < ARR_SIZE; ++i) {
		if (arr[i] < 0) SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
		else SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderLine(renderer, 10 + i * 10, WIN_H, 10 + i * 10, WIN_H - abs(arr[i]) * 6);
	}

	if (sorting && SDL_GetTicks() > sortingTimer + SORTING_INTERVAL) {
		for (Ushort i = 0; i < ARR_SIZE; ++i) { arr[i] = abs(arr[i]); }
		isSorted = sortingFunctions[algoChoosen](arr, ARR_SIZE, indexSorting);
		indexSorting++;
		if (isSorted == SORTING_STOP || indexSorting >= ARR_SIZE) {
			sorting = false;
			greenPassingIndex = 0;
		}

		sortingTimer = SDL_GetTicks();
	}

	if (greenPassingIndex >= 0 && greenPassingIndex < ARR_SIZE - 1 && SDL_GetTicks() > greenTimer + GREEN_EFFECT_INTERVAL) {
		greenPassingIndex++;
		greenTimer = SDL_GetTicks();
	}

	for (int i = 0; i < N_ALGOS; ++i) {
		renderButton(renderer, &buttons[i]);
		TTF_SetTextColor(buttonsText[i], 0, 0, 0, SDL_ALPHA_OPAQUE);
		TTF_DrawRendererText(buttonsText[i], 1060, 20 + 60 * i);
	}

	if (elementHovered) SDL_SetCursor(handCursor);
	else SDL_SetCursor(defaultCursor);

	SDL_RenderPresent(renderer);

	return SDL_APP_CONTINUE;
}
