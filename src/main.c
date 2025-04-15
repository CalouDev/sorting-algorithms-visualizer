#define SDL_MAIN_USE_CALLBACKS

#include <SDL3/SDL_main.h>
#include "../include/setup.h"

SDL_AppResult SDL_AppInit(void **appstate __attribute__((unused)), int argc __attribute__((unused)), char **argv __attribute__((unused))) {
	initializeSDL();

	initializeTextEngineTTF();

	initializeComponents();

	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate __attribute__((unused)), SDL_AppResult result __attribute__((unused))) {
	freeAll();
}

SDL_AppResult SDL_AppEvent(void *appstate __attribute__((unused)), SDL_Event *event) {
	switch (event->type) {
		case SDL_EVENT_QUIT:

			return SDL_APP_SUCCESS;

		case SDL_EVENT_MOUSE_MOTION:
			element_hovered = false;
			increment_delay_button.hovered = isFRectHovered(increment_delay_button.box, event->motion.x, event->motion.y);
			decrement_delay_button.hovered = isFRectHovered(decrement_delay_button.box, event->motion.x, event->motion.y);
			if (increment_delay_button.hovered || decrement_delay_button.hovered) element_hovered = true;
			for (uint16_t i = 0; i < N_ALGOS; ++i) {
				buttons[i].hovered = isFRectHovered(buttons[i].box, event->motion.x, event->motion.y); 
				if (buttons[i].hovered) element_hovered = true;
			}

			break;

		case SDL_EVENT_MOUSE_BUTTON_DOWN:

			if (event->button.button == SDL_BUTTON_LEFT) {
				if (increment_delay_button.hovered) {
					increment_delay_button.pressed = true;
					if (sorting_interval < MAX_DELAY) sorting_interval += STEP_DELAY;
				} else if (decrement_delay_button.hovered) {
					decrement_delay_button.pressed = true;
					if (sorting_interval > MIN_DELAY) sorting_interval -= STEP_DELAY;
				} else {
					for (uint16_t i = 0; i < N_ALGOS; ++i) {
						if (buttons[i].hovered) {
							buttons[i].pressed = true;
							sorting = false;
							is_sorted = SORTING_CONTINUE;
							green_passing_index = -1;
							algo_choosen = i;
							for (uint16_t j = 0; j < main_arr->size; ++j) main_arr->arr[j] = SDL_abs(main_arr->arr[j]);
							shuffle(main_arr->arr, main_arr->size);
						}
					}
				}

				SDL_snprintf(str_sorting_interval, DELAY_MAX_LIM_N + 1, "%d", sorting_interval);
				SDL_free(algo_text_name);
				algo_text_name = SDL_malloc(strlen(str_sorting_functions[algo_choosen]) + strlen(" - delay  ms") + strlen(str_sorting_interval) + 1);
				checkAllocation(algo_text_name);
				str_delay_text = SDL_realloc(str_delay_text, strlen("Delay : ") + strlen(str_sorting_interval) + 1);
				checkAllocation(algo_text_name);
				SDL_snprintf(algo_text_name, strlen(str_sorting_functions[algo_choosen]) + strlen(" - Delay  ms") + strlen(str_sorting_interval) + 1, "%s - delay %s ms", str_sorting_functions[algo_choosen], str_sorting_interval);
				SDL_snprintf(str_delay_text, strlen("Delay : ") + strlen(str_sorting_interval) + 1, "Delay : %s", str_sorting_interval);
				ttf_algo_text_name = TTF_CreateText(text_engine, font, algo_text_name, strlen(algo_text_name));
				ttf_delay_text = TTF_CreateText(text_engine, font, str_delay_text, strlen(str_delay_text));
			}
			break;
		case SDL_EVENT_MOUSE_BUTTON_UP:
			if (event->button.button == SDL_BUTTON_LEFT) {
				if (increment_delay_button.pressed) increment_delay_button.pressed = false;
				if (decrement_delay_button.pressed) decrement_delay_button.pressed = false;
				for (uint16_t i = 0; i < N_ALGOS; ++i) 
					if (buttons[i].pressed) buttons[i].pressed = false;
			}
			break;
		case SDL_EVENT_KEY_DOWN:
			switch (event->key.scancode) {
				case SDL_SCANCODE_SPACE:
					if (!sorting) {
						main_arr->index = (algo_choosen == 0 || algo_choosen == 3) ? 1 : 0;
						sorting_timer = SDL_GetTicks();
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
	mouse_data = SDL_GetMouseState(&mouse_x, &mouse_y);

	if (element_hovered) SDL_SetCursor(hand_cursor);
	else SDL_SetCursor(default_cursor);

	clearRenderer(CLR_BLACK);

	renderGreenEffect();

	renderAndProcessSortingEffect();

	incrementGreenEffect();

	renderComponents();
	renderText();

	SDL_RenderPresent(renderer);

	return SDL_APP_CONTINUE;
}
