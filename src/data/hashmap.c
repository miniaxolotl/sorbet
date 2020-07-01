#include "hashmap.h"
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

HASHMAP_T* hashmap__create(size_t initial_size)
{
	HASHMAP_T* hashtable = malloc(sizeof(HASHMAP_T));
	if(!hashtable)
	{
		return NULL;
	}

	hashtable->data = vector__create(initial_size);
	hashtable->used = 0;
	hashtable->size = initial_size;

	return hashtable;
} // hashmap__create()

size_t hashmap__code(HASHMAP_T* hashtable, size_t key)
{
   return key % hashtable->size;
} // hashmap__code()

HASHMAP_ITEM_T* hashmap__search(HASHMAP_T* hashtable, size_t key)
{
	size_t index = hashmap__code(hashtable, key);  

	while(hashtable->data->data[index] != NULL)
	{
		if(((HASHMAP_ITEM_T*)hashtable->data->data[index])->key == key)
			return (HASHMAP_ITEM_T*)hashtable->data->data[index]; 
			
		++index;
		index %= hashtable->size;
	}        

	return NULL; 
} // hashmap__search()

void hashmap__insert(HASHMAP_T* hashtable, size_t key, void* data)
{
	HASHMAP_ITEM_T* item = malloc(sizeof(HASHMAP_ITEM_T));
	if(!item) { return; }

	item->data = data;  
	item->key = key;

	size_t index = hashmap__code(hashtable, key);

	while(hashtable->data->data[index] != NULL)
	{
		++index;
		index %= hashtable->size;
	}
		
	hashtable->data->data[index] = item;
	hashtable->used++;
} // hashmap__insert()

HASHMAP_ITEM_T* hashmap__remove(HASHMAP_T* hashtable, size_t key)
{
	size_t index = hashmap__code(hashtable, key);

	while(hashtable->data->data[index] != NULL)
	{
		if(((HASHMAP_ITEM_T*)hashtable->data->data[index])->key == key)
		{
			HASHMAP_ITEM_T* temp = hashtable->data->data[index]; 
				
			hashtable->data->data[index] = NULL; 
			hashtable->used--;
			return temp;
		}
			
		++index;
		index %= hashtable->size;
	}      
		
	return NULL;
} // hashmap__remove()

void hashmap__free(HASHMAP_T* hashtable)
{
	if(hashtable)
	{
		vector__free(hashtable->data);
		free(hashtable);
	}
} // hashmap__free()