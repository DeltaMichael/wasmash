#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdint.h>
#include <stdbool.h>
#include "list.h"

typedef struct {
	char* key;
	void* value;
	int prev;
	int next;
	bool active;
} H_ENTRY;

typedef struct {
	H_ENTRY* data;
	int current_size;
	int capacity;
	int (*hash)(char* key, int capacity);
} HASHMAP;

// Internal
int hash_string(char* key, int capacity);
HASHMAP* hashmap_init_with_capacity(int capacity);
HASHMAP* hashmap_init();
void hashmap_resize(HASHMAP* map);
int hashmap_find_empty(HASHMAP* map, int start);
int hashmap_find_key_index(HASHMAP* map, int start, char* key);
int hashmap_get_key_index(HASHMAP* map, char* key);
void hashmap_insert(HASHMAP* map, char* key, void* value);
void* hashmap_get(HASHMAP* map, char* key);
int hashmap_remove(HASHMAP* map, char* key);

// Integer
int hashmap_get_int(HASHMAP* map, char* key, int *dest);
void hashmap_insert_int(HASHMAP* map, char* key, int value);

// List

int hashmap_get_list(HASHMAP* map, char* key, LIST **dest);
void hashmap_insert_list(HASHMAP* map, char* key, LIST *value);

#endif

