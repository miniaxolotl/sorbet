#include "system.h"

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

SYSTEM_T* system__create(void (*func), ID_T system_id)
{
	SYSTEM_T* system = malloc(sizeof(SYSTEM_T));
	if(!system) { return NULL; }

	system->system_id = system_id;
	system->components = llist__create();
	system->status = UNLINKED;
	system->func = func;

	return system;
} // system__create()

bool system__register_component(SYSTEM_T* system, COMPONENT_T* component)
{
	component->status = ENABLED;
	llist__push_head(system->components, *component->entity->entity_id,
		component);
	return true;
} // system__register_component()

void system__flush(SYSTEM_T* system)
{
	LLIST_NODE_T* head = llist__pop_all(system->components);
	LLIST_NODE_T* node = head;
	while(node)
	{
		COMPONENT_T* component = (COMPONENT_T*) node->data;
		component->status = UNLINKED;

		LLIST_NODE_T* next = node->next;
		free(node);
		node = next;
	}
} // system__flush()