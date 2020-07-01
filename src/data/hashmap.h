#ifndef DATA_HASHMAP_H
#define DATA_HASHMAP_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "vector.h"

/*****************************************************
 * ENUMERATIONS
 *****************************************************/

/*****************************************************
 * TYPE DECLARATIONS
 *****************************************************/

/** an item in a hashmap. */
typedef struct HASHMAP_ITEM HASHMAP_ITEM_T;

/** container for a hashtable. */
typedef struct HASHMAP HASHMAP_T;

/*****************************************************
 * TYPE IMPLEMENTATIONS
 *****************************************************/

typedef struct HASHMAP_ITEM {
	void* data; // payload carried by item.
	size_t key; // access key for item.
} HASHMAP_ITEM_T;

typedef struct HASHMAP {
	VECTOR_T* data; // hashtable is represented as vector.
	size_t used; // used indexes of hashtable.
	size_t size; // size of hashtable.
} HASHMAP_T;

/*****************************************************
 * FUNCTION DECLARATIONS
 *****************************************************/

/** 
 * create an empty hashtable.
 * @param initial_size initial size of hashtable.
 * @returns an empty hashtable.
 **/
HASHMAP_T* hashmap__create(size_t initial_size);

/**
 * get an index code from a key value.
 * @param hashtable the table to refrence from.
 * @param key the key to generate and index from.
 * @returns the index of the key.
 **/
size_t hashmap__code(HASHMAP_T* hashtable, size_t key);

/**
 * get an item from a key value.
 * @param hashtable the table to refrence from.
 * @param key the key of the item to find.
 * @returns the item at the specified index.
 **/
HASHMAP_ITEM_T* hashmap__search(HASHMAP_T* hashtable, size_t key);

/**
 * insert an item into a hashtable.
 * @param hashtable the table to insert into.
 * @param key the key of the item.
 * @param item item to insert into hashtable.
 **/
void hashmap__insert(HASHMAP_T* hashtable, size_t key, void* item);

/**
 * remove an item from a hashtable.
 * @param hashtable the table to remove from.
 * @param key the key of the item.
 * @returns the item at the specified index.
 **/
HASHMAP_ITEM_T* hashmap__remove(HASHMAP_T* hashtable, size_t key);

/**
 * free a hashtable from memory (if it exists).
 * @param hashtable the hashtable to free.
 **/
void hashmap__free(HASHMAP_T* hashtable);

/*****************************************************
 * FUNCTION IMPLEMENTATIONS
 *****************************************************/

#endif