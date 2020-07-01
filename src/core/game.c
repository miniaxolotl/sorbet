#include "game.h"
#include "../ecs.h"

#include <stdlib.h>
#include <SDL2/SDL_timer.h>

/*****************************************************
 * ENUMERATIONS
 *****************************************************/

/*****************************************************
 * TYPE DECLARATIONS
 *****************************************************/

/*****************************************************
 * TYPE IMPLEMENTATIONS
 *****************************************************/

/*****************************************************
 * FUNCTION DECLARATIONS
 *****************************************************/

void (*sorbet__custom_tick)(SORBET_T* sorbet,
	SDL_Event* event, SORBET_LENGTH_T delta) = NULL;

/*****************************************************
 * FUNCTION IMPLEMENTATIONS
 *****************************************************/

SORBET_T* sorbet__create()
{
	SORBET_T* sorbet = malloc(sizeof(SORBET_T));
	if(!sorbet)
	{
		return NULL;
	}

	sorbet->options = NULL;
	sorbet->window = NULL;
	sorbet->surface = NULL;
	sorbet->renderer = NULL;
	sorbet->quit = false;

	return sorbet;
} // SORBET_T* sorbet__create()

bool sorbet__init(SORBET_T* sorbet, SORBET_OPTIONS_T *options)
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("SDL2 could not initalize! Error: %s\n", SDL_GetError());
		return false;
    }

    sorbet->window = SDL_CreateWindow(options->title,
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		options->width, options->height, options->flags);
    if(!sorbet->window)
    {
        printf("Window could not be created! Error: %s\n", SDL_GetError());
		return false;
	}

	sorbet->renderer = SDL_CreateRenderer(sorbet->window, -1,
		SDL_RENDERER_ACCELERATED);
	if(!sorbet->renderer)
	{
		printf("Renderer could not initalize! Error: %s\n",
			SDL_GetError());
		return false;
	}

	sorbet->surface = SDL_GetWindowSurface(sorbet->window);
	if(!sorbet->surface)
	{
		printf("Surface could not initalize! Error: %s\n",
			SDL_GetError());
		return false;
	}
	
	int img_flags = IMG_INIT_PNG;
	if(!(IMG_Init(img_flags) & img_flags))
	{
		printf("SDL_image could not initialize! Error: %s\n",
			IMG_GetError());
		return false;
	}

	if(TTF_Init() == -1)
	{
		printf("SDL_ttf could not initialize! Error: %s\n",
			TTF_GetError());
		return false;
	}

	sorbet->options = options;
	return true;
} // sorbet__init()

void sorbet__tick(SORBET_T* sorbet, SDL_Event* event, SORBET_LENGTH_T delta)
{
	if(!sorbet || !sorbet->options || !sorbet->renderer)
	{
		return;
	}

	COLLECTION_T* collection = sorbet->options->collection;

	SDL_SetRenderDrawColor(sorbet->renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderClear(sorbet->renderer);

	while(SDL_PollEvent(event))
	{
		switch (event->type)
		{
			case SDL_QUIT:
				sorbet->quit = true;
				break;
			default:
				break;
		}
	}

	if(collection)
	{
		collection__tick(sorbet, collection, event, delta);

		for(size_t i = 0; i < collection->entity_count; i++)
		{
			ENTITY_T* entity = collection->entity_list->data[i];
			if(entity && entity->update)
			{
				entity->update(sorbet, entity->payload, delta);
			}
		}

		for(size_t i = 0; i < collection->entity_count; i++)
		{
			ENTITY_T* entity = collection->entity_list->data[i];
			if(entity && entity->render)
			{
				entity->render(sorbet, entity->payload, delta);
			}
		}
	}

	if(sorbet__custom_tick)
	{
		sorbet__custom_tick(sorbet, event, delta);
	}

	SDL_RenderPresent(sorbet->renderer);
} // sorbet__tick()

void sorbet__run(SORBET_T* sorbet)
{
	SDL_Event event;
	SORBET_LENGTH_T TARGET_TICKS_PER_FRAME = 0;
	SORBET_LENGTH_T TIME = 0, LAST = 0, DELTA = 0;

	if(!sorbet->options || sorbet->options->framerate <= 0)
	{
		printf("Invalid framerate or options.\n");
		return;
	}

	while (!sorbet->quit)
	{
		SORBET_LENGTH_T FRAMERATE = sorbet->options->framerate;
		TARGET_TICKS_PER_FRAME = 1000 / FRAMERATE;

		while(!SDL_TICKS_PASSED(SDL_GetTicks(), LAST + TARGET_TICKS_PER_FRAME));

		TIME = SDL_GetTicks();
		DELTA = (TIME - LAST) > (FRAMERATE * 3)
			? (FRAMERATE * 3) / 1000.0 : (TIME - LAST) / 1000.0;

		sorbet__tick(sorbet, &event, DELTA);

		LAST = TIME;
	}
	
	sorbet__exit(sorbet);
} // void sorbet__run()

void sorbet__exit(SORBET_T* sorbet)
{
	// destroy renderer
	if(sorbet->renderer)
	{
		SDL_DestroyRenderer(sorbet->renderer);
		sorbet->renderer = NULL;
	}

	// destroy window (also frees the window surface)
	if(sorbet->window)
	{
		SDL_DestroyWindow(sorbet->window);
		sorbet->window = NULL;
	}
	sorbet->surface = NULL;

    TTF_Quit();
    IMG_Quit();
 
	// quit SDL subsystems
	SDL_Quit();

	free(sorbet);
} // void sorbet__exit()
