#include <stdbool.h>
#include <stdio.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#include "grid.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

#define SQUARE_SIZE 40

#define PADDING_SIZE 10

void must_init(bool test, const char *description) {
  if (test)
    return;

  printf("Couldn't initialize %s\n", description);

  exit(1);
}

bool is_key_pressed(ALLEGRO_EVENT event, int key) {
  return (event.type == ALLEGRO_EVENT_KEY_DOWN) &&
         (event.keyboard.keycode == key);
}

void randomize_grid(ConwayGrid *grid) {

  for (unsigned int i = 0; i < grid->height; i++) {
    for (unsigned int j = 0; j < grid->width; j++) {
      grid->matrix[i][j] = rand() % 2 == 0;
    }
  }
}

void render_conway_grid(ConwayGrid *grid) {

  ALLEGRO_COLOR white = al_map_rgb(255, 255, 255);

  for (unsigned int i = 0; i < grid->height; i++) {
    for (unsigned int j = 0; j < grid->width; j++) {

      int x1 = j * SQUARE_SIZE;
      int y1 = i * SQUARE_SIZE;

      if (grid->matrix[i][j] == true) {
        al_draw_filled_rectangle(x1, y1, SQUARE_SIZE, SQUARE_SIZE, white);
      } else {
        al_draw_rectangle(x1, y1, SQUARE_SIZE, SQUARE_SIZE, white, 1.0);
      }
    }
  }
}

bool between(int p, int x1, int x2) { return p > x1 && p <= x2; }

void render_loop(ALLEGRO_EVENT_QUEUE *queue) {

  ALLEGRO_EVENT event;

  bool redraw = false;

  ALLEGRO_COLOR white = al_map_rgb(255, 255, 255), black = al_map_rgb(0, 0, 0);

  int SIZE = 50, SQUARES = 20;

  bool grid[20][20] = {};

  while (true) {

    al_wait_for_event(queue, &event);

    if (event.type == ALLEGRO_EVENT_TIMER) {
      redraw = true;
    }

    if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {

      for (int i = 0; i < SQUARES; i++) {
        for (int j = 0; j < SQUARES; j++) {

          int x1 = j * SIZE, y1 = i * SIZE, x2 = x1 + SIZE, y2 = y1 + SIZE;

          grid[i][j] =
              between(event.mouse.x, x1, x2) && between(event.mouse.y, y1, y2);
        }
      }
    }

    if (is_key_pressed(event, ALLEGRO_KEY_SPACE)) {
      printf("\n");
    }

    if (redraw && al_is_event_queue_empty(queue)) {

      al_clear_to_color(black);

      for (int i = 0; i < SQUARES; i++) {
        for (int j = 0; j < SQUARES; j++) {

          int x1 = j * SIZE, y1 = i * SIZE;

          if (grid[i][j]) {
            al_draw_filled_rectangle(x1, y1, x1 + SIZE, y1 + SIZE, white);
          } else {
            al_draw_rectangle(x1, y1, x1 + SIZE, y1 + SIZE, white, 1.0);
          }
        }
      }

      al_flip_display();

      redraw = false;
    }

    if (is_key_pressed(event, ALLEGRO_KEY_ESCAPE) ||
        (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)) {
      break;
    }
  }
}

int main(void) {

  must_init(al_init(), "allegro");
  must_init(al_install_keyboard(), "allegro-keyboard");
  must_init(al_install_mouse(), "allegro-mouse");
  must_init(al_init_primitives_addon(), "allegro-primitives");

  ALLEGRO_DISPLAY *main_display =
      al_create_display(WINDOW_WIDTH, WINDOW_HEIGHT);

  must_init(main_display, "allegro-display");

  ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60.0);
  must_init(timer, "timer");

  /* Event queue */
  ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
  must_init(queue, "queue");

  al_register_event_source(queue, al_get_keyboard_event_source());
  al_register_event_source(queue, al_get_mouse_event_source());
  al_register_event_source(queue, al_get_display_event_source(main_display));
  al_register_event_source(queue, al_get_timer_event_source(timer));

  al_start_timer(timer);

  render_loop(queue);

  al_destroy_display(main_display);
  al_destroy_timer(timer);
  al_destroy_event_queue(queue);

  return 0;
}
