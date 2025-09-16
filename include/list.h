#ifndef LIST_H
#define LIST_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
	int32_t capacity;
	int32_t pointer;
	void* data;
} LIST;

#define LIST_INIT(type, cap) ({ LIST* list = malloc(sizeof(LIST)); list->capacity = cap; list->pointer = -1; list->data = (type*)malloc(cap * sizeof(type)); list; })
#define LIST_APPEND(list, type, value) \
	({ \
	 	type temp = (value); \
	 	if(list->pointer == list->capacity + 1) { \
 			type* new_data = (type*)realloc(list->data, sizeof(type) * list->capacity * 2);\
 			if(new_data != NULL) { \
 				list->data = new_data; \
				list->capacity = list->capacity * 2; \
 			} else { \
 				printf("Could not realloc list data on appending\n"); \
 				exit(1); \
 			} \
		} \
		type* data = (type*)list->data;\
		data[list->pointer + 1] = temp; \
		list->pointer++; \
	})

// TODO: Handle errors gracefully
#define LIST_GET(list, type, index) \
	({ \
		if((index) > list->pointer || (index) < 0) { \
			printf("List access outside of range"); \
			exit(1); \
	 	} \
		type* data = (type*)list->data;\
		data[(index)]; \
	})

#endif

