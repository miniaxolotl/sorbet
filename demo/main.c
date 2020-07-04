#include "../src/sorbet.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>

/* --------------------------------------------------------------------
 * Game Constants
 * -------------------------------------------------------------------- */
#define GRID_W        30
#define GRID_H        20
#define CELL_SIZE     24
#define WIN_W         (GRID_W * CELL_SIZE)
#define WIN_H         (GRID_H * CELL_SIZE)
#define MAX_SNAKE_LEN (GRID_W * GRID_H)
#define SYSTEM_ID     0

/* --------------------------------------------------------------------
 * Types
 * -------------------------------------------------------------------- */
typedef struct { int x, y; } Vec2;

typedef struct {
	Vec2  body[MAX_SNAKE_LEN];
	int   length;
	int   dir;           /* 0=up, 1=right, 2=down, 3=left */
	int   next_dir;
	float move_timer;
	float move_interval;
	bool  dead;
	int   score;
	bool  grow;
} SnakeState;

typedef struct {
	int  x, y;
	bool active;
} FoodState;

typedef struct {
	SnakeState snake;
	FoodState  food;
	bool       running;
} GameState;

/* --------------------------------------------------------------------
 * Helpers
 * -------------------------------------------------------------------- */
static void spawn_food(GameState *game)
{
	bool valid;
	do {
		valid = true;
		game->food.x = rand() % GRID_W;
		game->food.y = rand() % GRID_H;
		for (int i = 0; i < game->snake.length; i++) {
			if (game->snake.body[i].x == game->food.x &&
			    game->snake.body[i].y == game->food.y) {
				valid = false;
				break;
			}
		}
	} while (!valid);
	game->food.active = true;
}

static void reset_game(GameState *game)
{
	memset(game, 0, sizeof(GameState));
	game->snake.length = 3;
	game->snake.body[0] = (Vec2){ GRID_W / 2,     GRID_H / 2 };
	game->snake.body[1] = (Vec2){ GRID_W / 2 - 1, GRID_H / 2 };
	game->snake.body[2] = (Vec2){ GRID_W / 2 - 2, GRID_H / 2 };
	game->snake.dir = 1;
	game->snake.next_dir = 1;
	game->snake.move_interval = 0.15f;
	game->running = true;
	spawn_food(game);
}

/* --------------------------------------------------------------------
 * ECS System
 * -------------------------------------------------------------------- */
