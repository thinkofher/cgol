typedef bool cgol_life;

#define cgol_life_alive true
#define cgol_life_dead false

typedef struct __point {
  int x, y;
} cgol_point;

typedef struct __matrix {
  cgol_point key;
  cgol_life value;
} cgol_matrix;

typedef struct __life_matrix {
  cgol_matrix *hash;
  int height, width;
} cgol_life_matrix;

char cgol_life_show(cgol_life l);
cgol_life cgol_life_advance(cgol_life l, int neighbours);

cgol_point cgol_point_add(cgol_point a, cgol_point b);

cgol_life_matrix *cgol_life_matrix_new(int height, int width);
int cgol_life_matrix_neighbours(cgol_life_matrix *m, int x, int y);
int cgol_life_matrix_neighbours_point(cgol_life_matrix *m, cgol_point p);
void cgol_life_matrix_rand(cgol_life_matrix *m);
void cgol_life_matrix_advance(cgol_life_matrix *m);
void cgol_life_matrix_show(cgol_life_matrix *m);
void cgol_life_matrix_copy(cgol_life_matrix *src, cgol_life_matrix *dst);
void cgol_life_matrix_clean(cgol_life_matrix *m);
void cgol_life_matrix_free(cgol_life_matrix *m);
