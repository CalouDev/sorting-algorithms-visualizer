#include "../include/rendering.h"

void clearRenderer(SDL_Color bg_clr) {
    SDL_SetRenderDrawColor(renderer, bg_clr.r, bg_clr.g, bg_clr.b, bg_clr.a);
	SDL_RenderClear(renderer);
}

void renderText(void) {
	TTF_DrawRendererText(ttf_algo_text_name, 10.0f, 10.0f);
	TTF_DrawRendererText(ttf_delay_text, 1050.0f, 315.0f);

    for (uint16_t i = 0; i < N_ALGOS; ++i) {
		TTF_DrawRendererText(buttons_text[i], 1060.0f, 20 + 60.0f * i);
    }
}

void renderComponents(void) {
	for (uint16_t i = 0; i < N_ALGOS; ++i) {
		renderButton(renderer, &buttons[i]);
	}

    renderButton(renderer, &increment_delay_button);
	renderButton(renderer, &decrement_delay_button);

	SDL_RenderGeometry(renderer, NULL, increment_delay_button_triangle_vertices, 3, NULL, 3);
	SDL_RenderGeometry(renderer, NULL, decrement_delay_button_triangle_vertices, 3, NULL, 3);
}

void renderAndProcessSortingEffect(void) {
	for (uint16_t i = green_passing_index + 1; i < main_arr->size; ++i) {
		if (main_arr->arr[i] < 0) SDL_SetRenderDrawColor(renderer, CLR_RED.r, CLR_RED.g, CLR_RED.b, CLR_RED.a);
		else SDL_SetRenderDrawColor(renderer, CLR_WHITE.r, CLR_WHITE.g, CLR_WHITE.b, CLR_WHITE.a);
		SDL_RenderFillRect(renderer, &(SDL_FRect){1 + i * 4, WIN_H - SDL_abs(main_arr->arr[i]) * 2, 4, WIN_H});
	}

	if (sorting && SDL_GetTicks() > sorting_timer + sorting_interval) {
		for (uint16_t i = 0; i < main_arr->size; ++i) { main_arr->arr[i] = SDL_abs(main_arr->arr[i]); }
		is_sorted = sorting_functions[algo_choosen](main_arr);
		if (3 != algo_choosen) main_arr->index++;
		else main_arr->index *= 2;
		if (SORTING_STOP == is_sorted || main_arr->index >= main_arr->size) {
			sorting = false;
			green_passing_index = 0;
		}

		sorting_timer = SDL_GetTicks();
	}
}

void renderGreenEffect(void) {
	if (green_passing_index >= 0) {
		SDL_SetRenderDrawColor(renderer, CLR_GREEN.r, CLR_GREEN.g, CLR_GREEN.b, CLR_GREEN.a);
		for (short i = 0; i <= green_passing_index - 1; ++i)  SDL_RenderFillRect(renderer, &(SDL_FRect){1 + i * 4, WIN_H - SDL_abs(main_arr->arr[i]) * 2, 4, SDL_abs(main_arr->arr[i]) * 2});

		if (green_passing_index <= main_arr->size-1) {
			SDL_SetRenderDrawColor(renderer, CLR_RED.r, CLR_RED.g, CLR_RED.b, CLR_RED.a);
			SDL_RenderFillRect(renderer, &(SDL_FRect){1 + green_passing_index * 4, WIN_H - SDL_abs(main_arr->arr[green_passing_index]) * 2, 4, WIN_H});
		}
	}
}

void incrementGreenEffect(void) {
	if ((green_passing_index >= 0) && (green_passing_index < main_arr->size) && (SDL_GetTicks() > (green_timer + GREEN_EFFECT_INTERVAL))) {
		green_passing_index++;
		green_timer = SDL_GetTicks();
	}
}
