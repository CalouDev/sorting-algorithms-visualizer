#define SDL_MAIN_USE_CALLBACKS

#include <SDL3/SDL_main.h>
#include "../include/components.h"
#include "../include/setup.h"
#include "../include/sorting_algorithms.h"
#include "../include/utils.h"

SDL_AppResult SDL_AppInit(void **appstate __attribute__((unused)), int argc __attribute__((unused)), char **argv __attribute__((unused))) {
	SDL_srand(time(NULL));

	SDL_SetAppMetadata("Sorting Algorithms Visualizer", "0.7", NULL);
	SDL_SetAppMetadataProperty("SDL_PROP_APP_METADATA_NAME_STRING", "Sorting Algorithms Visualizer");
	SDL_SetAppMetadataProperty("SDL_PROP_APP_METADATA_VERSION_STRING", "13/04/2025");
	SDL_SetAppMetadataProperty("SDL_PROP_APP_METADATA_CREATOR_STRING", "CalouDev");
	
    if (!(SDL_Init(SDL_INIT_VIDEO) && SDL_Init(SDL_INIT_AUDIO) && TTF_Init())) return SDL_APP_FAILURE;

    window = SDL_CreateWindow("Sorting Algorithms Visualizer", WIN_W, WIN_H, SDL_WINDOW_OPENGL);
    checkAllocation(window);
    renderer = SDL_CreateRenderer(window, NULL);
    checkAllocation(renderer);

	main_arr = SDL_malloc(sizeof(SortData) + ARR_SIZE * sizeof(short));
	checkAllocation(main_arr);

	main_arr->size = ARR_SIZE;
	main_arr->index = 0;

	for (uint16_t i = 1; i <= main_arr->size; ++i) main_arr->arr[i-1] = i;
	shuffle(main_arr->arr, main_arr->size);

	sortingTimer = SDL_GetTicks();
	greenTimer = SDL_GetTicks();

	strSortingInterval = SDL_malloc(DELAY_MAX_LIM_N + 1);
	checkAllocation(strSortingInterval);
	SDL_snprintf(strSortingInterval, DELAY_MAX_LIM_N + 1, "%d", sortingInterval);

	SDL_asprintf(&fontPath, "%s/../font/%s", SDL_GetBasePath(), "sans.ttf");
	font = TTF_OpenFont(fontPath, 25);
	checkAllocation(font);

	algoTextName = SDL_malloc(strlen(strSortingFunctions[0]) + strlen(" - Delay  ms") + strlen(strSortingInterval) + 1);
	checkAllocation(algoTextName);
	strDelayText = SDL_malloc(strlen("Delay : ") + strlen(strSortingInterval) + 1);
	checkAllocation(strDelayText);
	SDL_snprintf(algoTextName, strlen(strSortingFunctions[0]) + strlen(" - Delay  ms") + strlen(strSortingInterval) + 1, "%s - delay %s ms", strSortingFunctions[algoChoosen], strSortingInterval);
	SDL_snprintf(strDelayText, strlen("Delay : ") + strlen(strSortingInterval) + 1, "Delay : %s", strSortingInterval);

	textEngine = TTF_CreateRendererTextEngine(renderer);
	checkAllocation(textEngine);
	delayText = TTF_CreateText(textEngine, font, strDelayText, strlen(strDelayText));
	checkAllocation(delayText);
	topLeftText = TTF_CreateText(textEngine, font, algoTextName, strlen(algoTextName));
	checkAllocation(topLeftText);

	handCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_POINTER);
	checkAllocation(handCursor);
	defaultCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_DEFAULT);
	checkAllocation(defaultCursor);

	incrementDelayButton = createButton(1230, 310, 20, 20);
	decrementDelayButton = createButton(1230, 335, 20, 20);

	for (uint16_t i = 0; i < N_ALGOS; ++i) {
		buttons[i] = createButton(1050, 10 + 60 * i, 200, 50);
		buttonsText[i] = TTF_CreateText(textEngine, font, strSortingFunctions[i], strlen(strSortingFunctions[i]));
		checkAllocation(buttonsText[i]);
	}

	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate __attribute__((unused)), SDL_AppResult result __attribute__((unused))) {
	SDL_free(main_arr);
	SDL_free(strSortingInterval);
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
    SDL_DestroyWindow(window);
    window = NULL;
	SDL_free(fontPath);
	fontPath = NULL;
	SDL_free(algoTextName);
	algoTextName = NULL;
	SDL_free(strDelayText);
	strDelayText = NULL;
	for (uint16_t i = 0; i < N_ALGOS; ++i) TTF_DestroyText(buttonsText[i]);
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
	SDL_Quit();

	TTF_DestroyGPUTextEngine(textEngine);
	TTF_DestroyText(topLeftText);
	TTF_Quit();
}

