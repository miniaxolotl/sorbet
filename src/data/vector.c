#include "vector.h"

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

VECTOR_T* vector__create(size_t inital_size)
{
	VECTOR_T* vector = malloc(sizeof(VECTOR_T));
	if(!vector)
	{
		return NULL;
	}

	vector->data = malloc(inital_size * sizeof(void*));
	if(!vector->data)
	{
		free(vector);
		return NULL;
	}

	vector->used = 0;
	vector->size = inital_size;

	return vector;
} // vector__create()

void vector__insert(VECTOR_T* vector, void* item)
{
	if(!vector) { return; }

	if (vector->used == vector->size)
	{
		vector->size *= 2;
		void** new_data = realloc(vector->data, vector->size * sizeof(void*));
		if(!new_data) { return; }
		vector->data = new_data;
	}

	vector->data[vector->used++] = item;
} // vector__insert()

void vector__insert_index(VECTOR_T* vector, void* item, size_t index)
{
	if(!vector) { return; }

	if (vector->used == vector->size)
	{
		vector->size *= 2;
		void** new_data = realloc(vector->data, vector->size * sizeof(void*));
		if(!new_data) { return; }
		vector->data = new_data;
	}

	if (index > vector->used)
	{
		index = vector->used;
	}

	if (index < vector->used)
	{
		memmove(&vector->data[index + 1], &vector->data[index],
			(vector->used - index) * sizeof(void*));
	}

	vector->data[index] = item;
	vector->used++;
} // vector__insert_index()

void* vector__get(VECTOR_T* vector, size_t index)
{
	if(index < vector->used)
	{
		return vector->data[index];
	}
	
	return NULL;
} // vector__get()

void* vector__remove(VECTOR_T* vector, size_t index)
{
	if(index < vector->used)
	{
		void* tmp = vector->data[index];
		memmove(&vector->data[index], &vector->data[index + 1],
			(vector->used - index - 1) * sizeof(void*));
		vector->used--;
		return tmp;
	}
	
	return NULL;
} // vector__remove()

void vector__free(VECTOR_T* vector)
{
	if(vector)
	{
		free(vector->data);
		vector->data = NULL;
		vector->used = vector->size = 0;
		free(vector);
	}
} // vector__free()