///////////////////////////////////////////////////////////
//
// Βοηθητικές συναρτήσεις για τον ADT MPQ.
//
// Υποστηρίζουν οποιαδήποτε υλοποίηση του MPQ (χωρίς να
// γνωρίζουμε τις λεπτομέρειες της υλοποίησης).
//
///////////////////////////////////////////////////////////

#include "ADTMPQ_aux.h"

MPQ mpq_create_from_values(CompareFunc compare, DestroyFunc destroy_value, Vector values) {
	// Απλοϊκή υλοποίηση που κάνει διαδοχικά mpq_insert. Θέλουμε να την
	// αντικαταστήσουμε με κάτι πιο αποδοτικό, που να εξακολουθεί όμως να
	// δουλεύει για οποιαδήποτε υλοποίηση του ADT MPQ.

	MPQ mpq = mpq_create(compare, destroy_value);

	for (int i = 0; i < vector_size(values); i++)
		mpq_insert(mpq, vector_get_at(values, i));

	return mpq;
}
