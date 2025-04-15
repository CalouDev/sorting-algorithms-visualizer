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
