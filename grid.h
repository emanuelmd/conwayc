#ifndef CONWAY_GRID
#define CONWAY_GRID

#include <stdbool.h>
#include <stdlib.h>

typedef struct Grid {
  bool **matrix;
  unsigned int width;
  unsigned int height;
} ConwayGrid;

ConwayGrid *init_grid(unsigned int width, unsigned int height);

void free_conway_grid(ConwayGrid *grid);
void print_grid(ConwayGrid *grid);

#endif
