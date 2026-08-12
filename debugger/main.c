#include "../include/list.h"
#include <errno.h>
#include <fcntl.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

WINDOW *create_newwin(int height, int width, int starty, int startx);
void destroy_win(WINDOW *local_win);
void print_buffer(WINDOW *win, LIST *buffer);

int main(int argc, char** argv) {
  // CURSES!!!

  WINDOW *prog_win;
  WINDOW *cmd_win;
  WINDOW *stack_win;
  int prog_startx, prog_starty, prog_width, prog_height;
  int cmd_startx, cmd_starty, cmd_width, cmd_height;
  int stack_startx, stack_starty, stack_width, stack_height;
  int ch;

  initscr();			/* Start curses mode 		*/
  cbreak();			/* Line buffering disabled, Pass on
  				 * everty thing to me 		*/
  keypad(stdscr, TRUE);		/* I need that nifty F1 	*/

  int row,col;
  getmaxyx(stdscr,row,col);

  prog_height = 3 * row / 4;
  prog_width = col / 2;
  prog_starty = 0;
  prog_startx = 0;

  cmd_height = row - prog_height;
  cmd_width = prog_width;
  cmd_starty = prog_height;
  cmd_startx = 0;

  stack_height = row;
  stack_width = prog_width;
  stack_starty = 0;
  stack_startx = prog_width;

  refresh();
  prog_win = create_newwin(prog_height, prog_width, prog_starty, prog_startx);
  cmd_win = create_newwin(cmd_height, cmd_width, cmd_starty, cmd_startx);
  stack_win =
      create_newwin(stack_height, stack_width, stack_starty, stack_startx);

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

  LIST *prog_buffer = LIST_INIT(char *, 64);

  while(1) {
	// fgets(user_input, sizeof(user_input), stdin);
	mvwgetstr(cmd_win, 1, 1, user_input);
  	pipetx_file = open(pipetx, O_WRONLY);
	write(pipetx_file, user_input, sizeof(user_input));
	close(pipetx_file);

	piperx_file = open(piperx, O_RDONLY);
        ssize_t n = read(piperx_file, program_output, sizeof(program_output));
        close(piperx_file);
        if (strcmp(program_output, "END") == 0) {
          break;
        }
        LIST_APPEND(prog_buffer, char *, strdup(program_output));
        print_buffer(prog_win, prog_buffer);
        wrefresh(prog_win);
  }
  pclose(program);
}

WINDOW *create_newwin(int height, int width, int starty, int startx)
{	WINDOW *local_win;

	local_win = newwin(height, width, starty, startx);
	box(local_win, 0 , 0);		/* 0, 0 gives default characters
					 * for the vertical and horizontal
					 * lines			*/
	wrefresh(local_win);		/* Show that box 		*/

	return local_win;
}

void destroy_win(WINDOW *local_win)
{
	/* box(local_win, ' ', ' '); : This won't produce the desired
	 * result of erasing the window. It will leave it's four corners
	 * and so an ugly remnant of window.
	 */
	wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	/* The parameters taken are
	 * 1. win: the window on which to operate
	 * 2. ls: character to be used for the left side of the window
	 * 3. rs: character to be used for the right side of the window
	 * 4. ts: character to be used for the top side of the window
	 * 5. bs: character to be used for the bottom side of the window
	 * 6. tl: character to be used for the top left corner of the window
	 * 7. tr: character to be used for the top right corner of the window
	 * 8. bl: character to be used for the bottom left corner of the window
	 * 9. br: character to be used for the bottom right corner of the window
	 */
	wrefresh(local_win);
	delwin(local_win);
}

void print_buffer(WINDOW *win, LIST *buffer) {
  int height, width;
  getmaxyx(win, height, width);
  height -= 2;
  int end = buffer->size;
  int start = end - height;
  if (start < 0) {
    start = 0;
  }
  for (int i = start; i < end; i++) {
    char *line = LIST_GET(buffer, char *, i);
    mvwprintw(win, i - start + 1, 1, "%s", line);
  }
}
