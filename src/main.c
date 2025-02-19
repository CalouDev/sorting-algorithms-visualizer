#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_ttf.h>
#include <SDL3/SDL_mixer.h>
#include<time.h>
#include <stdio.h>
#include "../include/sorting_algorithms.h"
#include "../include/utils.h"
#include "../include/components.h"

#define WIN_W 1280
#define WIN_H 640
#define GREEN_EFFECT_INTERVAL 2 
#define ARR_SIZE 250 
#define N_ALGOS 4
#define DELAY_MAX_LIM_N 4

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

Ushort sortingInterval = 75;
char strSortingInterval[DELAY_MAX_LIM_N];

// Sorting Algos vars

const char* strSortingFunctions[N_ALGOS] = {"Insertion Sort", "Selection Sort", "Bubble Sort", "Merge Sort"};
TTF_Text* buttonsText[N_ALGOS];
sortingState (*sortingFunctions[N_ALGOS])(short[], Ushort, Ushort) = {sortInsertion, sortSelection, sortBubble, sortMerge};

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

char* strDelayText;
TTF_Text* delayText;
Button incrementDelayButton;
Button decrementDelayButton;
const SDL_Vertex incrementDelayButtonTriangleVertices[3] = {{{1240, 312}, {0.4, 0.4, 0.4, SDL_ALPHA_OPAQUE}},
															{{1232, 328}, {0.4, 0.4, 0.4, SDL_ALPHA_OPAQUE}},
															{{1248, 328}, {0.4, 0.4, 0.4, SDL_ALPHA_OPAQUE}}};
