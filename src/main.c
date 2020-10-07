#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32

#include <Windows.h>
#define clear() (system("cls"))

#else

#include <unistd.h>
#define clear() (printf("%c[2J%c[1;1H", 27, 27))

#endif

#include "../include/cgol.h"

static volatile int keep_running = 1;

void stop_running(int dummy) { keep_running = 0; }

int main(int argc, char **argv) {
  srand(time(NULL));
  signal(SIGINT, stop_running);

  if (argc != 3) {
    printf("Provide width and height as command line arguments.\n");
    return 1;
  }

  int width = atoi(argv[1]);
  int height = atoi(argv[2]);

  cgol_life_matrix *matrix = cgol_life_matrix_new(height, width);
  cgol_life_matrix_rand(matrix);

  while (keep_running) {
    cgol_life_matrix_show(matrix);
    usleep(50000);
    clear();
    cgol_life_matrix_advance(matrix);
  }

  cgol_life_matrix_free(matrix);
  return 0;
}
