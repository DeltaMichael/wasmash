#include <ncurses.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

WINDOW *create_newwin(int height, int width, int starty, int startx);
void destroy_win(WINDOW *local_win);

int main(int argc, char** argv) {
  // CURSES!!!

  WINDOW *prog_win;
  WINDOW *cmd_win;
  WINDOW *stack_win;
  int startx, starty, width, height;
  int ch;

  initscr();			/* Start curses mode 		*/
  cbreak();			/* Line buffering disabled, Pass on
  				 * everty thing to me 		*/
  keypad(stdscr, TRUE);		/* I need that nifty F1 	*/

  int row,col;
  getmaxyx(stdscr,row,col);

  height = 3 * row / 4;
  width = col / 2;
  starty = 0;
  startx = 0;
  refresh();
  prog_win = create_newwin(height, width, starty, startx);

  cmd_win = create_newwin(row - height, width, height, 0);
  stack_win = create_newwin(row, width, 0, width);

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
	// fgets(user_input, sizeof(user_input), stdin);
	mvwgetstr(cmd_win, 1, 1, user_input);
  	pipetx_file = open(pipetx, O_WRONLY);
	write(pipetx_file, user_input, sizeof(user_input));
	close(pipetx_file);

	piperx_file = open(piperx, O_RDONLY);
	read(piperx_file, program_output, sizeof(program_output));
    // printf("%s", program_output);
  	mvwprintw(prog_win, 1, 1, program_output);
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

