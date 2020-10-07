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
#include "../include/stb_ds.h"
#include "SDL.h"

#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 400

#define CELL_WIDTH 5
#define CELL_HEIGHT 5

#define DELAY 75

void cgol_life_matrix_show_sdl(cgol_life_matrix *m, SDL_Renderer *r) {
  cgol_point p;
  cgol_life l;

  for (int y = m->height - 1; y >= 0; y--) {
    for (int x = 0; x < m->width; x++) {
      p = (cgol_point){x, y};
      l = stbds_hmget(m->hash, p);

      if (l) {
        SDL_Rect fill_rect = {x * CELL_WIDTH, y * CELL_HEIGHT, CELL_WIDTH,
                              CELL_HEIGHT};
        SDL_SetRenderDrawColor(r, 0xFF, 0x00, 0x00, 0xFF);
        SDL_RenderFillRect(r, &fill_rect);
      }
    }
  }
}

static volatile bool keep_running = true;

void stop_running(int dummy) { keep_running = false; }

int main(int argc, char **argv) {
  signal(SIGINT, stop_running);
  srand(time(NULL));
  cgol_life_matrix *matrix = cgol_life_matrix_new(SCREEN_HEIGHT / CELL_HEIGHT,
                                                  SCREEN_WIDTH / CELL_WIDTH);
  cgol_life_matrix_rand(matrix);

  SDL_Window *window = NULL;
  SDL_Surface *screen_surface = NULL;
  SDL_Event e;
  SDL_Renderer *r = NULL;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n",
            SDL_GetError());
  } else {
    window = SDL_CreateWindow("cgol", SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                              SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
      fprintf(stderr, "Window could not be created! SDL_Error: %s\n",
              SDL_GetError());
    } else {
      r = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
      if (r == NULL) {
        fprintf(stderr, "Renderer could not be created! SDL Error: %s\n",
                SDL_GetError());
      }

      screen_surface = SDL_GetWindowSurface(window);

      SDL_FillRect(screen_surface, NULL,
                   SDL_MapRGB(screen_surface->format, 0xFF, 0xFF, 0xFF));

      SDL_UpdateWindowSurface(window);
    }
  }

  while (keep_running) {
    while (SDL_PollEvent(&e) != 0) {
      switch (e.type) {
      case SDL_KEYDOWN:
      case SDL_KEYUP:
        switch (e.key.keysym.sym) {
        case SDLK_SPACE:
          cgol_life_matrix_rand(matrix);
          break;
        case SDLK_ESCAPE:
          keep_running = false;
          break;
        }
        break;
      case SDL_QUIT:
        keep_running = false;
        break;
      }
    }

    SDL_SetRenderDrawColor(r, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(r);

    cgol_life_matrix_show_sdl(matrix, r);

    SDL_RenderPresent(r);
    SDL_Delay(DELAY);

    cgol_life_matrix_advance(matrix);
  }

  cgol_life_matrix_free(matrix);
  SDL_DestroyRenderer(r);
  SDL_DestroyWindow(window);

  SDL_Quit();

  return 0;
}
