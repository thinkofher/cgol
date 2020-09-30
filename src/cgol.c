#include <stdbool.h>
#include <stdio.h>

#define STB_DS_IMPLEMENTATION
#include "../libs/stb_ds.h"

#include "cgol.h"

/* cgol_life methods */

char cgol_life_show(cgol_life l) { return l ? '*' : ' '; }

cgol_life cgol_life_advance(cgol_life l, int neighbours) {
  if (l && (neighbours == 2 || neighbours == 3))
    return cgol_life_alive;
  else if (!l && (neighbours == 3))
    return cgol_life_alive;
  else
    return cgol_life_dead;
}

/* cgol_point methods */

cgol_point cgol_point_add(cgol_point a, cgol_point b) {
  return (cgol_point){a.x + b.x, a.y + b.y};
}

/* cgol_life_matrix methods */

cgol_life_matrix *cgol_life_matrix_new(int height, int width) {
  cgol_life_matrix *res = (cgol_life_matrix *)malloc(sizeof(cgol_life_matrix));

  res->hash = NULL;
  res->height = height;
  res->width = width;

  cgol_point p;
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      p = (cgol_point){x, y};
      stbds_hmput(res->hash, p, cgol_life_dead);
    }
  }

  return res;
}

static const cgol_point neighbours_cords_diff[8] = {
    {-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1},
};

int cgol_life_matrix_neighbours(cgol_life_matrix *m, int x, int y) {
  cgol_point neighbour;
  cgol_point target = (cgol_point){x, y};

  if (stbds_hmgeti(m->hash, target) < 0) {
    return -1;
  }

  cgol_life l;
  int result = 0;

  for (int i = 0; i < 8; i++) {
    neighbour = cgol_point_add(target, neighbours_cords_diff[i]);
    if (stbds_hmgeti(m->hash, neighbour) > 0) {
      l = stbds_hmget(m->hash, neighbour);
      result += (l ? 1 : 0);
    }
  }

  return result;
}

int cgol_life_matrix_neighbours_point(cgol_life_matrix *m, cgol_point p) {
  return cgol_life_matrix_neighbours(m, p.x, p.y);
}

void cgol_life_matrix_rand(cgol_life_matrix *m) {
  for (int i = 0; i < stbds_hmlen(m->hash); i++) {
    m->hash[i].value = rand() % 2;
  }
}

void cgol_life_matrix_advance(cgol_life_matrix *m) {
  cgol_life_matrix *tmp = cgol_life_matrix_new(0, 0);
  cgol_life_matrix_copy(m, tmp);

  int neighbours;
  for (int i = 0; i < stbds_hmlen(m->hash); i++) {
    neighbours = cgol_life_matrix_neighbours_point(tmp, m->hash[i].key);
    stbds_hmput(m->hash, m->hash[i].key,
                cgol_life_advance(m->hash[i].value, neighbours));
  }

  cgol_life_matrix_free(tmp);
}

void cgol_life_matrix_show(cgol_life_matrix *m) {
  cgol_point p;
  cgol_life l;

  for (int y = m->height - 1; y >= 0; y--) {
    for (int x = 0; x < m->width; x++) {
      p = (cgol_point){x, y};
      l = stbds_hmget(m->hash, p);
      putchar(cgol_life_show(l));
    }
    putchar('\n');
  }
}

void cgol_life_matrix_copy(cgol_life_matrix *src, cgol_life_matrix *dst) {
  dst->height = src->height;
  dst->width = src->width;
  stbds_hmfree(dst->hash);

  for (int i = 0; i < stbds_hmlen(src->hash); i++) {
    stbds_hmput(dst->hash, src->hash[i].key, src->hash[i].value);
  }
}

void cgol_life_matrix_free(cgol_life_matrix *m) {
  stbds_hmfree(m->hash);
  free(m);
}
