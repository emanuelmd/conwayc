#include <stdio.h>
#include <stdlib.h>

#include "grid.h"

ConwayGrid *init_grid(unsigned int width, unsigned int height) {

  ConwayGrid *result = malloc(sizeof(ConwayGrid *));

  result->matrix = malloc(sizeof(Cell *) * height);

  for (unsigned int i = 0; i < height; i++) {

    result->matrix[i] = malloc(width * sizeof(Cell));

    for (unsigned int j = 0; j < width; j++) {
      result->matrix[i][j] = (Cell){.is_hover = false, .is_active = false};
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

      Cell cell = grid->matrix[i][j];

      char hover = cell.is_hover ? 'h' : ' ';
      char active = cell.is_active ? 'a' : ' ';

      printf("%c%c ", hover, active);
    }

    printf("\n");
  }
}

// CellN -> N as in Neighbour (too cumbersome to spell)
typedef struct CellN {
  int x;
  int y;
  bool is_active;
} CellN;

int count_neighbours(ConwayGrid *grid, int x, int y) {

  int neighbours[16] = {// Top Left
                        x - 1, y - 1,
                        // Top
                        x, y - 1,
                        // Top Right
                        x + 1, y - 1,
                        // Left
                        x - 1, y,
                        // Right
                        x + 1, y,
                        // Bottom Left
                        x - 1, y + 1,
                        // Bottom Center
                        x, y + 1,
                        // Bottom Right
                        x + 1, y + 1};

  int result = 0;

  for (int i = 0; i < 16; i += 2) {
    int x1 = neighbours[i], y1 = neighbours[i + 1];

    bool is_outside =
        x1 < 0 || y1 < 0 || x1 >= grid->width || y1 >= grid->height;

    if (is_outside) {
      continue;
    }

    Cell neighbour = grid->matrix[y1][x1];

    if (neighbour.is_active) {
      result++;
    }
  }

  return result;
}

void advance_grid(ConwayGrid *grid) {

  CellN changes[grid->width * grid->height];

  int index = 0;

  for (unsigned int y = 0; y < grid->height; y++) {
    for (unsigned int x = 0; x < grid->width; x++) {

      int alive_neighbours = count_neighbours(grid, x, y);

      Cell current_cell = grid->matrix[y][x];

      if (current_cell.is_active &&
          (alive_neighbours < 2 || alive_neighbours > 3)) {
        changes[index++] = (CellN){.x = x, .y = y, .is_active = false};
      }

      if (!current_cell.is_active && alive_neighbours == 3) {
        changes[index++] = (CellN){.x = x, .y = y, .is_active = true};
      }
    }
  }

  for (int i = 0; i < index; i++) {
    CellN change = changes[i];
    grid->matrix[change.y][change.x].is_active = change.is_active;
  }
}