const SDL_Vertex decrementDelayButtonTriangleVertices[3] = {{{1232, 337}, {0.4, 0.4, 0.4, SDL_ALPHA_OPAQUE}},
															{{1248, 337}, {0.4, 0.4, 0.4, SDL_ALPHA_OPAQUE}},
															{{1240, 353}, {0.4, 0.4, 0.4, SDL_ALPHA_OPAQUE}}};

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv) {
	SDL_srand(time(NULL));

	// Filling the array

	for (Ushort i = 1; i <= ARR_SIZE; ++i) arr[i-1] = i;

	shuffle(arr, ARR_SIZE);

	// SDL Initialize
	
    if (!SDL_Init(SDL_INIT_VIDEO)) return SDL_APP_FAILURE;
	if (!SDL_Init(SDL_INIT_AUDIO)) return SDL_APP_FAILURE;
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
	SDL_snprintf(strSortingInterval, DELAY_MAX_LIM_N + 1, "%d", sortingInterval);
	algoTextName = SDL_malloc(strlen(strSortingFunctions[0]) + strlen(" - Delay  ms") + strlen(strSortingInterval) + 1);
	strDelayText = SDL_malloc(strlen("Delay : ") + strlen(strSortingInterval) + 1);
	if (algoTextName == NULL) return SDL_APP_FAILURE;
	SDL_snprintf(algoTextName, strlen(strSortingFunctions[0]) + strlen(" - Delay  ms") + strlen(strSortingInterval) + 1, "%s - delay %s ms", strSortingFunctions[algoChoosen], strSortingInterval);
	SDL_snprintf(strDelayText, strlen("Delay : ") + strlen(strSortingInterval) + 1, "Delay : %s", strSortingInterval);
	delayText = TTF_CreateText(textEngine, font, strDelayText, strlen(strDelayText));
	topLeftText = TTF_CreateText(textEngine, font, algoTextName, strlen(algoTextName));

	sortingTimer = SDL_GetTicks();
	greenTimer = SDL_GetTicks();

	incrementDelayButton = createButton(1230, 310, 20, 20);
	decrementDelayButton = createButton(1230, 335, 20, 20);

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
	SDL_free(strDelayText);
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
			incrementDelayButton.hovered = isHovered(incrementDelayButton.box, event->motion.x, event->motion.y);
			decrementDelayButton.hovered = isHovered(decrementDelayButton.box, event->motion.x, event->motion.y);
			if (incrementDelayButton.hovered || decrementDelayButton.hovered) elementHovered = true;
			for (int i = 0; i < N_ALGOS; ++i) {
				buttons[i].hovered = isHovered(buttons[i].box, event->motion.x, event->motion.y); 
				if (buttons[i].hovered) elementHovered = true;
			}

			break;
		case SDL_EVENT_MOUSE_BUTTON_DOWN:
			if (event->button.button == SDL_BUTTON_LEFT) {
				if (incrementDelayButton.hovered) {
					incrementDelayButton.pressed = true;
					if (sortingInterval < 974) sortingInterval += 25;
				} else if (decrementDelayButton.hovered) {
					decrementDelayButton.pressed = true;
					if (sortingInterval > 25) sortingInterval -= 25;
				} else {
					for (int i = 0; i < N_ALGOS; ++i) {
						if (buttons[i].hovered) {
							buttons[i].pressed = true;
							sorting = false;
							isSorted = SORTING_CONTINUE;
							greenPassingIndex = -1;
							algoChoosen = i;
							for (Ushort j = 0; j < ARR_SIZE; ++j) arr[j] = SDL_abs(arr[j]);
							shuffle(arr, ARR_SIZE);
						}
					}
				}

				SDL_snprintf(strSortingInterval, DELAY_MAX_LIM_N + 1, "%d", sortingInterval);
				algoTextName = SDL_realloc(algoTextName, strlen(strSortingFunctions[algoChoosen]) + strlen(" - delay  ms") + strlen(strSortingInterval) + 1);
				if (algoTextName == NULL) return SDL_APP_FAILURE;
				strDelayText = SDL_realloc(strDelayText, strlen("Delay : ") + strlen(strSortingInterval) + 1);
				if (strDelayText == NULL) return SDL_APP_FAILURE;
				SDL_snprintf(algoTextName, strlen(strSortingFunctions[algoChoosen]) + strlen(" - Delay  ms") + strlen(strSortingInterval) + 1, "%s - delay %s ms", strSortingFunctions[algoChoosen], strSortingInterval);
				SDL_snprintf(strDelayText, strlen("Delay : ") + strlen(strSortingInterval) + 1, "Delay : %s", strSortingInterval);
				topLeftText = TTF_CreateText(textEngine, font, algoTextName, strlen(algoTextName));
				delayText = TTF_CreateText(textEngine, font, strDelayText, strlen(strDelayText));
			}
			break;
		case SDL_EVENT_MOUSE_BUTTON_UP:
			if (event->button.button == SDL_BUTTON_LEFT) {
				if (incrementDelayButton.pressed) incrementDelayButton.pressed = false;
				if (decrementDelayButton.pressed) decrementDelayButton.pressed = false;
				for (int i = 0; i < N_ALGOS; ++i) 
					if (buttons[i].pressed) buttons[i].pressed = false;
			}
			break;
		case SDL_EVENT_KEY_DOWN:
			switch (event->key.scancode) {
				case SDL_SCANCODE_SPACE:
					if (!sorting) {
						indexSorting = (algoChoosen == 0 || algoChoosen == 3) ? 1 : 0;
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
	TTF_DrawRendererText(delayText, 1050, 315);

	if (greenPassingIndex >= 0) {
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
		for (short i = 0; i <= greenPassingIndex - 1; ++i)  SDL_RenderFillRect(renderer, &(SDL_FRect){1 + i * 4, WIN_H - SDL_abs(arr[i]) * 2, 4, SDL_abs(arr[i]) * 2});

		if (greenPassingIndex <= ARR_SIZE-1) {
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
			SDL_RenderFillRect(renderer, &(SDL_FRect){1 + greenPassingIndex * 4, WIN_H - SDL_abs(arr[greenPassingIndex]) * 2, 4, WIN_H});
		}
	}

	for (Ushort i = greenPassingIndex + 1; i < ARR_SIZE; ++i) {
		if (arr[i] < 0) SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
		else SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderFillRect(renderer, &(SDL_FRect){1 + i * 4, WIN_H - SDL_abs(arr[i]) * 2, 4, WIN_H});
	}

	if (sorting && SDL_GetTicks() > sortingTimer + sortingInterval) {
		for (Ushort i = 0; i < ARR_SIZE; ++i) { arr[i] = SDL_abs(arr[i]); }
		isSorted = sortingFunctions[algoChoosen](arr, ARR_SIZE, indexSorting);
		if (algoChoosen != 3) indexSorting++;
		else indexSorting *= 2;
		if (isSorted == SORTING_STOP || indexSorting >= ARR_SIZE) {
			sorting = false;
			greenPassingIndex = 0;
		}

		sortingTimer = SDL_GetTicks();
	}

	if (greenPassingIndex >= 0 && greenPassingIndex < ARR_SIZE && SDL_GetTicks() > greenTimer + GREEN_EFFECT_INTERVAL) {
		greenPassingIndex++;
		greenTimer = SDL_GetTicks();
	}

	renderButton(renderer, &incrementDelayButton);
	renderButton(renderer, &decrementDelayButton);

	for (int i = 0; i < N_ALGOS; ++i) {
		renderButton(renderer, &buttons[i]);
		TTF_SetTextColor(buttonsText[i], 0, 0, 0, SDL_ALPHA_OPAQUE);
		TTF_DrawRendererText(buttonsText[i], 1060, 20 + 60 * i);
	}

	SDL_RenderGeometry(renderer, NULL, incrementDelayButtonTriangleVertices, 3, NULL, 3);
	SDL_RenderGeometry(renderer, NULL, decrementDelayButtonTriangleVertices, 3, NULL, 3);

	if (elementHovered) SDL_SetCursor(handCursor);
	else SDL_SetCursor(defaultCursor);

	SDL_RenderPresent(renderer);

	return SDL_APP_CONTINUE;
}
