#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv) {
  char command[256];
  char user_input[256];
  char program_output[256];
  sprintf(command, "../mishmash -f %s", argv[1]);
  FILE *program = popen(command, "r");

  int piperx_file;
  int pipetx_file;
  char *piperx = "/tmp/mashtx";
  mkfifo(piperx, 0666);
  char *pipetx = "/tmp/mashrx";
  mkfifo(pipetx, 0666);

  while(1) {
	fgets(user_input, sizeof(user_input), stdin);
  	pipetx_file = open(pipetx, O_WRONLY);
	write(pipetx_file, user_input, sizeof(user_input));
	close(pipetx_file);

	piperx_file = open(piperx, O_RDONLY);
	read(piperx_file, program_output, sizeof(program_output));
        printf("%s", program_output);
  }
  pclose(program);
}

