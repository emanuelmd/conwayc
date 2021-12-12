#ifndef CONWAY_GRID
#define CONWAY_GRID

#include <stdbool.h>
#include <stdlib.h>

typedef struct Cell {
  bool is_hover;
  bool is_active;
} Cell;

typedef struct Grid {
  Cell **matrix;
  unsigned int width;
  unsigned int height;
} ConwayGrid;

ConwayGrid *init_grid(unsigned int width, unsigned int height);

void free_grid(ConwayGrid *grid);
void print_grid(ConwayGrid *grid);
void advance_grid(ConwayGrid *grid);

#endif
