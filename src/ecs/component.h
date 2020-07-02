#ifndef ECS_COMPONENT_H
#define ECS_COMPONENT_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../data.h"
#include "../types.h"
#include "entity.h"

typedef struct ENTITY ENTITY_T;

/*****************************************************
 * ENUMERATIONS
 *****************************************************/

/*****************************************************
 * TYPE DECLARATIONS
 *****************************************************/

/** container for a entity component */
typedef struct COMPONENT COMPONENT_T;

/*****************************************************
 * TYPE IMPLEMENTATIONS
 *****************************************************/

typedef struct COMPONENT
{
  /** id of the system the payload belongs to. */
  ID_T system_id;
  /** the entity the payload belong to. */
  ENTITY_T *entity;
  /** payload attached to component. */
  void *payload;
  /** status of the component. */
  STATUS_T status;
} COMPONENT_T;

/*****************************************************
 * FUNCTION DECLARATIONS
 *****************************************************/

/*****************************************************
 * FUNCTION IMPLEMENTATIONS
 *****************************************************/

/**
 * create a component.
 * @param payload data to attach to component.
 * @param system_id system the component belongs to.
 * @returns returns a generated component.
 */
COMPONENT_T *component__create(void *payload, ID_T system_id);

/**
 * destroy a component and free it's resources.
 * @param component component to destroy.
 */
void component__destroy(COMPONENT_T *component);

#endif