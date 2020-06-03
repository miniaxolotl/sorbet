#include "collection.h"
#include <stdlib.h>

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

/*****************************************************
 * FUNCTION IMPLEMENTATIONS
 *****************************************************/

COLLECTION_T* collection__create()
{
	COLLECTION_T* collection = malloc(sizeof(COLLECTION_T));
	if(!collection) { return NULL; }

	collection->system_count = 0;
	collection->system_list = vector__create(1);

	collection->entity_count = 0;
	collection->entity_list = vector__create(1);

	collection->id_count = 0;
	collection->id_pool = queue_create(ECS__ENTITIES_MAX);

	return collection;
} // collection__create()

bool collection__register_system(COLLECTION_T* collection, SYSTEM_T* system)
{
	if(!collection || !system) { return false; }

	vector__insert(collection->system_list, system);
	system->status = DISABLED;

	collection->system_count++;
	return true;
} // collection__register_system()

bool collection__register_entity(COLLECTION_T* collection, ENTITY_T* entity)
{
	if(!collection || !entity) { return false; }

	ID_T* entity_id = NULL;

	if(queue_empty(collection->id_pool)
		&& (collection->id_count < ECS__ENTITIES_MAX))
	{
		ID_T* new_id = malloc(sizeof(ID_T));
		if(!new_id) { return false; }

		*new_id = collection->id_count++;

		if(!queue_push(collection->id_pool, new_id))
		{
			free(new_id);
			return false;
		}
	}

	entity_id = queue_pop(collection->id_pool);

	if(entity_id)
	{
		vector__insert(collection->entity_list, entity);
		entity->status = DISABLED;
		entity->entity_id = entity_id;
		collection->entity_count++;

		return true;
	}

	return false;
} // collection__register_entity()

void collection__link_components(COLLECTION_T* collection)
{
	ENTITY_T* entity = NULL;
	COMPONENT_T* component = NULL;
	SYSTEM_T* system = NULL;

	for(size_t i = 0; i < collection->entity_list->used; i++)
	{
		entity = collection->entity_list->data[i];
		if(!entity) { continue; }

		for(size_t j = 0; j < entity->components->used; j++)
		{
			component = entity->components->data[j];
			if(!component) { continue; }

			for(size_t k = 0; k < collection->system_list->used; k++)
			{
				system = collection->system_list->data[k];
				if(system && system->system_id == component->system_id)
				{
					llist__push_head(system->components,
						*component->entity->entity_id, component);
				}
			}
		}
	}
} // collection__link_components()

void collection__flush_components(COLLECTION_T* collection)
{
	SYSTEM_T* system = NULL;
	for(size_t i = 0; i < collection->system_list->used; i++)
	{
		system = collection->system_list->data[i];
		if(system)
		{
			system__flush(system);
		}
	}
} // collection__flush_components()


void collection__tick(SORBET_T* sorbet, COLLECTION_T* collection,
	SDL_Event* event, SORBET_LENGTH_T delta)
{
	SYSTEM_T* system = NULL;
	LLIST_NODE_T* node = NULL;
	
	for(size_t i = 0; i < collection->system_count; i++)
	{
		system = collection->system_list->data[i];
		if(!system || system->status != ENABLED) { continue; }

		node = system->components->head;
		while(node)
		{
			system->func(sorbet, system->components, event,
				node->data, delta);
			node = node->next;
		}
	}
} // collection__tick()