static void game_system(SORBET_T *sorbet, LLIST_T *components,
                        SDL_Event *event, COMPONENT_T *component,
                        SORBET_LENGTH_T delta)
{
	(void)components;
	(void)event;

	GameState *game = (GameState *)component->payload;
	if (!game) return;

	/* Restart on space if dead */
	if (!game->running) {
		const Uint8 *keys = SDL_GetKeyboardState(NULL);
		if (keys[SDL_SCANCODE_SPACE]) {
			reset_game(game);
		}
		/* Draw game-over screen */
		SDL_SetRenderDrawColor(sorbet->renderer, 20, 20, 30, 255);
		SDL_RenderClear(sorbet->renderer);

		/* Simple "GAME OVER" text using rectangles (no TTF for simplicity) */
		SDL_SetRenderDrawColor(sorbet->renderer, 220, 60, 60, 255);
		SDL_Rect go = { WIN_W / 2 - 60, WIN_H / 2 - 20, 120, 40 };
		SDL_RenderFillRect(sorbet->renderer, &go);
		return;
	}

	/* ---- Input ---- */
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	if (keys[SDL_SCANCODE_UP]    && game->snake.dir != 2)
		game->snake.next_dir = 0;
	else if (keys[SDL_SCANCODE_RIGHT] && game->snake.dir != 3)
		game->snake.next_dir = 1;
	else if (keys[SDL_SCANCODE_DOWN]  && game->snake.dir != 0)
		game->snake.next_dir = 2;
	else if (keys[SDL_SCANCODE_LEFT]  && game->snake.dir != 1)
		game->snake.next_dir = 3;

	/* ---- Movement ---- */
	game->snake.move_timer += (float)delta;
	if (game->snake.move_timer >= game->snake.move_interval) {
		game->snake.move_timer -= game->snake.move_interval;
		game->snake.dir = game->snake.next_dir;

		Vec2 head = game->snake.body[0];
		switch (game->snake.dir) {
			case 0: head.y--; break;
			case 1: head.x++; break;
			case 2: head.y++; break;
			case 3: head.x--; break;
		}

		/* Wrap around */
		if (head.x < 0)       head.x = GRID_W - 1;
		if (head.x >= GRID_W) head.x = 0;
		if (head.y < 0)       head.y = GRID_H - 1;
		if (head.y >= GRID_H) head.y = 0;

		/* Self collision */
		for (int i = 0; i < game->snake.length; i++) {
			if (game->snake.body[i].x == head.x &&
			    game->snake.body[i].y == head.y) {
				game->snake.dead = true;
				game->running = false;
				return;
			}
		}

		/* Shift body */
		for (int i = game->snake.length; i > 0; i--)
			game->snake.body[i] = game->snake.body[i - 1];
		game->snake.body[0] = head;

		if (game->snake.grow) {
			game->snake.length++;
			game->snake.grow = false;
		}

		/* Eat food */
		if (head.x == game->food.x && head.y == game->food.y) {
			game->snake.grow = true;
			game->snake.score++;
			if (game->snake.move_interval > 0.05f)
				game->snake.move_interval *= 0.97f;
			spawn_food(game);
		}
	}

	/* ---- Rendering ---- */
	SDL_SetRenderDrawColor(sorbet->renderer, 20, 20, 30, 255);
	SDL_RenderClear(sorbet->renderer);

	/* Grid lines */
	SDL_SetRenderDrawColor(sorbet->renderer, 40, 40, 50, 255);
	for (int x = 0; x <= GRID_W; x++)
		SDL_RenderDrawLine(sorbet->renderer, x * CELL_SIZE, 0,
		                   x * CELL_SIZE, WIN_H);
	for (int y = 0; y <= GRID_H; y++)
		SDL_RenderDrawLine(sorbet->renderer, 0, y * CELL_SIZE,
		                   WIN_W, y * CELL_SIZE);

	/* Food */
	SDL_Rect food_rect = {
		game->food.x * CELL_SIZE + 4,
		game->food.y * CELL_SIZE + 4,
		CELL_SIZE - 8,
		CELL_SIZE - 8
	};
	SDL_SetRenderDrawColor(sorbet->renderer, 220, 60, 60, 255);
	SDL_RenderFillRect(sorbet->renderer, &food_rect);

	/* Snake */
	for (int i = 0; i < game->snake.length; i++) {
		SDL_Rect seg = {
			game->snake.body[i].x * CELL_SIZE + 2,
			game->snake.body[i].y * CELL_SIZE + 2,
			CELL_SIZE - 4,
			CELL_SIZE - 4
		};
		if (i == 0)
			SDL_SetRenderDrawColor(sorbet->renderer, 80, 220, 80, 255);
		else
			SDL_SetRenderDrawColor(sorbet->renderer, 50, 180, 50, 255);
		SDL_RenderFillRect(sorbet->renderer, &seg);
	}
}

/* --------------------------------------------------------------------
 * Main
 * -------------------------------------------------------------------- */
int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;

	srand((unsigned)time(NULL));

	/* Game state lives on the stack; the ECS holds a pointer to it */
	GameState game_state;
	reset_game(&game_state);

	/* Build ECS world */
	COLLECTION_T *collection = collection__create();

	SYSTEM_T *game_sys = system__create(game_system, SYSTEM_ID);
	collection__register_system(collection, game_sys);

	ENTITY_T *game_entity = entity__create(NULL, NULL, NULL);
	collection__register_entity(collection, game_entity);

	COMPONENT_T *game_comp = component__create(&game_state, SYSTEM_ID);
	entity__register_component(game_entity, game_comp);

	collection__link_components(collection);

	/* Sorbet options */
	SORBET_OPTIONS_T options = {
		.collection = collection,
		.title      = "Snake Demo",
		.width      = WIN_W,
		.height     = WIN_H,
		.framerate  = 60,
		.flags      = SDL_WINDOW_SHOWN,
		.debug      = false,
		.log        = false
	};

	SORBET_T *sorbet = sorbet__create();
	if (!sorbet || !sorbet__init(sorbet, &options)) {
		fprintf(stderr, "Failed to initialize sorbet.\n");
		return 1;
	}

	sorbet__run(sorbet);

	/* Cleanup */
	collection__destroy(collection);

	return 0;
}
