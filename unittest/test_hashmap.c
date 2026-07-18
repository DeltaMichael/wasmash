#include "../include/hashmap.h"
#include <stdio.h>
#include <assert.h>

void test_basic_insert_get() {
	HASHMAP *map = hashmap_init();
	hashmap_insert(map, "mykey", 27);
	hashmap_insert(map, "mykey", 28);
	hashmap_insert(map, "mykey", 20);
	hashmap_insert(map, "mykey", 30);
	hashmap_insert(map, "mykey", 1);
	int value;
	int error = hashmap_get(map, "mykey", &value);
	assert(error == 0);
	assert(value == 1);
}

void test_basic_remove() {
	HASHMAP *map = hashmap_init();
	hashmap_insert(map, "mykey", 27);
	hashmap_insert(map, "yourkey", 28);

	hashmap_remove(map, "yourkey");

	int value;
	int error = hashmap_get(map, "mykey", &value);
	assert(error == 0);
	assert(value == 27);

	error = hashmap_get(map, "yourkey", &value);
	assert(error == 1);
}

int main(int argc, char** args) {
	test_basic_insert_get();
	test_basic_remove();
}

