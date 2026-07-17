#include "../include/hashmap.h"
#include <stdio.h>

int main(int argc, char** args) {
	HASHMAP *map = hashmap_init();
	hashmap_insert(map, "mykey", 27);
	int key_exists = 0;
	int value = hashmap_get(map, "mykey", &key_exists);
	if (key_exists) {
		printf("Value for key %s is %d", "mykey", value);
	}
}

