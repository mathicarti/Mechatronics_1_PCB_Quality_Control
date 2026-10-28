#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include "bitmap.h"
#include "connections.h"
#include "matching.h"
#include "template.h"


void handle_t_mode(Input_request request, FILE *template_fp)
{
    uint8_t template_index;
    fread(&template_index, sizeof(uint8_t), 1, template_fp);

    if (request.index < 0 || request.index >= template_index)
    {
        printf("Template index out of range\n");
        return;
    }
    fseek(template_fp, 1 + request.index * 128, SEEK_SET);

    // 8 pixel per byte; 32 pixels per row = 4 bytes per row; 32 rows = 128 bytes
    uint8_t template_buffer[128];
    fread(template_buffer, sizeof(uint8_t), 128, template_fp);

    printf("Template data:\n");

    // Traverse image from bottom to top and get bytes
    for (int row = 31; row >= 0; row--)
    {
        for (int col_byte = 0; col_byte < 4; col_byte++)
        {
            uint8_t byte = template_buffer[row * 4 + col_byte];

            // Bitmask each bit to get pixel value
            // Reverse the printfing ord to be MSB to LSB for .bmp format
            for (int i = 7; i >= 0; i--)
            {
                uint8_t byte_buffer = byte & (1 << i);
                // Bit at position i not 1
                if (byte_buffer == 0)
                    printf(" ");
                else
                    printf("1");
            }
        }
        printf("\n");
    }

    return;
}