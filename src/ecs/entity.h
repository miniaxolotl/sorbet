#ifndef ECS_ENTITY_H
#define ECS_ENTITY_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../data.h"
#include "../types.h"
#include "component.h"

typedef struct COMPONENT COMPONENT_T;
typedef struct SORBET SORBET_T;

/*****************************************************
 * ENUMERATIONS
 *****************************************************/

/*****************************************************
 * TYPE DECLARATIONS
 *****************************************************/

/** container for a entity */
typedef struct ENTITY ENTITY_T;

/*****************************************************
 * TYPE IMPLEMENTATIONS
 *****************************************************/

typedef struct ENTITY {
	/** id of the entity. */
	ID_T* entity_id;
	/** list of components attached to entity. */
	VECTOR_T* components;
	/** payload attached to entity. */
	void* payload;
	/** status of the entity. */
	STATUS_T status;
	/** update the object */
	void (*update)(SORBET_T* sorbet, void* payload, SORBET_LENGTH_T delta);
	/** render the object */
	void (*render)(SORBET_T* sorbet, void* payload, SORBET_LENGTH_T delta);
} ENTITY_T;

/*****************************************************
 * FUNCTION DECLARATIONS
 *****************************************************/

/*****************************************************
 * FUNCTION IMPLEMENTATIONS
 *****************************************************/

/**
 * create an empty entity with a specified payload.
 * @param payload payload to attach to entity.
 * @param update update function.
 * @param render render function.
 */
ENTITY_T* entity__create(void* payload, void (*update), void (*render));

/**
 * attach a component to an entity.
 * @param entity entity to modify.
 * @param component component to attach to entity.
 * @returns true if success false if not.
 */
bool entity__register_component(ENTITY_T* entity, COMPONENT_T* component);

#endif