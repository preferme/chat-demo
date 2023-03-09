/*
 * memory.h
 *
 *  Created on: 2023年3月4日
 *      Author: hou-lei
 */

#ifndef __MEMORY_H_
#define __MEMORY_H_

void * chat_alloc(int size);

void chat_free(void * ptr);

#endif /* __MEMORY_H_ */
