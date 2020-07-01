#include "queue.h"

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

QUEUE_T* queue_create(size_t size)
{
	QUEUE_T* queue = malloc(sizeof(QUEUE_T));
	if(!queue) { return NULL; }

	queue->data = malloc(size * sizeof(void*));
	if(!queue->data)
	{
		free(queue);
		return NULL;
	}
	
	queue->front = 0;
    queue->rear = size - 1;
    queue->used = 0;
	queue->size = size;

	return queue;
} // queue_create()


bool queue_empty(QUEUE_T* queue)
{
	if(queue->used == 0) {
		return true;
	} else {
		return false;
	}
} // queue_empty()

bool queue_full(QUEUE_T* queue)
{
	if(queue->used == queue->size) {
		return true;
	} else {
		return false;
	}
} // queue_full()

bool queue_push(QUEUE_T* queue, void* item)
{
	if (queue_full(queue) == true) {
		return false;
	} else {
		queue->rear = (queue->rear + 1) % queue->size; 
		queue->data[queue->rear] = item; 
		queue->used = queue->used + 1; 
	}
	return true;
} // queue_push()

void* queue_pop(QUEUE_T* q)
{
	void* item = NULL;

    if (queue_empty(q) == true) 
        return item;

    item = q->data[q->front]; 
    q->front = (q->front + 1) % q->size; 
    q->used = q->used - 1; 

    return item;
} // () queue_pop()

void* queue_cycle(QUEUE_T* queue)
{
	void* item = queue_pop(queue);
	if(item)
	{
		queue_push(queue, item);
		return item;
	}
	return NULL;
} // queue_cycle()