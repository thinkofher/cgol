#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32

#include <Windows.h>
#define clear() (system("cls"))

#else

#include <unistd.h>
#define clear() (system("clear"))

#endif

#include "cgol.h"


static volatile int keep_running = 1;

void stop_running(int dummy) { keep_running = 0; }

int main(int argc, char **argv) {
  srand(time(NULL));
  signal(SIGINT, stop_running);

  cgol_life_matrix *matrix = cgol_life_matrix_new(30, 30);
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
