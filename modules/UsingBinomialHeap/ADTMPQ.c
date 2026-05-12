///////////////////////////////////////////////////////////
//
// Υλοποίηση του ADT MPQ μέσω Binomial Heap
//
///////////////////////////////////////////////////////////

#include <stdlib.h>

#include "ADTMPQ.h"

//Δομή κόμβου Binomial Tree
struct node {
	Pointer value; 
	Vector children;
};

// Ενα MPQ είναι pointer σε αυτό το struct
struct mpq {
	Vector trees;
	int size;
	CompareFunc compare;
	DestroyFunc destroy_value;
};

//Βοηθητική συνάρτηση: Συγχωνεύει δύο Binomial Trees τάξης k σε ένα τάξης k+1
struct node* merge_trees(struct node* Node1, struct node* Node2, MPQ mpq) {
    //Αν το Node1 είναι μεγαλύτερο από το Node2
	if (mpq->compare(Node1->value, Node2->value) > 0) {
		//Το Node2 γίνεται παιδί του Node1
		vector_insert_last(Node1->children, Node2);
		return Node1;
	}
	//Και αντίθετα 
	else {
		vector_insert_last(Node2->children, Node1);
		return Node2;
	}
}

//Βοηθητική συνάρτηση: Προσθέτει ένα δέντρο στο δάσος
static void add_trees(Vector trees, struct node* new, MPQ mpq) {
	int k = vector_size(new->children);

	//Όσο στη θέση k υπάρχει ήδη δέντρο
    while (k < vector_size(trees) && vector_get_at(trees, k) != NULL) {
		struct node* tree = vector_get_at(trees, k);
		//Αδειάζουμε τη θέση k
		vector_set_at(trees, k, NULL);
		//Κάνουμε merge τα δύο δέντρα
		new = merge_trees(new, tree, mpq);
		//Και προχωράμε στην επόμενη τάξη
		k++;
	}

	//Το τοποθετούμε στην πρώτη κενή θέση k
	if (k < vector_size(trees)) {
		vector_set_at(trees, k, new);
	}
	//Αν ο vector δεν φτάνει μέχρι την τάξη k τον μεγαλώνουμε
	else {
		vector_insert_last(trees, new);
	}
}

//Βοηθητική συνάρτηση: Αναδρομική καταστροφή κόμβου και των παιδιών του
static void node_destroy(struct node* node, DestroyFunc destroy_value) {
	if (node == NULL) {
		return;
	}

	//Καταστρέφουμε αναδρομικά όλα τα παιδιά
	for (int i = 0; i < vector_size(node->children); i++) {
		node_destroy(vector_get_at(node->children, i), destroy_value);
	}

	//Καταστροφή του vector των παιδιών
	vector_destroy(node->children);
	//Καταστροφή τιμής
	if (destroy_value != NULL) {
		destroy_value(node->value);
	}
	//Ελευθέρωση του ίδιου του struct
	free(node);
}

MPQ mpq_create(CompareFunc compare, DestroyFunc destroy_value) {
	//Δέσμευση μνήμης
	MPQ mpq = malloc(sizeof(*mpq));
	//Δημιουργία κενής ουράς
	mpq->compare = compare;
	mpq->destroy_value = destroy_value;
	mpq->size = 0;
	mpq->trees = vector_create(0, NULL);
	return mpq;
}

int mpq_size(MPQ mpq) {
	return mpq->size;
}

Pointer mpq_max(MPQ mpq) {
	Pointer max = NULL;
	//Διατρέχουμε όλο τον πίνακα trees
	for (int i = 0; i < vector_size(mpq->trees); i++) {
		struct node* node = vector_get_at(mpq->trees, i);
		if (node != NULL) {
			//Ελέγχουμε αν η ρίζα είναι μεγαλύτερη από το τρέχων max
			if (max == NULL || mpq->compare(node->value, max) > 0) {
			    max = node->value;
		    }
		}
    }
	return max;
}

void mpq_insert(MPQ mpq, Pointer value) {
	//Δημιουργία δέντρου τάξης 0
	struct node* node = malloc(sizeof(struct node));
	node->value = value;
	node->children = vector_create(0, NULL);
	//Προσθήκη στο δάσος
	add_trees(mpq->trees, node, mpq);
	mpq->size++;
}

void mpq_remove_max(MPQ mpq) {
	if (mpq->size == 0)
	    return;

	struct node* max = NULL;
	int max_index = -1;

	//Διατρέχουμε όλο τον πίνακα trees
	for (int i = 0; i < vector_size(mpq->trees); i++) {
		struct node* node = vector_get_at(mpq->trees, i);
		if (node != NULL) {
			//Ελέγχουμε αν η ρίζα είναι μεγαλύτερη από το τρέχων max
			if (max == NULL || mpq->compare(node->value, max->value) > 0) {
			    max = node;
				max_index = i;
		    }
		}
    }

	//Αφαιρούμε το max δέντρο από το δάσος
	vector_set_at(mpq->trees, max_index, NULL);

	//Διατρέχουμε όλα τα παιδιά του δέντρου
	for (int i = 0; i < vector_size(max->children); i++) {
		struct node* child = vector_get_at(max->children, i);
		//Τα προσθέτουμε πίσω στο δάσος
		add_trees(mpq->trees, child, mpq);
	}

	//Αποδέσμευση μνήμης ρίζας
	if (mpq->destroy_value != NULL) {
		mpq->destroy_value(max->value);
	}
	vector_destroy(max->children);
	free(max);
	mpq->size--;
}

void mpq_merge(MPQ mpq, MPQ mpq2) {
	//Παίρνουμε κάθε δέντρο από το mpq2 και το προσθέτουμε στο mpq
	for (int i = 0; i < vector_size(mpq2->trees); i++) {
		struct node* node = vector_get_at(mpq2->trees, i);
		if (node != NULL) {
			add_trees(mpq->trees, node, mpq);
		}
	}
	mpq->size += mpq2->size;

	//Ελευθερώνουμε το mpq2  
	vector_destroy(mpq2->trees);
	free(mpq2);
}

DestroyFunc mpq_set_destroy_value(MPQ mpq, DestroyFunc destroy_value) {
	DestroyFunc old = mpq->destroy_value;
	mpq->destroy_value = destroy_value;
	return old;
}

void mpq_destroy(MPQ mpq) {
	//Για κάθε δέντρο
    for (int i = 0; i < vector_size(mpq->trees); i++) {
		struct node* node = vector_get_at(mpq->trees, i);
		if (node != NULL){
			node_destroy(node, mpq->destroy_value);
		}
	}

	//Καταστροφή του vector των δέντρων
	vector_destroy(mpq->trees);
	//Ελευθέρωση του ίδιου του struct
    free(mpq);
}
