#ifndef SETUP
#define SETUP

#include "globals.h"
#include "sorting_algorithms.h"

static SortData* main_arr = NULL;

static SortingState is_sorted = SORTING_CONTINUE;
static SortingState (*sorting_functions[N_ALGOS])(SortData*) = {sortInsertion, sortSelection, sortBubble, sortMerge};

static Button buttons[N_ALGOS];
static Button increment_delay_button, decrement_delay_button;

static short green_passing_index = -1;

static const char* str_sorting_functions[N_ALGOS] = {"Insertion Sort", "Selection Sort", "Bubble Sort", "Merge Sort"};
static char* font_path = NULL;
static char* algo_text_name = NULL;
static char* str_delay_text = NULL;
static char* str_sorting_interval = NULL;

static float mouse_x, mouse_y;

static bool sorting = false;
static bool element_hovered = false;

static uint64_t sorting_timer, green_timer;

static uint16_t algo_choosen = 0;
static uint16_t sorting_interval = 75;

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

static SDL_MouseButtonFlags mouse_data;
static SDL_Cursor* hand_cursor = NULL;
static SDL_Cursor* default_cursor = NULL;

static const SDL_Vertex increment_delay_button_triangle_vertices[3] = {
    {{1240.0f, 312.0f}, FDARK_GREY, FDEFAULT_POINT},
    {{1232.0f, 328.0f}, FDARK_GREY, FDEFAULT_POINT},
    {{1248.0f, 328.0f}, FDARK_GREY, FDEFAULT_POINT}
};

static const SDL_Vertex decrement_delay_button_triangle_vertices[3] = {
    {{1232.0f, 337.0f}, FDARK_GREY, FDEFAULT_POINT},
    {{1248.0f, 337.0f}, FDARK_GREY, FDEFAULT_POINT},
    {{1240.0f, 353.0f}, FDARK_GREY, FDEFAULT_POINT}
};

static TTF_TextEngine* text_engine = NULL;
static TTF_Font* font = NULL;
static TTF_Text* top_left_text = NULL;
static TTF_Text* delay_text = NULL;
static TTF_Text* buttons_text[N_ALGOS];

#endif
