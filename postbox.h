#ifndef __POSTBOX_H__
#define __POSTBOX_H__

#include "screen.h"
#include <stdbool.h>

// this for share memory
struct postbox {
    char output [MAX_OUTPUT_LINE][MAX_OUTPUT_WIDTH];
    char user   [MAX_USER_LINE][MAX_USER_WIDTH];
};

#endif