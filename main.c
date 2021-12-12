#include <stdbool.h>
#include <stdio.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#include "grid.h"
#include "utils.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

bool between(int p, int x1, int x2) { return p > x1 && p <= x2; }

bool mouse_between(ALLEGRO_EVENT event, int x1, int x2, int y1, int y2) {
  return between(event.mouse.x, x1, x2) && between(event.mouse.y, y1, y2);
}

void render_loop(ALLEGRO_EVENT_QUEUE *queue) {

  ALLEGRO_TIMER *logic_timer = al_create_timer(0.25);

  al_register_event_source(queue, al_get_timer_event_source(logic_timer));
  al_start_timer(logic_timer);

  ALLEGRO_EVENT event;

  bool redraw = false, is_autoplay = false;

  ALLEGRO_COLOR background = al_map_rgb(128, 128, 128),
                active = al_map_rgb(247, 214, 208),
                inactive = al_map_rgb(255, 255, 240);

  int SIZE = 20, SQUARES = 120;

  ConwayGrid *grid = init_grid(SQUARES, SQUARES);

  while (true) {

    al_wait_for_event(queue, &event);

    if (is_key_pressed(event, ALLEGRO_KEY_ESCAPE) ||
        is_key_pressed(event, ALLEGRO_KEY_Q) ||
        (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)) {
      break;
    }

    if (event.type == ALLEGRO_EVENT_TIMER) {
      redraw = true;
    }

    if (is_key_pressed(event, ALLEGRO_KEY_P)) {
      is_autoplay = !is_autoplay;
    }

    if (is_key_pressed(event, ALLEGRO_KEY_SPACE) && !is_autoplay) {
      advance_grid(grid);
    }

    if (is_key_pressed(event, ALLEGRO_KEY_C)) {

      is_autoplay = false;

      for (int i = 0; i < SQUARES; i++) {
        for (int j = 0; j < SQUARES; j++) {
          Cell *current_cell = &grid->matrix[i][j];
          current_cell->is_active = false;
        }
      }
    }

    if (event.timer.source == logic_timer) {
      if (is_autoplay) {
        advance_grid(grid);
      }

    }

    bool is_mouse_move = event.type == ALLEGRO_EVENT_MOUSE_AXES;
    bool is_mouse_click = event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN;

    if (is_mouse_move || is_mouse_click) {

      for (int i = 0; i < SQUARES; i++) {
        for (int j = 0; j < SQUARES; j++) {

          int x1 = j * SIZE, y1 = i * SIZE, x2 = x1 + SIZE, y2 = y1 + SIZE;

          Cell *current_cell = &grid->matrix[i][j];

          bool is_between = mouse_between(event, x1, x2, y1, y2);

          current_cell->is_hover = is_between;

          if (is_mouse_click && is_between) {
            current_cell->is_active = !current_cell->is_active;
          }
        }
      }
    }

    if (redraw && al_is_event_queue_empty(queue)) {

      al_clear_to_color(background);

      for (int i = 0; i < SQUARES; i++) {
        for (int j = 0; j < SQUARES; j++) {

          int x1 = j * SIZE, y1 = i * SIZE;

          Cell *cell = &grid->matrix[i][j];

          if (cell->is_active || cell->is_hover) {
            al_draw_filled_rectangle(x1, y1, x1 + SIZE, y1 + SIZE,
                                     is_autoplay ? active : inactive);
          } else {
            al_draw_rectangle(x1, y1, x1 + SIZE, y1 + SIZE, background, 1.0);
          }
        }
      }

      al_flip_display();

      redraw = false;
    }
  }

  al_destroy_timer(logic_timer);

  free_grid(grid);
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
