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
TTF_Text* ttf_algo_text_name = NULL;
TTF_Text* ttf_delay_text = NULL;
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

    hand_cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_POINTER);
	checkAllocation(hand_cursor);
	default_cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_DEFAULT);
	checkAllocation(default_cursor);

	setupArr(&main_arr, DEFAULT_ARR_SIZE);

	sorting_timer = SDL_GetTicks();
	green_timer = SDL_GetTicks();

    return true;
}

void initializeTextEngineTTF(void) {
    size_t strlen_sorting_function0, strlen_sorting_interval, strlen_delay_text_begin, strlen_delay_text_end;

	str_sorting_interval = SDL_malloc(intLen(sorting_interval) + 1);
	checkAllocation(str_sorting_interval);
	SDL_snprintf(str_sorting_interval, intLen(sorting_interval) + 1, "%d", sorting_interval);

    strlen_sorting_function0 = strlen(str_sorting_functions[0]);
    strlen_delay_text_begin = strlen("Delay : ");
    strlen_delay_text_end = strlen(" - Delay  ms");
    strlen_sorting_interval = strlen(str_sorting_interval);

	algo_text_name = SDL_malloc(strlen_sorting_function0 + strlen_delay_text_end + strlen_sorting_interval + 1);
	checkAllocation(algo_text_name);
	str_delay_text = SDL_malloc(strlen_delay_text_begin + strlen_sorting_interval + 1);
	checkAllocation(str_delay_text);
	SDL_snprintf(algo_text_name, strlen_sorting_function0 + strlen_delay_text_end + strlen_sorting_interval + 1, "%s - delay %s ms", str_sorting_functions[algo_choosen], str_sorting_interval);
	SDL_snprintf(str_delay_text, strlen_delay_text_begin + strlen_sorting_interval + 1, "Delay : %s", str_sorting_interval);

    SDL_asprintf(&font_path, "%s/../font/%s", SDL_GetBasePath(), "sans.ttf");
	font = TTF_OpenFont(font_path, 25);
	checkAllocation(font);

	text_engine = TTF_CreateRendererTextEngine(renderer);
	checkAllocation(text_engine);
	ttf_delay_text = TTF_CreateText(text_engine, font, str_delay_text, strlen(str_delay_text));
	checkAllocation(ttf_delay_text);
	ttf_algo_text_name = TTF_CreateText(text_engine, font, algo_text_name, strlen(algo_text_name));
	checkAllocation(ttf_algo_text_name);

	for (uint16_t i = 0; i < N_ALGOS; ++i) {
        buttons_text[i] = TTF_CreateText(text_engine, font, str_sorting_functions[i], strlen(str_sorting_functions[i]) + 1);
		TTF_SetTextColor(buttons_text[i], CLR_BLACK.r, CLR_BLACK.g, CLR_BLACK.b, CLR_BLACK.a);
    }

	TTF_SetTextColor(ttf_algo_text_name, CLR_WHITE.r, CLR_WHITE.g, CLR_WHITE.b, CLR_WHITE.a);
}

void initializeComponents(void) {
	createButton(&increment_delay_button, (SDL_FRect){1230.0f, 310.0f, 20.0f, 20.0f}, CLR_WHITE, CLR_LIGHT_GREY, CLR_GREY);
	createButton(&decrement_delay_button, (SDL_FRect){1230.0f, 335.0f, 20.0f, 20.0f}, CLR_WHITE, CLR_LIGHT_GREY, CLR_GREY);

	for (uint16_t i = 0; i < N_ALGOS; ++i) {
        createButton(&buttons[i], (SDL_FRect){1050.0f, 10 + 60.0f * i, 200.0f, 50.0f}, CLR_WHITE, CLR_LIGHT_GREY, CLR_GREY);
    }
}

void freeAll(void) {
    SDL_free(main_arr);
	main_arr = NULL;
	SDL_free(str_sorting_interval);
	str_sorting_interval = NULL;
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
    SDL_DestroyWindow(window);
    window = NULL;
	SDL_free(font_path);
	font_path = NULL;
	SDL_free(algo_text_name);
	algo_text_name = NULL;
	SDL_free(str_delay_text);
	str_delay_text = NULL;
	for (uint16_t i = 0; i < N_ALGOS; ++i) TTF_DestroyText(buttons_text[i]);
	TTF_DestroyGPUTextEngine(text_engine);
	TTF_DestroyText(ttf_algo_text_name);
    TTF_DestroyText(ttf_delay_text);
	TTF_Quit();

	SDL_QuitSubSystem(SDL_INIT_VIDEO);
	SDL_Quit();
}
