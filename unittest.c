#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "include/list.h"

LIST *run_program_and_get_output(char *test_file_name) {
	LIST *output = LIST_INIT(char*, 64);

	char command[256];
	// sprintf(command, "clang -g -fsanitize=address -fsanitize=undefined ./unittest/%s.c ./hashmap.c -o ./unittest/%s 2>&1", test_file_name, test_file_name, test_file_name);
	sprintf(command, "clang ./unittest/%s.c ./hashmap.c -o ./unittest/%s 2>&1", test_file_name, test_file_name);
	system(command);

	sprintf(command, "./unittest/%s 2>&1", test_file_name);
	FILE *file = popen(command, "r");
	char *line = malloc(sizeof(char) * 256);
	while (fgets(line, 256, file)) {
		LIST_APPEND(output, char*, line);
		line = malloc(sizeof(char) * 256);
	}
	free(line);
	pclose(file);

	sprintf(command, "rm -f ./unittest/%s ./unittest/%s.o", test_file_name, test_file_name);
	system(command);
	return output;
}

bool run_test(char *file_name)
{
	bool passed = true;
	printf("----------%s----------\n", file_name);
	LIST* output = run_program_and_get_output(file_name);
	if (output->size == 0) {
		printf("PASSED\n");
	} else {
		passed = false;
		printf("FAILED\n");
		for (int i = 0; i < output->size; i++) {
			printf("%s\n", LIST_GET(output, char*, i));
		}
	}
	printf("----------%s----------\n\n", file_name);
	return passed;
}

int main(int argc, char** args) {
	FILE *file = popen("ls -1 ./unittest", "r");
	char line[256];
	int success_count = 0;
	int total_count = 0;
	while (fgets(line, sizeof(line), file)) {
		line[strcspn(line, ".")] = 0;
        bool result = run_test(line);
		success_count += result;
		total_count++;
	}
	pclose(file);
	printf("Passed %d/%d\n", success_count, total_count);
	// run_test("sad_unexpected_token.msh");
	return 0;
}

