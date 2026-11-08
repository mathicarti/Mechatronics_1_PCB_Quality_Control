#include <stdlib.h>
#include <stdint.h>

#include "types.h"
#include "board.h"

static double average_colour(uint8_t *pixel)
{
    return (pixel[0] + pixel[1] + pixel[2]) / 3.0;
}

static void set_pixel_black(uint8_t *pixel)
{
    pixel[0] = 0;
    pixel[1] = 0;
    pixel[2] = 0;
}

static void set_pixel_white(uint8_t *pixel)
{
    pixel[0] = 255;
    pixel[1] = 255;
    pixel[2] = 255;
}

void threshold_board_image(Bmp *board_image)
{
    for (int row = board_image->height - 1; row >= 0; row --)
    {
        for (int col = 0; col < board_image->width; col ++)
        {
            uint8_t *pixel = board_image->pixels[row][col];
            // Average pixel colour and compare them to custom threshold
            if (average_colour(pixel) > THRESHOLD)
                set_pixel_white(pixel);
            else
                set_pixel_black(pixel);
        }
    }
}