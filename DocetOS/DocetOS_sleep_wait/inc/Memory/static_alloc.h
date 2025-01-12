#ifndef STATIC_ALLOC_H
#define STATIC_ALLOC_H
#include <stddef.h>

#define STATIC_ALLOC_ALIGNMENT ((size_t) 8)

void * static_alloc(size_t bytes);

#endif /* STATIC_ALLOC_H */
