///////////////////////////////////////////////////////////
//
// Υλοποίηση του ADT MPQ μέσω συνδεδεμένης λίστας.
//
///////////////////////////////////////////////////////////

#include <stdlib.h>

#include "ADTMPQ.h"

// Ενα MPQ είναι pointer σε αυτό το struct
struct mpq {
};

MPQ mpq_create(CompareFunc compare, DestroyFunc destroy_value) {
	return NULL;
}

int mpq_size(MPQ mpq) {
	return 0;
}

Pointer mpq_max(MPQ mpq) {
	return NULL;
}

void mpq_insert(MPQ mpq, Pointer value) {
}

void mpq_remove_max(MPQ mpq) {
}

void mpq_merge(MPQ mpq, MPQ mpq2) {
}

DestroyFunc mpq_set_destroy_value(MPQ mpq, DestroyFunc destroy_value) {
	return NULL;
}

void mpq_destroy(MPQ mpq) {
}
