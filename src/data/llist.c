#include "llist.h"

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

LLIST_T* llist__create()
{
	LLIST_T* llist = malloc(sizeof(LLIST_T));
	if(!llist) { return NULL; }

	llist->head = NULL;
	llist->tail = NULL;
	llist->size = 0;

	return llist;
} // llist__create

void llist__push_head(LLIST_T* llist, size_t key, void* item)
{
	LLIST_NODE_T* node = malloc(sizeof(LLIST_NODE_T));
	if(!node) { return; }

	node->next = llist->head;
	node->back = NULL;

	node->key = key;
	node->data = item;

	if(llist->head)
	{
		llist->head->back = node;
		llist->head = node;
	}
	else
	{
		llist->tail = node;
		llist->head = node;
	}
	++llist->size;
} // llist__push_head()

void llist__push_tail(LLIST_T* llist, size_t key, void* item)
{
	LLIST_NODE_T* node = malloc(sizeof(LLIST_NODE_T));
	if(!node) { return; }

	node->back = llist->tail;
	node->next = NULL;
	
	node->key = key;
	node->data = item;

	if(llist->tail)
	{
		llist->tail->next = node;
		llist->tail = node;
	}
	else
	{
		llist->tail = node;
		llist->head = node;
	}
	++llist->size;
} // llist__push_tail()

void llist__push_node_head(LLIST_T* llist, LLIST_NODE_T* node)
{
	if(llist->head)
	{
		llist->head->back = node;
		node->next = llist->head;
		llist->head = node;
		node->back = NULL;
		++llist->size;

		return;
	}

	llist->tail = node;
	llist->head = node;

	node->back = NULL;
	node->next = NULL;
	++llist->size;

	return;
} // llist__push_node_head()

void llist__push_node_tail(LLIST_T* llist, LLIST_NODE_T* node)
{
	if(llist->tail)
	{
		llist->tail->next = node;
		node->back = llist->tail;
		llist->tail = node;
		node->next = NULL;
		++llist->size;

		return;
	}

	llist->tail = node;
	llist->head = node;

	node->back = NULL;
	node->next = NULL;
	++llist->size;

	return;
} // llist__push_node_tail

LLIST_NODE_T* llist__pop(LLIST_T* llist, size_t key)
{
	LLIST_NODE_T* node = llist->head;

	while(node && !(node->key == key))
	{
		node = node->next;
	}

	if(node)
	{
		if(node->back)
		{
			node->back->next = node->next;
		}
		else
		{
			llist->head = node->next;
		}

		if(node->next)
		{
			node->next->back = node->back;
		}
		else
		{
			llist->tail = node->back;
		}

		node->back = NULL;
		node->next = NULL;
		--llist->size;
	}
	
	return node;
} // llist__pop()

LLIST_NODE_T* llist__pop_all(LLIST_T* llist)
{
	LLIST_NODE_T* node = llist->head;

	llist->head = NULL;
	llist->tail = NULL;
	llist->size = 0;
	
	return node;
} // llist__pop_all()

LLIST_NODE_T* llist__get(LLIST_T* llist, size_t key)
{
	LLIST_NODE_T* node = llist->head;

	while(node && !(node->key == key))
	{
		node = node->next;
	}

	return node;
} // llist__get()

LLIST_NODE_T* llist__peek_head(LLIST_T* llist)
{
	return llist->head;
} // llist__peek_head()

LLIST_NODE_T* llist__peek_tail(LLIST_T* llist)
{
	return llist->tail;
} // llist__peek_tail()

LLIST_NODE_T* llist__cycle(LLIST_T* llist)
{
	LLIST_NODE_T* node = llist__pop(llist, llist->head->key);
	if(node)
	{
		llist__push_tail(llist, node->key, node->data);
		free(node);

		return llist__peek_tail(llist);
	}
	
	return NULL;
} // llist__cycle()

void llist__free(LLIST_T* llist)
{
	if(llist)
	{
		llist__node_free(llist->head);
		free(llist);
	}
} // llist__free()

void llist__node_free(LLIST_NODE_T* node)
{
	if(node && node->next) { llist__node_free(node->next); }
	free(node);
} // llist__node_free()