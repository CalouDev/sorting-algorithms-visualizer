#include "../include/setup.h"

SortData* main_arr = NULL;

SortingState is_sorted = SORTING_CONTINUE;
SortingState (*sorting_functions[N_ALGOS])(SortData*) = {sortInsertion, sortSelection, sortBubble, sortMerge};

Button buttons[N_ALGOS];
Button increment_delay_button, decrement_delay_button;

short green_passing_index = -1;

const char* str_sorting_functions[N_ALGOS] = {"Insertion Sort", "Selection Sort", "Bubble Sort", "Merge Sort"};
char* font_path = NULL;
char* algo_text_name = NULL;
char* str_delay_text = NULL;
char* str_sorting_interval = NULL;

float mouse_x, mouse_y;

bool sorting = false;
bool element_hovered = false;

uint64_t sorting_timer, green_timer;

uint16_t algo_choosen = 0;
uint16_t sorting_interval = 75;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

SDL_MouseButtonFlags mouse_data;
SDL_Cursor* hand_cursor = NULL;
SDL_Cursor* default_cursor = NULL;

const SDL_Vertex increment_delay_button_triangle_vertices[3] = {
    {{1240.0f, 312.0f}, FDARK_GREY, FDEFAULT_POINT},
    {{1232.0f, 328.0f}, FDARK_GREY, FDEFAULT_POINT},
    {{1248.0f, 328.0f}, FDARK_GREY, FDEFAULT_POINT}
};

const SDL_Vertex decrement_delay_button_triangle_vertices[3] = {
    {{1232.0f, 337.0f}, FDARK_GREY, FDEFAULT_POINT},
    {{1248.0f, 337.0f}, FDARK_GREY, FDEFAULT_POINT},
    {{1240.0f, 353.0f}, FDARK_GREY, FDEFAULT_POINT}
};

TTF_TextEngine* text_engine = NULL;
TTF_Font* font = NULL;
TTF_Text* top_left_text = NULL;
TTF_Text* delay_text = NULL;
TTF_Text* buttons_text[N_ALGOS];

bool initializeSDL(void) {
    SDL_srand(time(NULL));

	SDL_SetAppMetadata("Sorting Algorithms Visualizer", "0.7", NULL);
	SDL_SetAppMetadataProperty("SDL_PROP_APP_METADATA_NAME_STRING", "Sorting Algorithms Visualizer");
	SDL_SetAppMetadataProperty("SDL_PROP_APP_METADATA_VERSION_STRING", "13/04/2025");
	SDL_SetAppMetadataProperty("SDL_PROP_APP_METADATA_CREATOR_STRING", "CalouDev");
	
    if (!(SDL_Init(SDL_INIT_VIDEO) && SDL_Init(SDL_INIT_AUDIO) && TTF_Init())) return false;

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

	sorting_timer = SDL_GetTicks();
	green_timer = SDL_GetTicks();

    return true;
}