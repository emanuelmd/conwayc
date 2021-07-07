#ifndef CONWAY_UTILS
#define CONWAY_UTILS

#include <allegro5/allegro.h>
#include <stdlib.h>

void must_init(bool test, const char *description);
bool is_key_pressed(ALLEGRO_EVENT event, int key);

#endif
