#ifndef ECS_COLLECTION_H
#define ECS_COLLECTION_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <SDL2/SDL.h>

#include "../data.h"
#include "../types.h"
#include "system.h"

typedef struct SORBET SORBET_T;

/*****************************************************
 * ENUMERATIONS
 *****************************************************/

/*****************************************************
 * TYPE DECLARATIONS
 *****************************************************/

/*****************************************************
 * TYPE IMPLEMENTATIONS
 *****************************************************/

typedef struct COLLECTION
{
  size_t system_count;
  VECTOR_T *system_list;
  size_t entity_count;
  VECTOR_T *entity_list;
  size_t id_count;
  QUEUE_T *id_pool;
} COLLECTION_T;

/*****************************************************
 * FUNCTION DECLARATIONS
 *****************************************************/

/**
 * creates a blank collection.
 * @returns a blank collection.
 */
COLLECTION_T *collection__create();

/**
 * register a system into the collection.
 * @param collection the collection to modify.
 * @param system the system to link.
 */
bool collection__register_system(COLLECTION_T *collection, SYSTEM_T *system);

/**
 * register a entity into the collection.
 * @param collection the collection to modify.
 * @param entity the entity to link.
 */
bool collection__register_entity(COLLECTION_T *collection, ENTITY_T *entity);

/**
 * link the components of every entity onto their corresponding system.
 * @param collection the collection to modify.
 */
void collection__link_components(COLLECTION_T *collection);

/**
 * remove all components from every system.
 * @param collection the collection to modify.
 */
void collection__flush_components(COLLECTION_T *collection);

/**
 * process a tick on every single system.
 * @param collection the collection to process.
 * @param event event to pass to tick.
 * @param delta delta between last frame.
 */
void collection__tick(SORBET_T *sorbet, COLLECTION_T *collection,
                      SDL_Event *event, SORBET_LENGTH_T delta);

/**
 * destroy a collection and all it's entities and systems.
 * @param collection collection to destroy.
 */
void collection__destroy(COLLECTION_T *collection);

/*****************************************************
 * FUNCTION IMPLEMENTATIONS
 *****************************************************/

#endif