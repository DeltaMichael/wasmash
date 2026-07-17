#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
	char* key;
	int value;
	bool active;
} H_ENTRY;

typedef struct {
	H_ENTRY* data;
	int current_size;
	int capacity;
} HASHMAP;

int hash_string(HASHMAP* map, char* key);
HASHMAP* hashmap_init();
void hashmap_resize(HASHMAP* map);
int hashmap_find_empty(HASHMAP* map, int start);
int hashmap_find_key_index(HASHMAP* map, int start, char* key);
int hashmap_get_key_index(HASHMAP* map, char* key);
void hashmap_insert(HASHMAP* map, char* key, int value);
int hashmap_get(HASHMAP* map, char* key, int *error_code);
void hashmap_remove(HASHMAP* map, char* key);

#endif

