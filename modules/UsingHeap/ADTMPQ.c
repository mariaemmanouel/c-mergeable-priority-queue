///////////////////////////////////////////////////////////
//
// Υλοποίηση του ADT MPQ μέσω σωρού.
//
///////////////////////////////////////////////////////////

#include <stdlib.h>

#include "ADTMPQ.h"

// Ενα MPQ είναι pointer σε αυτό το struct
struct mpq {
	Vector values;				// Τα δεδομένα, σε Vector ώστε να έχουμε μεταβλητό μέγεθος χωρίς κόπο
	CompareFunc compare;		// Η διάταξη
	DestroyFunc destroy_value;	// Συνάρτηση που καταστρέφει ένα στοιχείο του vector.
};

static Pointer node_value(MPQ mpq, int node_id) {
	// τα node_ids είναι 1-based, το node_id αποθηκεύεται στη θέση node_id - 1
	return vector_get_at(mpq->values, node_id - 1);
}

static void node_swap(MPQ mpq, int node_id1, int node_id2) {
	// τα node_ids είναι 1-based, το node_id αποθηκεύεται στη θέση node_id - 1
	Pointer value1 = node_value(mpq, node_id1);
	Pointer value2 = node_value(mpq, node_id2);

	vector_set_at(mpq->values, node_id1 - 1, value2);
	vector_set_at(mpq->values, node_id2 - 1, value1);
}

static void bubble_up(MPQ mpq, int node_id) {
	// Αν φτάσαμε στη ρίζα, σταματάμε
	if (node_id == 1)
		return;

	int parent = node_id / 2;		// Ο πατέρας του κόμβου. Τα node_ids είναι 1-based

	// Αν ο πατέρας έχει μικρότερη τιμή από τον κόμβο, swap και συνεχίζουμε αναδρομικά προς τα πάνω
	if (mpq->compare(node_value(mpq, parent), node_value(mpq, node_id)) < 0) {
		node_swap(mpq, parent, node_id);
		bubble_up(mpq, parent);
	}
}

static void bubble_down(MPQ mpq, int node_id) {
	// βρίσκουμε τα παιδιά του κόμβου (αν δεν υπάρχουν σταματάμε)
	int left_child = 2 * node_id;
	int right_child = left_child + 1;

	int size = mpq_size(mpq);
	if (left_child > size)
		return;

	// βρίσκουμε το μέγιστο από τα 2 παιδιά
	int max_child = left_child;
	if (right_child <= size && mpq->compare(node_value(mpq, left_child), node_value(mpq, right_child)) < 0)
			max_child = right_child;

	// Αν ο κόμβος είναι μικρότερος από το μέγιστο παιδί, swap και συνεχίζουμε προς τα κάτω
	if (mpq->compare(node_value(mpq, node_id), node_value(mpq, max_child)) < 0) {
		node_swap(mpq, node_id, max_child);
		bubble_down(mpq, max_child);
	}
}

MPQ mpq_create(CompareFunc compare, DestroyFunc destroy_value) {
	//Δέσμευση μνήμης
	MPQ mpq = malloc(sizeof(*mpq));
	mpq->compare = compare;
	mpq->destroy_value = destroy_value;
	//Δημιουργία Vector χωρίς destroy_value
	mpq->values = vector_create(0, NULL);
	return mpq;
}

int mpq_size(MPQ mpq) {
	return vector_size(mpq->values);
}

Pointer mpq_max(MPQ mpq) {
	//Αν το size είναι 0, δεν υπάρχει max
	if (vector_size(mpq->values) == 0)
	    return NULL;
	else
	    //Το μέγιστο είναι πάντα η ρίζα
	    return node_value(mpq, 1);
}

void mpq_insert(MPQ mpq, Pointer value) {
	//Προσθήκη στο τέλος της λίστας
	vector_insert_last(mpq->values, value);
	//Ανέβασμα στη σωστή θέση
    bubble_up(mpq, vector_size(mpq->values));
}

void mpq_remove_max(MPQ mpq) {
	//Αν το size είναι 0, δεν κάνει τίποτα
	if (vector_size(mpq->values) == 0)
	    return;

	//Καταστροφή της τιμής της ρίζας
	if (mpq->destroy_value != NULL)
	    mpq->destroy_value(mpq_max(mpq));

	//Ανταλλαγή ρίζας με τελευταίο στοιχείο και διαγραφή τελευταίου
	node_swap(mpq, 1, vector_size(mpq->values));
	vector_remove_last(mpq->values);

	//Αποκατάσταση σωρού από τη ρίζα και κάτω
	if (vector_size(mpq->values) != 0)
	    bubble_down(mpq, 1);
}

void mpq_merge(MPQ mpq, MPQ mpq2) {
	//Μεταφορά στοιχείων της mpq2 στην mpq
	for (int i = 1; i <= vector_size(mpq2->values); i++)
	    vector_insert_last(mpq->values, node_value(mpq2, i));

	//Μεταροπή του πίνακα σε σωρό
    int size = vector_size(mpq->values);
	for (int i = size/2; i >= 1; i--)
	    bubble_down(mpq, i);

	//Καταστροφή της mpq2
	mpq_set_destroy_value(mpq2, NULL);
	mpq_destroy(mpq2);
}

DestroyFunc mpq_set_destroy_value(MPQ mpq, DestroyFunc destroy_value) {
	DestroyFunc old = mpq->destroy_value;
	mpq->destroy_value = destroy_value;
	return old;
}

void mpq_destroy(MPQ mpq) {
	//Ενημέρωση του Vector για την συνάρτηση destroy
	vector_set_destroy_value(mpq->values, mpq->destroy_value);
	//Καταστροφή του Vector
	vector_destroy(mpq->values);
	//Ελευθέρωση του ίδιου του struct
	free(mpq);
}
