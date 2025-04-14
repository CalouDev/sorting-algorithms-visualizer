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
			increment_delay_button.hovered = isHovered(increment_delay_button.box, event->motion.x, event->motion.y);
			decrement_delay_button.hovered = isHovered(decrement_delay_button.box, event->motion.x, event->motion.y);
			if (increment_delay_button.hovered || decrement_delay_button.hovered) element_hovered = true;
			for (uint16_t i = 0; i < N_ALGOS; ++i) {
				buttons[i].hovered = isHovered(buttons[i].box, event->motion.x, event->motion.y); 
				if (buttons[i].hovered) element_hovered = true;
			}

			break;
		case SDL_EVENT_MOUSE_BUTTON_DOWN:
			if (event->button.button == SDL_BUTTON_LEFT) {
				if (increment_delay_button.hovered) {
					increment_delay_button.pressed = true;
					if (sorting_interval < 974) sorting_interval += 25;
				} else if (decrement_delay_button.hovered) {
					decrement_delay_button.pressed = true;
					if (sorting_interval > 25) sorting_interval -= 25;
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
				if (algo_text_name == NULL) return SDL_APP_FAILURE;
				str_delay_text = SDL_realloc(str_delay_text, strlen("Delay : ") + strlen(str_sorting_interval) + 1);
				if (str_delay_text == NULL) return SDL_APP_FAILURE;
				SDL_snprintf(algo_text_name, strlen(str_sorting_functions[algo_choosen]) + strlen(" - Delay  ms") + strlen(str_sorting_interval) + 1, "%s - delay %s ms", str_sorting_functions[algo_choosen], str_sorting_interval);
				SDL_snprintf(str_delay_text, strlen("Delay : ") + strlen(str_sorting_interval) + 1, "Delay : %s", str_sorting_interval);
				top_left_text = TTF_CreateText(text_engine, font, algo_text_name, strlen(algo_text_name));
				delay_text = TTF_CreateText(text_engine, font, str_delay_text, strlen(str_delay_text));
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

	SDL_SetRenderDrawColor(renderer, 10, 10, 10, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);

	TTF_SetTextColor(top_left_text, 255, 255, 255, 255);
	TTF_DrawRendererText(top_left_text, 10, 10);
	TTF_DrawRendererText(delay_text, 1050, 315);

	if (green_passing_index >= 0) {
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
		for (short i = 0; i <= green_passing_index - 1; ++i)  SDL_RenderFillRect(renderer, &(SDL_FRect){1 + i * 4, WIN_H - SDL_abs(main_arr->arr[i]) * 2, 4, SDL_abs(main_arr->arr[i]) * 2});

		if (green_passing_index <= main_arr->size-1) {
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
			SDL_RenderFillRect(renderer, &(SDL_FRect){1 + green_passing_index * 4, WIN_H - SDL_abs(main_arr->arr[green_passing_index]) * 2, 4, WIN_H});
		}
	}

	for (uint16_t i = green_passing_index + 1; i < main_arr->size; ++i) {
		if (main_arr->arr[i] < 0) SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
		else SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderFillRect(renderer, &(SDL_FRect){1 + i * 4, WIN_H - SDL_abs(main_arr->arr[i]) * 2, 4, WIN_H});
	}

	if (sorting && SDL_GetTicks() > sorting_timer + sorting_interval) {
		for (uint16_t i = 0; i < main_arr->size; ++i) { main_arr->arr[i] = SDL_abs(main_arr->arr[i]); }
		is_sorted = sorting_functions[algo_choosen](main_arr);
		if (algo_choosen != 3) main_arr->index++;
		else main_arr->index *= 2;
		if (is_sorted == SORTING_STOP || main_arr->index >= main_arr->size) {
			sorting = false;
			green_passing_index = 0;
		}

		sorting_timer = SDL_GetTicks();
	}

	if (green_passing_index >= 0 && green_passing_index < main_arr->size && SDL_GetTicks() > green_timer + GREEN_EFFECT_INTERVAL) {
		green_passing_index++;
		green_timer = SDL_GetTicks();
	}

	renderButton(renderer, &increment_delay_button);
	renderButton(renderer, &decrement_delay_button);

	for (uint16_t i = 0; i < N_ALGOS; ++i) {
		renderButton(renderer, &buttons[i]);
		TTF_SetTextColor(buttons_text[i], 0, 0, 0, SDL_ALPHA_OPAQUE);
		TTF_DrawRendererText(buttons_text[i], 1060, 20 + 60 * i);
	}

	SDL_RenderGeometry(renderer, NULL, increment_delay_button_triangle_vertices, 3, NULL, 3);
	SDL_RenderGeometry(renderer, NULL, decrement_delay_button_triangle_vertices, 3, NULL, 3);

	if (element_hovered) SDL_SetCursor(hand_cursor);
	else SDL_SetCursor(default_cursor);

	SDL_RenderPresent(renderer);

	return SDL_APP_CONTINUE;
}
