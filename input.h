#ifndef __INPUT_H__
#define __INPUT_H__

#include "screen.h"

struct screen input_screen(struct window_size size);

void input_init(struct screen* screen);
void input_loop(struct screen* screen, void *data);
void input_close(struct screen* screen);

#endif