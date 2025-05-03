#ifndef SETUP
#define SETUP

#include "components.h"
#include "globals.h"
#include "rendering.h"
#include "sorting_algorithms.h"
#include "utils.h"

extern SortData* main_arr;

extern SortingState is_sorted;
extern SortingState (*sorting_functions[N_ALGOS])(SortData*);

extern Button buttons[N_ALGOS];

extern Counter delay_counter;
extern Counter arr_sz_counter;
extern Counter counters[N_COUNTER];

extern short green_passing_index;

extern const char* str_sorting_functions[N_ALGOS];
extern char *font_path, *algo_text_name, *str_delay_text, *str_sorting_interval;

extern float mouse_x, mouse_y, bar_width, bar_height;

extern bool sorting;
extern bool element_hovered;

extern uint64_t sorting_timer, green_timer;

extern uint16_t algo_choosen;
extern uint16_t sorting_interval;

extern SDL_Window* window;
extern SDL_Renderer* renderer;

extern SDL_MouseButtonFlags mouse_data;
extern SDL_Cursor *hand_cursor, *default_cursor;

extern const SDL_Vertex increment_delay_button_triangle_vertices[3], decrement_delay_button_triangle_vertices[3];

extern TTF_TextEngine* text_engine;
extern TTF_Font* font;
extern TTF_Text* ttf_algo_text_name;
extern TTF_Text* ttf_delay_text;
extern TTF_Text* buttons_text[N_ALGOS];

bool initializeSDL(void);
void initializeTextEngineTTF(void);
void initializeComponents(void);
void freeAll(void);

#endif
