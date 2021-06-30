#include <stdio.h>
#include <stdbool.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

#define SQUARE_SIZE 80

void must_init(bool test, const char *description) {
	if (test) return;

	printf("Couldn't initialize %s\n", description);

	exit(1);
}

void draw_grid() {

	ALLEGRO_COLOR white = al_map_rgb(255, 255, 255);

	for (int i = 0; i <= WINDOW_WIDTH / 10; i++) {
		for (int j = 0; j <= WINDOW_HEIGHT / 10; j++) {

			int x = i * SQUARE_SIZE;
			int y = j * SQUARE_SIZE;

			al_draw_rectangle(x, y, x + SQUARE_SIZE, y + SQUARE_SIZE, white, 1.0f);
		}
	}
}

void generate_grid() {
	int m[SQUARE_SIZE / 10][SQUARE_SIZE / 10];
}

int main(void) {

	must_init(al_init(), "allegro");
	must_init(al_install_mouse(), "allegro-mouse");
	must_init(al_install_keyboard(), "allegro-keyboard");
	must_init(al_init_image_addon(), "allegro-image");
	must_init(al_init_primitives_addon(), "allegro-primitives");

	ALLEGRO_DISPLAY* main_display = al_create_display(WINDOW_WIDTH, WINDOW_HEIGHT);
	must_init(main_display, "allegro-display");

	ALLEGRO_TIMER* timer = al_create_timer(1.0 / 10.0);
	must_init(timer, "timer");

	/* Event queue */
	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
	must_init(queue, "queue");

	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(main_display));
	al_register_event_source(queue, al_get_timer_event_source(timer));
	al_register_event_source(queue, al_get_mouse_event_source());

	/* Colors */
	ALLEGRO_COLOR black = al_map_rgb(0, 0, 0);

	bool redraw = true;

	ALLEGRO_EVENT event;

	al_start_timer(timer);

	while (1)
	{
		al_wait_for_event(queue, &event);

		if (event.type == ALLEGRO_EVENT_TIMER) {
			redraw = true;
		}

		if (event.type == ALLEGRO_EVENT_KEY_DOWN || event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			break;
		}

		if (redraw && al_is_event_queue_empty(queue)) {

			al_clear_to_color(black);
			draw_grid();

			al_flip_display();

			redraw = false;
		}
	}

	al_destroy_display(main_display);
	al_destroy_timer(timer);
	al_destroy_event_queue(queue);

	return 0;
}
