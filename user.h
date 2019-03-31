#ifndef __USER_H__
#define __USER_H__

#include "screen.h"

struct screen user_screen(struct window_size size);

void user_init(struct screen* screen);
void user_loop(struct screen* screen, void *data);
void user_close(struct screen* screen);

#endif