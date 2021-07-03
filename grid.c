#include <stdio.h>
#include <stdlib.h>

#include "grid.h"

ConwayGrid *init_grid(unsigned int width, unsigned int height) {

  ConwayGrid *result = malloc(sizeof(ConwayGrid *));

  result->matrix = malloc(sizeof(bool *) * height);

  for (unsigned int i = 0; i < height; i++) {
    result->matrix[i] = malloc(width * sizeof(bool));

    for (unsigned int j = 0; j < width; j++) {
      result->matrix[i][j] = false;
    }
  }

  result->width = width;
  result->height = height;

  return result;
}

void free_conway_grid(ConwayGrid *grid) {

  for (unsigned int i = 0; i < grid->height; i++) {
    free(grid->matrix[i]);
  }

  free(grid->matrix);
  free(grid);
}

void print_grid(ConwayGrid *grid) {
  for (unsigned int i = 0; i < grid->height; i++) {
    for (unsigned int j = 0; j < grid->width; j++) {
      printf("%c ", grid->matrix[i][j] ? 't' : 'f');
    }
    printf("\n");
  }
}