SDL_AppResult SDL_AppEvent(void *appstate __attribute__((unused)), SDL_Event *event) {
	switch (event->type) {
		case SDL_EVENT_QUIT:
			return SDL_APP_SUCCESS;
		case SDL_EVENT_MOUSE_MOTION:
			elementHovered = false;
			incrementDelayButton.hovered = isHovered(incrementDelayButton.box, event->motion.x, event->motion.y);
			decrementDelayButton.hovered = isHovered(decrementDelayButton.box, event->motion.x, event->motion.y);
			if (incrementDelayButton.hovered || decrementDelayButton.hovered) elementHovered = true;
			for (uint16_t i = 0; i < N_ALGOS; ++i) {
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
					for (uint16_t i = 0; i < N_ALGOS; ++i) {
						if (buttons[i].hovered) {
							buttons[i].pressed = true;
							sorting = false;
							isSorted = SORTING_CONTINUE;
							greenPassingIndex = -1;
							algoChoosen = i;
							for (uint16_t j = 0; j < main_arr->size; ++j) main_arr->arr[j] = SDL_abs(main_arr->arr[j]);
							shuffle(main_arr->arr, main_arr->size);
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
				for (uint16_t i = 0; i < N_ALGOS; ++i) 
					if (buttons[i].pressed) buttons[i].pressed = false;
			}
			break;
		case SDL_EVENT_KEY_DOWN:
			switch (event->key.scancode) {
				case SDL_SCANCODE_SPACE:
					if (!sorting) {
						main_arr->index = (algoChoosen == 0 || algoChoosen == 3) ? 1 : 0;
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

SDL_AppResult SDL_AppIterate(void *appstate __attribute__((unused))) {
	mouseData = SDL_GetMouseState(&mouseX, &mouseY);	

	SDL_SetRenderDrawColor(renderer, 10, 10, 10, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);

	TTF_SetTextColor(topLeftText, 255, 255, 255, SDL_ALPHA_OPAQUE);
	TTF_DrawRendererText(topLeftText, 10, 10);
	TTF_DrawRendererText(delayText, 1050, 315);

	if (greenPassingIndex >= 0) {
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
		for (short i = 0; i <= greenPassingIndex - 1; ++i)  SDL_RenderFillRect(renderer, &(SDL_FRect){1 + i * 4, WIN_H - SDL_abs(main_arr->arr[i]) * 2, 4, SDL_abs(main_arr->arr[i]) * 2});

		if (greenPassingIndex <= main_arr->size-1) {
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
			SDL_RenderFillRect(renderer, &(SDL_FRect){1 + greenPassingIndex * 4, WIN_H - SDL_abs(main_arr->arr[greenPassingIndex]) * 2, 4, WIN_H});
		}
	}

	for (uint16_t i = greenPassingIndex + 1; i < main_arr->size; ++i) {
		if (main_arr->arr[i] < 0) SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
		else SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderFillRect(renderer, &(SDL_FRect){1 + i * 4, WIN_H - SDL_abs(main_arr->arr[i]) * 2, 4, WIN_H});
	}

	if (sorting && SDL_GetTicks() > sortingTimer + sortingInterval) {
		for (uint16_t i = 0; i < main_arr->size; ++i) { main_arr->arr[i] = SDL_abs(main_arr->arr[i]); }
		isSorted = sortingFunctions[algoChoosen](main_arr);
		if (algoChoosen != 3) main_arr->index++;
		else main_arr->index *= 2;
		if (isSorted == SORTING_STOP || main_arr->index >= main_arr->size) {
			sorting = false;
			greenPassingIndex = 0;
		}

		sortingTimer = SDL_GetTicks();
	}

	if (greenPassingIndex >= 0 && greenPassingIndex < main_arr->size && SDL_GetTicks() > greenTimer + GREEN_EFFECT_INTERVAL) {
		greenPassingIndex++;
		greenTimer = SDL_GetTicks();
	}

	renderButton(renderer, &incrementDelayButton);
	renderButton(renderer, &decrementDelayButton);

	for (uint16_t i = 0; i < N_ALGOS; ++i) {
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
