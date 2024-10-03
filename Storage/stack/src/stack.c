#include "stack.h"

void push(int32_t **sp, int32_t value) {
	**sp = value;
	*sp = *sp + 1;
}

int32_t pop(int **sp) {
	*sp = *sp - 1;
	int32_t value = **sp;
	return value;
}
