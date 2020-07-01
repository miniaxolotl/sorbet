#ifndef CORE_GAME_H
#define CORE_GAME_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "../types.h"
#include "../ecs/collection.h"

/*****************************************************
 * ENUMERATIONS
 *****************************************************/

/*****************************************************
 * TYPE DECLARATIONS
 *****************************************************/

/** core sorbet object */
typedef struct SORBET SORBET_T;
/** options for initializing sorbet object. */
typedef struct SORBET_OPTIONS SORBET_OPTIONS_T;

/*****************************************************
 * TYPE IMPLEMENTATIONS
 *****************************************************/

typedef struct SORBET
{
  SORBET_OPTIONS_T *options; // options for sorbet object
  SDL_Window *window;        // SDL2 window refrence
  SDL_Surface *surface;      // SDL2 surface refrence
  SDL_Renderer *renderer;    // SDL2 renderer refrence
  bool quit;                 // object quit flag
} SORBET_T;

typedef struct SORBET_OPTIONS
{
  COLLECTION_T *collection;
  char *title;     // window title.
  short width;     // window width.
  short height;    // window height.
  short framerate; // target frames per second.
  uint32_t flags;  // SDL2 init flags.
  bool debug;      // debugging & development flag.
  bool log;        // logging system flag.
} SORBET_OPTIONS_T;

/*****************************************************
 * FUNCTION DECLARATIONS
 *****************************************************/

/**
 * generate a blank sorbet object.
 * @returns an empty sorbet object.
 **/
SORBET_T *sorbet__create();

/**
 * initialize sorbet and all it's subsytems.
 * @param sorbet sorbet object to be initialized.
 * @param options options to apply to sorbet object.
 **/
bool sorbet__init(SORBET_T *sorbet, SORBET_OPTIONS_T *options);

/**
 * compute a tick (frame).
 * @param sorbet sorbet object to compute.
 */
void sorbet__tick(SORBET_T *sorbet, SDL_Event *event, SORBET_LENGTH_T delta);

/**
 * compute a tick (frame).
 * @param sorbet sorbet object to compute.
 */
void sorbet__run(SORBET_T *sorbet);

/**
 * exit sorbet and uninatialize all subsystems.
 * @param sorbet sorbet object to compute.
 */
void sorbet__exit(SORBET_T *sorbet);

/*****************************************************
 * FUNCTION IMPLEMENTATIONS
 *****************************************************/

/**
 * a custom tick function to run during game loop. (IMPLEMENTED BY DEVELOPER)
 * @param sorbet sorbet object to compute.
 */
extern void (*sorbet__custom_tick)(SORBET_T *sorbet,
                                   SDL_Event *event, SORBET_LENGTH_T delta);

#endif