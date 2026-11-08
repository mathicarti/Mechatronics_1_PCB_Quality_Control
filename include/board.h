#ifndef BOARD_H
#define BOARD_H

#include "bitmap.h"

// Converts every pixel to pure black or pure white in place, so later code
// can test a single channel instead of averaging again
// board_image: must be a loaded image; modified directly
void threshold_board_image(Bmp *board_image);

#endif