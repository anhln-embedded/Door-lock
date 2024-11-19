#include "circular_queue.h"

void CircularQueue_Init(CircularQueue_t *queue){
	queue->head = 0;
	queue->tail = 0;
}
bool CircularQueue_IsFull(const CircularQueue_t *queue){
	return ((queue->head + 1) % QUEUE_SIZE) == queue->tail;
}
bool CircularQueue_IsEmpty(const CircularQueue_t *queue){
	return queue->head == queue->tail;
}
bool CircularQueue_Enqueue(CircularQueue_t *queue, uint8_t data){
    if (CircularQueue_IsFull(queue)) {
        return false;
    }	
		queue->buffer[queue->head] = data;
		queue->head = (queue->head + 1) % QUEUE_SIZE;
		return true;
}
bool CircularQueue_Dequeue(CircularQueue_t *queue, uint8_t *data){
    if (CircularQueue_IsEmpty(queue)) {
        return false;
    }	
		*data = queue->buffer[queue->tail];
		queue->tail = (queue->tail + 1) % QUEUE_SIZE;
		return true;
}

void CircularQueue_Free(CircularQueue_t *queue){
	queue->head = queue->tail;
}

uint16_t CircularQueue_Size(CircularQueue_t *queue) {
    if (queue->head >= queue->tail) {
        return queue->head - queue->tail;
    } else {
        return QUEUE_SIZE - queue->tail + queue->head;
    }
}