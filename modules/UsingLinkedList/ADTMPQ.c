///////////////////////////////////////////////////////////
//
// Υλοποίηση του ADT MPQ μέσω συνδεδεμένης λίστας.
//
///////////////////////////////////////////////////////////

#include <stdlib.h>

#include "ADTMPQ.h"

//Δομή κόμβου λίστας
struct node {
	Pointer value;
	struct node* next;
};

// Ενα MPQ είναι pointer σε αυτό το struct
struct mpq {
	struct node* first; //Δείκτης στον πρώτο κόμβο
	struct node* last; //Δείκτης στον τελευταίο κόμβο
	struct node* max; //Δείκτης στον κόμβο με την μέγιστη τιμή
	int size; //Μέγεθος ουράς
	CompareFunc compare;
	DestroyFunc destroy_value;
};

MPQ mpq_create(CompareFunc compare, DestroyFunc destroy_value) {
    //Δέσμευση μνήμης
	MPQ mpq = malloc(sizeof(*mpq));
	//Δημιουργία κενής ουράς
	mpq->first = NULL;
	mpq->last = NULL;
	mpq->max = NULL;
	mpq->size = 0;
	mpq->compare = compare;
	mpq->destroy_value = destroy_value;
	return mpq;
}

int mpq_size(MPQ mpq) {
	return mpq->size;
}

Pointer mpq_max(MPQ mpq) {
    //Αν το size είναι 0, δεν υπάρχει max
	if (mpq->size == 0)
	    return NULL;
	else
	    return mpq->max->value;
}

void mpq_insert(MPQ mpq, Pointer value) {
	struct node* Node = malloc(sizeof(struct node));
	Node->value = value;
	Node->next = NULL;
	
	//Αν η λίστα είναι κενή, ο νέος κόμβος είναι αρχή, τέλος και μέγιστο
	if (mpq->size == 0) {
		mpq->first = Node;
		mpq->last = Node;
		mpq->max = Node;
	}
	else {
		//Προσθήκη στο τέλος της λίστας
		mpq->last->next = Node;
		mpq->last = Node;
		//Ενημέρωση του δείκτη max
		if (mpq->compare(value, mpq->max->value) > 0) {
			mpq->max = Node;
		}
	}
	mpq->size++;
}

void mpq_remove_max(MPQ mpq) {
    //Αν η λίστα είναι κενή, δεν κάνει τίποτα
	if (mpq->size == 0) {
		return;
	}

	//Αν το max είναι στην αρχή
	if (mpq->max == mpq->first) {
		//Ενημερώνουμε τον first
		mpq->first = mpq->first->next;
	}

	else {
		//Βρίσκουμε τον προηγούμενο κόμβο από τον max
		struct node* prev = mpq->first;
		while (prev->next != mpq->max){
			prev = prev->next;
		}

		//Αν το max είναι στο τέλος
		if (mpq->max == mpq->last) {
			//Ενημερώνουμε τον last
			mpq->last = prev;
			prev->next = NULL;
		}
		else {
			//Προσπερνάμε τον κόμβο max
			prev->next = mpq->max->next;
		}
	}

	//Αποδέσμευση μνήμης του κόμβου και της τιμής του
	struct node* temp = mpq->max;
	if (mpq->destroy_value != NULL) {
		mpq->destroy_value(temp->value);
	}
	free(temp);
	mpq->size--;

	//Εύρεση του νέου max
	if (mpq->size == 0) {
		mpq->first = NULL;
		mpq->last = NULL;
		mpq->max = NULL;
	}
	else {
		mpq->max = mpq->first;
		for (struct node* i = mpq->first; i != NULL; i = i->next) {
			if (mpq->compare(i->value, mpq->max->value) > 0) {
				mpq->max = i;
			}
		}
	}
}

void mpq_merge(MPQ mpq, MPQ mpq2) {
	//Αν και οι δύο ουρές έχουν στοιχεία
	if (mpq->size > 0 && mpq2->size > 0) {
		//Ενώνουμε το τέλος της πρώτης με την αρχή της δεύτερης
		mpq->last->next = mpq2->first;
		mpq->last = mpq2->last;

		//Ενημερώνουμε το max και το size
		if (mpq->compare(mpq->max->value, mpq2->max->value) < 0) {
			mpq->max = mpq2->max;
		}
		mpq->size += mpq2->size;
		
		//Ελευθερώνουμε το struct της δεύτερης ουράς
		free(mpq2);
	}

	//Αν η πρώτη είναι κενή αλλά η δεύτερη έχει στοιχεία 
	else if (mpq->size == 0 && mpq2->size > 0) {
		//Μεταφέρουμε όλα τα στοιχεία της δεύτερης στην πρώτη
		mpq->first = mpq2->first;
		mpq->last = mpq2->last;
		mpq->max = mpq2->max;
		mpq->size = mpq2->size;

		free(mpq2);
	}

	//Αν η δεύτερη είναι κενή ή και οι δύο
	else {
		free(mpq2);
	}
}

DestroyFunc mpq_set_destroy_value(MPQ mpq, DestroyFunc destroy_value) {
	DestroyFunc old = mpq->destroy_value;
	mpq->destroy_value = destroy_value;
	return old;
}

void mpq_destroy(MPQ mpq) {
	struct node* i = mpq->first;
	while (i != NULL) {
		//Αποθήκευση του επόμενου πριν το free
		struct node* next = i->next;
		if (mpq->destroy_value != NULL) {
			mpq->destroy_value(i->value);
		}
		//Ελευθέρωση του κόμβου
		free(i);
		i = next;
	}
	//Ελευθέρωση του ίδιου του struct
	free(mpq);
}
