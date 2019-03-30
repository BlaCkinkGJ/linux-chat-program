#ifndef __OUTPUT_H__
#define __OUTPUT_H__

#include "screen.h"

#define MAX_OUTPUT_LINE 15

struct screen output_screen(struct window_size size);

void output_init(struct screen* screen);
void output_loop(struct screen* screen);
void output_close(struct screen* screen);
void output_list_add(struct screen* screen, void *data);
void output_list_del(struct screen* screen);

#endif