#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "include/list.h"

LIST *get_expected_values(char *test_file_name)
{
	LIST *output = LIST_INIT(char*, 64);

	char file_path[256];
	sprintf(file_path, "./test/%s", test_file_name);
	FILE *file = fopen(file_path, "r");

	char line[256];
	while (fgets(line, sizeof(line), file)) {
		int line_index = 0;
		int value_index = 0;
		// skip whitespace
		while (line[line_index] == ' ')
			line_index++;
		if (line[0] == '/' && line[1] == '/') {
			char *value = malloc(sizeof(char) * 256);
			// skip comment and whitespace
			line_index += 2;
			while (line[line_index] == ' ')
				line_index++;
			while (line[line_index] != 0) {
				value[value_index] = line[line_index];
				value_index++;
				line_index++;
			}
			value[value_index] = 0;
			LIST_APPEND(output, char*, value);
		} else {
			break;
		}
	}
	fclose(file);
	return output;
}

LIST *run_program_and_get_output(char *test_file_name)
{
	LIST *output = LIST_INIT(char*, 64);

	char command[256];
	sprintf(command, "./mishmash -f ./test/%s 2>&1", test_file_name);
	FILE *file = popen(command, "r");
	char *line = malloc(sizeof(char) * 256);
	while (fgets(line, 256, file)) {
		LIST_APPEND(output, char*, line);
		line = malloc(sizeof(char) * 256);
	}
	free(line);
	pclose(file);
	return output;
}

void print_expected_output(LIST *expected, LIST *actual)
{
	printf("EXPECTED OUTPUT:\n");
	for (int i = 0; i < expected->size; i++) {
		printf("%s", LIST_GET(expected, char*, i));
	}
	printf("ACTUAL OUTPUT:\n");
	for (int i = 0; i < actual->size; i++) {
		printf("%s", LIST_GET(actual, char*, i));
	}
}

bool assert_output(LIST *expected, LIST *actual)
{
	if (expected->size != actual->size) {
		printf("Output has %d lines. Expected %d lines.\n",
		       actual->size, expected->size);
		print_expected_output(expected, actual);
		return false;
	}

	bool passed = true;

	for (int i = 0; i < expected->size; i++) {
		char *expected_line = LIST_GET(expected, char*, i);
		char *actual_line = LIST_GET(actual, char*, i);
		if (strcmp(expected_line, actual_line) != 0) {
			printf("Expected: %sActual: %s\n", expected_line,
			       actual_line);
            passed = false;
		};
	}
	if (!passed) {
        printf("FAIL\n");
	} else {
        printf("PASS\n");
    }
	return passed;
}

bool run_test(char *file_name)
{
	bool passed = true;
	printf("----------%s----------\n", file_name);
	LIST *expected_values = get_expected_values(file_name);
		LIST *actual_output = run_program_and_get_output(file_name);
		if (actual_output->size > 0) {
			passed = assert_output(expected_values, actual_output);
			// free_list(interpreter_output);
		}
	printf("----------%s----------\n\n", file_name);
	// free_list(expected_values);
	return passed;
}


int main(int argc, char **argv)
{
	FILE *file = popen("ls -1 ./test", "r");
	char line[256];
	int success_count = 0;
	int total_count = 0;
	while (fgets(line, sizeof(line), file)) {
		line[strcspn(line, "\n")] = 0;
        bool result = run_test(line);
		success_count += result;
		total_count++;
	}
	pclose(file);
	printf("Passed %d/%d\n", success_count, total_count);
	// run_test("sad_unexpected_token.msh");
	return 0;
}