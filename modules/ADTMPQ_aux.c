///////////////////////////////////////////////////////////
//
// Βοηθητικές συναρτήσεις για τον ADT MPQ.
//
// Υποστηρίζουν οποιαδήποτε υλοποίηση του MPQ (χωρίς να
// γνωρίζουμε τις λεπτομέρειες της υλοποίησης).
//
///////////////////////////////////////////////////////////

#include "ADTMPQ_aux.h"

//Βοηθητική αναδρομική συνάρτηση
static MPQ mpq_create_recursive(CompareFunc compare, DestroyFunc destroy_value, Vector values, int first, int last) {
	MPQ mpq;

	//Αν το vector είναι κενό
	if (first > last) {
		mpq = mpq_create(compare, destroy_value);
		return mpq;
	}

	//Αν περιέχει μόνο ένα στοιχείο
	else if (first == last) {
		mpq = mpq_create(compare, destroy_value);
		Pointer value = vector_get_at(values, first);
		mpq_insert(mpq, value);
		return mpq;
	}

	//Αν περιέχει περισσότερα από ένα στοιχεία
	int mid = (first + last)/2;

	//Αναδρομική δημιουργία δύο MPQ, το πρώτο μισό και το δεύτερο
	MPQ left = mpq_create_recursive(compare, destroy_value, values, first, mid);
	MPQ right = mpq_create_recursive(compare, destroy_value, values, mid+1, last);
	
	//Merge των δύο MPQ σε ένα (left)
	mpq_merge(left, right);
	return left;
}

MPQ mpq_create_from_values(CompareFunc compare, DestroyFunc destroy_value, Vector values) {
	int size = vector_size(values);
    MPQ mpq = mpq_create_recursive(compare, destroy_value, values, 0, size - 1);
	return mpq;
}
