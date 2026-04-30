///////////////////////////////////////////////////////////
//
// Υλοποίηση του ADT MPQ μέσω του ADT Priority Queue.
//
///////////////////////////////////////////////////////////

#include <stdlib.h>

#include "ADTMPQ.h"
#include "ADTPriorityQueue.h"

// Ενα MPQ είναι pointer σε αυτό το struct
struct mpq {
	PriorityQueue pqueue;
};

MPQ mpq_create(CompareFunc compare, DestroyFunc destroy_value) {
	//Δέσμευση μνήμης
	MPQ mpq = malloc(sizeof(*mpq));
	//Δημιουργία εσωτερικού PriorityQueue
	mpq->pqueue = pqueue_create(compare, destroy_value, NULL);
	return mpq;
}

int mpq_size(MPQ mpq) {
	return pqueue_size(mpq->pqueue);
}

Pointer mpq_max(MPQ mpq) {
	return pqueue_max(mpq->pqueue);
}

void mpq_insert(MPQ mpq, Pointer value) {
	pqueue_insert(mpq->pqueue, value);
}

void mpq_remove_max(MPQ mpq) {
	pqueue_remove_max(mpq->pqueue);
}

void mpq_merge(MPQ mpq, MPQ mpq2) {
	//Απενεργοποίηση της DestroyFunc της mpq2 για να μην κληθεί
    pqueue_set_destroy_value(mpq2->pqueue, NULL);
	
	int size = mpq_size(mpq2);
	for (int i = 0; i < size; i++){
		//Εισάγουμε το μέγιστο (της mpq2) στην mpq
        mpq_insert(mpq, mpq_max(mpq2));
		//Και το διαγράφουμε από την mpq2
	    mpq_remove_max(mpq2);
	}
	
	mpq_destroy(mpq2);
}

DestroyFunc mpq_set_destroy_value(MPQ mpq, DestroyFunc destroy_value) {
	return pqueue_set_destroy_value(mpq->pqueue, destroy_value);
}

void mpq_destroy(MPQ mpq) {
	pqueue_destroy(mpq->pqueue);
	free(mpq);
}
