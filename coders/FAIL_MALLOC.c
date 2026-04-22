#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>

void* malloc(size_t size) {
	static int count = 0;
	if (count++ == 2) { // Le 10ème malloc échouera
		return NULL;
	}
	void *(*real_malloc)(size_t) = dlsym(RTLD_NEXT, "malloc");
	return real_malloc(size);
}