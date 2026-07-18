#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include "include/hashmap.h"

int hash_string(HASHMAP* map, char* key) {
	double sum = 0;
	int i = 0;
	int p = 53;
	while(*key != 0) {
		sum += *key * pow(p, i);
		i++;
		key++;
	}
	uint64_t temp = sum;
	temp = temp % map->capacity;
	return (int)temp;
}

HASHMAP* hashmap_init() {
	HASHMAP* map = malloc(sizeof(HASHMAP));
	map->capacity = 256;
	map->current_size = 0;
	map->data = calloc(map->capacity, sizeof(H_ENTRY));
	for (int i = 0; i < map->capacity; i++) {
		(map->data + i)->active = false;
		(map->data + i)->prev = -1;
		(map->data + i)->next = -1;
	}
	return map;
}

void hashmap_resize(HASHMAP* map) {
	H_ENTRY *old_data = map->data;
	// Create new array
	H_ENTRY *new_data = calloc(map->capacity * 2, sizeof(H_ENTRY));
	map->capacity *= 2;
	map->current_size = 0;
	map->data = new_data;
	for (int i = 0; i < map->capacity; i++) {
		(map->data + i)->active = false;
		(map->data + i)->prev = -1;
		(map->data + i)->next = -1;
	}

	for (int i = 0; i < map->capacity / 2; i++) {
		H_ENTRY *entry = old_data + i;
		if (entry->active) {
			hashmap_insert(map, entry->key, entry->value);
		}
		if (entry->key != NULL) {
			free(entry->key);
		}
	}

	free(old_data);
}

int hashmap_find_empty(HASHMAP* map, int start) {

	// Search to the right
	for(int i = start; i < map->capacity; i++) {
		H_ENTRY *entry = map->data + i;
		if (!entry->active) {
			return i;
		}
	}

	// Search to the left
	for(int i = start; i >= 0; i++) {
		H_ENTRY *entry = map->data + i;
		if (!entry->active) {
			return i;
		}
	}

	return -1;
}

int hashmap_find_key_index(HASHMAP* map, int start, char* key) {

	// Search to the right
	for(int i = start; i < map->capacity; i++) {
		H_ENTRY *entry = map->data + i;
		if (entry->key != NULL && strcmp(key, entry->key) == 0) {
			return i;
		}
	}

	// Search to the left
	for(int i = start; i >= 0; i--) {
		H_ENTRY *entry = map->data + i;
		if (entry->key != NULL && strcmp(key, entry->key) == 0) {
			return i;
		}
	}

	return -1;
}

int hashmap_get_key_index(HASHMAP* map, char* key) {
	int index = hash_string(map, key);
	H_ENTRY *entry = map->data + index;
	if (entry->key != NULL && strcmp(key, entry->key) == 0) {
		return index;
	}

	return hashmap_find_key_index(map, index, key);
}

void hashmap_insert(HASHMAP* map, char* key, int value) {

	// Resize if necessary
	if (map->current_size > 0 && map->capacity / map->current_size <= 2) {
		hashmap_resize(map);
	}

	int index = hash_string(map, key);
	H_ENTRY *entry = map->data + index;

	// It's a new key
	if (!entry->active) {
		entry->key = strdup(key);
		entry->value = value;
		entry->active = true;
		return;
	}

	// Determine if overwrite or actual collision
	while (strcmp(key, entry->key) != 0 && entry->next != -1) {
		index = entry->next;
		entry = map->data + entry->next;
	}

	// Overwrite
	if (strcmp(key, entry->key) == 0) {
		entry->value = value;
		return;
	}

	// Find next entry
	int new_index = hashmap_find_empty(map, index);
	while(new_index == -1) {
		hashmap_resize(map);
		new_index = hashmap_find_empty(map, index);
	}

	// Fill in data
	H_ENTRY *new_entry = map->data + new_index;
	new_entry->key = strdup(key);
	new_entry->value = value;
	new_entry->active = true;

	// Stitch entries
	entry->next = new_index;
	new_entry->prev = index;
}

int hashmap_get(HASHMAP* map, char* key, int *dest) {
	int index = hash_string(map, key);
	H_ENTRY *entry = map->data + index;
	if (!entry->active) {
		return 1;
	}

	while (strcmp(key, entry->key) != 0 && entry->next != -1) {
		entry = map->data + entry->next;
	}

	if (strcmp(key, entry->key) == 0) {
		*dest = entry->value;
		return 0;
	}

	return 1;
}

int hashmap_remove(HASHMAP* map, char* key) {
	int index = hash_string(map, key);
	H_ENTRY *entry = map->data + index;

	if (!entry->active) {
		return 1;
	}

	while (strcmp(key, entry->key) != 0 && entry->next != -1) {
		entry = map->data + entry->next;
	}

	if (strcmp(key, entry->key) == 0) {
		// Remove it
		if (entry->prev == -1 && entry->next == -1) { // alone
			entry->active = false;
		} else if (entry->prev == -1 && entry->next != -1) { // head
			H_ENTRY *next = map->data + entry->next;
			entry->key = next->key;
			entry->value = next->value;
			entry->next = next->next;
			next->active = false;
			H_ENTRY *next_next = map->data + next->next;
			next_next->prev = next->prev;
		} else if (entry->prev != -1 && entry->next == -1) { // tail
			H_ENTRY *prev = map->data + entry->prev;
			prev->next = -1;
			entry->active = false;
			entry->prev = -1;
			entry->next = -1;
		} else { // middle
			H_ENTRY *prev = map->data + entry->prev;
			H_ENTRY *next = map->data + entry->next;
			prev->next = entry->next;
			next->prev = entry->prev;
			entry->active = false;
			entry->prev = -1;
			entry->next = -1;
		}
		return 0;
	}

	return 1;
}

