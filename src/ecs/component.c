#include "component.h"
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

COMPONENT_T *component__create(void *payload, ID_T system_id)
{
  COMPONENT_T *component = malloc(sizeof(COMPONENT_T));
  if (!component)
  {
    return NULL;
  }

  component->system_id = system_id;
  component->entity = NULL;
  component->payload = payload;
  component->status = UNLINKED;

  return component;
}

void component__destroy(COMPONENT_T *component)
{
  free(component);
} // component__destroy()