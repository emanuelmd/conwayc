from random import randint
from functools import cached_property


class Cell:
    def __init__(self, value=False):
        self.value = value

    def __repr__(self):

        if not isinstance(self.value, bool):
            print(f"Warning: self.value is not boolean but is {self.value}")

        return "1" if self.value else "0"

    @property
    def is_dead(self):
        return not self.value


class Grid:
    def __init__(self, initial_matrix=None):
        self._matrix = initial_matrix or []

    @cached_property
    def width(self):
        return min(*(len(row) for row in self._matrix))

    @cached_property
    def height(self):
        return len(self._matrix)

    def _is_out_of_bounds(self, x, y):
        return True

    def _generate_neighbours(self, x, y):
        def is_outside(p):
            (x1, y1) = p
            return x1 > 0 and y1 > 0 and x1 < self.width and y1 < self.height

        inside_neighbours = filter(
            is_outside,
            [
                (x - 1, y),
                (x + 1, y),
                (x, y - 1),
                (x, y + 1),
                (x - 1, y - 1),
                (x - 1, y + 1),
                (x + 1, y - 1),
                (x + 1, y + 1),
            ],
        )

        return [self._matrix[j][i] for (i, j) in inside_neighbours]

    def produce_next(self):

        self._generate_neighbours(0, 0)

        changes = []

        for y in range(0, self.height):
            for x in range(0, self.width):

                alive_neighbours = sum(
                    1 if c.value else 0 for c in self._generate_neighbours(x, y)
                )

                current_cell = self._matrix[y][x]

                if alive_neighbours < 2 or alive_neighbours > 3:
                    changes.append((y, x, False))

                if current_cell.is_dead and alive_neighbours == 3:
                    changes.append((y, x, True))

        for (y, x, new_value) in changes:
            self._matrix[y][x].value = new_value

    def produce_many(self, how_many=10, display=True):

        for _ in range(0, how_many):

            if display:
                print(self)
                print()

            self.produce_next()

    def __repr__(self):
        return "\n".join([", ".join([repr(c) for c in row]) for row in self._matrix])

    @classmethod
    def from_matrix(cls, matrix):

        normalized_matrix = []

        for y in range(0, len(matrix)):
            row = []
            for x in range(0, len(matrix)):
                row.append(Cell(value=matrix[y][x] == 1))

            normalized_matrix.append(row)

        return cls(initial_matrix=normalized_matrix)


def main():

    grid = Grid.from_matrix(
        [
            [0, 0, 0, 0, 0],
            [0, 0, 0, 0, 0],
            [0, 1, 1, 1, 0],
            [0, 0, 0, 0, 0],
            [0, 0, 0, 0, 0],
        ]
    )

    grid.produce_many()


if __name__ == "__main__":
    main()
