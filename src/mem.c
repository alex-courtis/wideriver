#include <stdint.h>
#include <stdlib.h>

#include "mem.h"

double *doubledup(const double d) {
	double *p = calloc(1, sizeof(double));
	*p = d;
	return p;
}

int32_t *int32dup(const int32_t i) {
	int32_t *p = calloc(1, sizeof(int32_t));
	*p = i;
	return p;
}

uint32_t *uint32dup(const uint32_t i) {
	uint32_t *p = calloc(1, sizeof(uint32_t));
	*p = i;
	return p;
}
