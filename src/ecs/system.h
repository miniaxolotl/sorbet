#ifndef ECS_SYSTEM_H
#define ECS_SYSTEM_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../data.h"
#include "../types.h"
#include "component.h"

/*****************************************************
 * ENUMERATIONS
 *****************************************************/

/*****************************************************
 * TYPE DECLARATIONS
 *****************************************************/

/** container for a system */
typedef struct SYSTEM SYSTEM_T;

/*****************************************************
 * TYPE IMPLEMENTATIONS
 *****************************************************/

typedef struct SYSTEM
{
  ID_T system_id;
  LLIST_T *components;
  STATUS_T status;
  void (*func)(SORBET_T *sorbet, LLIST_T *components, SDL_Event *event,
               COMPONENT_T *component, SORBET_LENGTH_T delta);
} SYSTEM_T;

/*****************************************************
 * FUNCTION DECLARATIONS
 *****************************************************/

/*****************************************************
 * FUNCTION IMPLEMENTATIONS
 *****************************************************/

/**
 * create a blank system container.
 * @param func the system logic function.
 * @param system_id identifier code of the system.
 * @returns a generated system.
 */
SYSTEM_T *system__create(void(*func), ID_T system_id);

/**
 * attach a component to system.
 * @param system system to modify.
 * @param component component to attach.
 * @returns true if success false if not.
 */
bool system__register_component(SYSTEM_T *system, COMPONENT_T *component);

/**
 * fluash (expunge) all components from a system.
 * @param system system to modify.
 */
void system__flush(SYSTEM_T *system);

#endif