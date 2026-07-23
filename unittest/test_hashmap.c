#include "../include/hashmap.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int always_collision(char *key, int capacity) { return 0; }

void test_resize() {
  HASHMAP *map = hashmap_init_with_capacity(8);
  char *expected_keys[5] = {"one", "two", "three", "four", "five"};
  int expected_values[5] = {11, 22, 31, 54, 44};

  for (int i = 0; i < 5; i++) {
    hashmap_insert_int(map, expected_keys[i], expected_values[i]);
  }

  assert(map->capacity == 16);

  for (int i = 0; i < 5; i++) {
    int value;
    int error = hashmap_get_int(map, expected_keys[i], &value);
    assert(error == 0);
    assert(value == expected_values[i]);
  }
}

void test_collisions_insert_get() {
  HASHMAP *map = hashmap_init();
  map->hash = always_collision;
  hashmap_insert_int(map, "first_key", 27);
  hashmap_insert_int(map, "second_key", 28);
  hashmap_insert_int(map, "third_key", 20);
  int value;
  int error = hashmap_get_int(map, "first_key", &value);
  assert(error == 0);
  assert(value == 27);
  error = hashmap_get_int(map, "second_key", &value);
  assert(error == 0);
  assert(value == 28);
  error = hashmap_get_int(map, "third_key", &value);
  assert(error == 0);
  assert(value == 20);
}

void test_collisions_remove_head() {
  HASHMAP *map = hashmap_init();
  map->hash = always_collision;
  char *expected_keys[5] = {"one", "two", "three", "four", "five"};
  int expected_values[5] = {11, 23, 38, 45, 57};

  for (int i = 0; i < 5; i++) {
    hashmap_insert_int(map, expected_keys[i], expected_values[i]);
  }

  int value;
  hashmap_remove(map, "one");

  int error = hashmap_get_int(map, "one", &value);
  assert(error == 1);

  for (int i = 1; i < 5; i++) {
    error = hashmap_get_int(map, expected_keys[i], &value);
    assert(error == 0);
    assert(value == expected_values[i]);
  }
}

void test_collisions_remove_tail() {
  HASHMAP *map = hashmap_init();
  map->hash = always_collision;
  char *expected_keys[5] = {"one", "two", "three", "four", "five"};
  int expected_values[5] = {11, 23, 38, 45, 57};

  for (int i = 0; i < 5; i++) {
    hashmap_insert_int(map, expected_keys[i], expected_values[i]);
  }

  int value;
  hashmap_remove(map, "five");

  int error = hashmap_get_int(map, "five", &value);
  assert(error == 1);

  for (int i = 0; i < 4; i++) {
    error = hashmap_get_int(map, expected_keys[i], &value);
    assert(error == 0);
    assert(value == expected_values[i]);
  }
}

void test_collisions_remove_middle() {
  HASHMAP *map = hashmap_init();
  map->hash = always_collision;
  char *expected_keys[5] = {"one", "two", "three", "four", "five"};
  int expected_values[5] = {1, 2, 3, 4, 5};

  for (int i = 0; i < 5; i++) {
    hashmap_insert_int(map, expected_keys[i], expected_values[i]);
  }

  int value;
  hashmap_remove(map, "three");
  int error = hashmap_get_int(map, "three", &value);
  assert(error == 1);
  error = hashmap_get_int(map, "one", &value);
  assert(error == 0);
  assert(value == 1);
  error = hashmap_get_int(map, "two", &value);
  assert(error == 0);
  assert(value == 2);
  error = hashmap_get_int(map, "four", &value);
  assert(error == 0);
  assert(value == 4);
  error = hashmap_get_int(map, "five", &value);
  assert(error == 0);
  assert(value == 5);
}

void test_get_non_existant() {
  HASHMAP *map = hashmap_init();
  hashmap_insert_int(map, "mykey", 27);
  int value;
  int error = hashmap_get_int(map, "yourkey", &value);
  assert(error == 1);
}

void test_basic_insert_get() {
  HASHMAP *map = hashmap_init();
  hashmap_insert_int(map, "mykey", 27);
  hashmap_insert_int(map, "mykey", 28);
  hashmap_insert_int(map, "mykey", 20);
  hashmap_insert_int(map, "mykey", 30);
  hashmap_insert_int(map, "mykey", 1);
  int value;
  int error = hashmap_get_int(map, "mykey", &value);
  assert(error == 0);
  assert(value == 1);
}

void test_basic_insert_get_list() {
  HASHMAP *map = hashmap_init();
  LIST *list = LIST_INIT(int, 25);
  LIST_APPEND(list, int, 0);
  LIST_APPEND(list, int, 1);
  LIST_APPEND(list, int, 2);
  LIST_APPEND(list, int, 3);
  hashmap_insert_list(map, "mykey", list);
  LIST *value;
  int error = hashmap_get_list(map, "mykey", &value);
  assert(error == 0);
  int element = LIST_GET(value, int, 0);
  assert(element == 0);
}

void test_basic_remove() {
  HASHMAP *map = hashmap_init();
  hashmap_insert_int(map, "mykey", 27);
  hashmap_insert_int(map, "yourkey", 28);

  hashmap_remove(map, "yourkey");

  int value;
  int error = hashmap_get_int(map, "mykey", &value);
  assert(error == 0);
  assert(value == 27);

  error = hashmap_get_int(map, "yourkey", &value);
  assert(error == 1);
}

int main(int argc, char **args) {
  test_basic_insert_get();
  test_basic_insert_get_list();
  test_basic_remove();
  test_collisions_insert_get();
  test_collisions_remove_head();
  test_collisions_remove_middle();
  test_collisions_remove_tail();
  test_get_non_existant();
  test_resize();
}
