#ifndef __OUTPUT_H__
#define __OUTPUT_H__

#include "screen.h"

struct screen output_screen(struct window_size size);

void output_init(struct screen* screen);
void output_loop(struct screen* screen, void *data);
void output_close(struct screen* screen);

#endif