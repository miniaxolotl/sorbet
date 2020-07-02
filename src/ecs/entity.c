#include "entity.h"
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

ENTITY_T *entity__create(void *payload,
                         void (*update)(SORBET_T *sorbet, void *payload, SORBET_LENGTH_T delta),
                         void (*render)(SORBET_T *sorbet, void *payload, SORBET_LENGTH_T delta))
{
  ENTITY_T *entity = malloc(sizeof(ENTITY_T));
  if (!entity)
  {
    return NULL;
  }

  entity->entity_id = NULL;
  entity->components = vector__create(1);
  entity->payload = payload;
  entity->status = UNLINKED;
  entity->update = update;
  entity->render = render;

  return entity;
} // entity__create()

bool entity__register_component(ENTITY_T *entity, COMPONENT_T *component)
{
  if (!entity || !component)
  {
    return false;
  }
  component->entity = entity;
  vector__insert(entity->components, component);
  return true;
} // entity__register_component()

void entity__destroy(ENTITY_T *entity)
{
  if (!entity)
  {
    return;
  }
  if (entity->entity_id)
  {
    free(entity->entity_id);
  }
  if (entity->components)
  {
    for (size_t i = 0; i < entity->components->used; i++)
    {
      COMPONENT_T *comp = (COMPONENT_T *)entity->components->data[i];
      if (comp)
        component__destroy(comp);
    }
    vector__free(entity->components);
  }
  free(entity);
} // entity__destroy()