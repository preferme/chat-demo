/*
 * memory.c
 *
 *  Created on: 2023年3月4日
 *      Author: hou-lei
 */
#include "../include/memory.h"
#include <stdlib.h>


void * chat_alloc(int size) {
	return malloc(size);
}

void chat_free(void * ptr) {
	free(ptr);
}
