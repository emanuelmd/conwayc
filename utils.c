
#include <allegro5/allegro.h>
#include <stdlib.h>

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
