#include "../include/components.h"

void createButton(Button* btn, SDL_FRect box, SDL_Color default_clr, SDL_Color hovered_clr, SDL_Color pressed_clr) {
    btn->box = box;
	btn->default_clr = default_clr;
	btn->hovered_clr = hovered_clr;
	btn->pressed_clr = pressed_clr;
	btn->hovered = false;
	btn->pressed = false;
}

void renderButton(SDL_Renderer* renderer, const Button* btn) {
	SDL_Color current_clr;

	current_clr = btn->default_clr;

	if (btn->pressed) current_clr = btn->pressed_clr;
	else if (btn->hovered)	current_clr = btn->hovered_clr;
	
	SDL_SetRenderDrawColor(renderer, current_clr.r, current_clr.g, current_clr.b, current_clr.a);

	SDL_RenderFillRect(renderer, &btn->box);
}

void createCounter(Counter* counter, size_t val, SDL_FRect box, SDL_Color bg_clr, SDL_Color default_clr, SDL_Color hovered_clr, SDL_Color pressed_clr) {
	SDL_FRect left_box, right_box;

	counter->main_box = box;
	counter->value = val;
	counter->bg_clr = bg_clr;


	left_box.x = box.x;
	left_box.y = box.y;
	left_box.w = box.h;
	left_box.h = box.h;

	right_box.x = box.x + box.w - box.h;
	right_box.y = box.y;
	right_box.w = box.h;
	right_box.h = box.h;

	createButton(&counter->decrement_btn, left_box, default_clr, hovered_clr, pressed_clr);
	createButton(&counter->increment_btn, right_box, default_clr, hovered_clr, pressed_clr);
}

void renderCounter(SDL_Renderer* renderer, TTF_TextEngine* text_engine, TTF_Font* font, const Counter* counter) {
	int pix_w, pix_h;
	char* text_value = NULL;
	SDL_FRect symbol_minus, symbol_plus_horizontal, symbol_plus_vertical;
	TTF_Text* ttf_text_val;

	text_value = SDL_malloc((intLen(counter->value) + 1));

	symbol_minus.x = counter->decrement_btn.box.x + 0.25 * counter->decrement_btn.box.w;
	symbol_minus.y = counter->decrement_btn.box.y + 0.45 * counter->decrement_btn.box.h;
	symbol_minus.w = 0.5 * counter->decrement_btn.box.w;
	symbol_minus.h = 0.1 * counter->decrement_btn.box.h;

	symbol_plus_horizontal.x = counter->increment_btn.box.x + 0.25 * counter->increment_btn.box.w;
	symbol_plus_horizontal.y = counter->increment_btn.box.y + 0.45 * counter->increment_btn.box.h;
	symbol_plus_horizontal.w = 0.5 * counter->increment_btn.box.w;
	symbol_plus_horizontal.h = 0.1 * counter->increment_btn.box.h;

	symbol_plus_vertical.x = counter->increment_btn.box.x + 0.45 * counter->increment_btn.box.w;
	symbol_plus_vertical.y = counter->increment_btn.box.y + 0.25 * counter->increment_btn.box.h;
	symbol_plus_vertical.w = 0.1 * counter->increment_btn.box.w;
	symbol_plus_vertical.h = 0.5 * counter->increment_btn.box.h;

	SDL_snprintf(text_value, (intLen(counter->value) + 1), "%lld", counter->value);
	ttf_text_val = TTF_CreateText(text_engine, font, text_value, (intLen(counter->value) + 1));
	TTF_GetTextSize(ttf_text_val, &pix_w, &pix_h);

	SDL_SetRenderDrawColor(renderer, counter->bg_clr.r, counter->bg_clr.g, counter->bg_clr.b, counter->bg_clr.a);
	SDL_RenderFillRect(renderer, &counter->main_box);

	renderButton(renderer, &counter->decrement_btn);
	renderButton(renderer, &counter->increment_btn);

	SDL_SetRenderDrawColor(renderer, CLR_BLACK.r, CLR_BLACK.g, CLR_BLACK.b, CLR_BLACK.a);
	SDL_RenderFillRect(renderer, &symbol_minus);
	SDL_RenderFillRect(renderer, &symbol_plus_horizontal);
	SDL_RenderFillRect(renderer, &symbol_plus_vertical);

	TTF_SetTextColor(ttf_text_val, CLR_BLACK.r, CLR_BLACK.g, CLR_BLACK.b, CLR_BLACK.a);
	TTF_DrawRendererText(ttf_text_val, counter->main_box.x + counter->main_box.w / 2 - pix_w / 2, counter->main_box.y + counter->main_box.h / 2 - pix_h / 2);

	SDL_free(text_value);
}
