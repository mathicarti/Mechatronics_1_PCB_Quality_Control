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
    // 8 pixel per byte; 32 pixels per row = 4 bytes per row; 32 rows = 128 bytes
    uint8_t template_buffer[128];

    if (get_template(template_buffer, request.index, template_fp))
    {
        printf("Template index out of range\n");
        return;
    }
    print_template(template_buffer);

    return;
}

int get_template(uint8_t *template_buffer, int template_index, FILE *template_fp)
{
    uint8_t template_fp_index;
    fread(&template_fp_index, sizeof(uint8_t), 1, template_fp);

    if (template_index < 0 || template_index >= template_fp_index)
        return 1;

    fseek(template_fp, 1 + template_index * 128, SEEK_SET);
    fread(template_buffer, sizeof(uint8_t), 128, template_fp);
    
    return 0;
}

void print_template(uint8_t *template)
{
    printf("Template data:\n");

    // Traverse image from bottom to top and get bytes
    for (int row = 31; row >= 0; row--)
    {
        for (int col_byte = 0; col_byte < 4; col_byte++)
        {
            uint8_t byte = template[row * 4 + col_byte];
            print_byte(byte);
        }
        printf("\n");
    }
}

void print_byte(uint8_t byte)
{
    // Bitmask each bit to get pixel value
    // Reverse the printfing ord to be MSB to LSB for .bmp format
    for (int i = 7; i >= 0; i--)
    {
        uint8_t byte_buffer = byte & (1 << i);
        // Bit at position i is not 1
        if (byte_buffer == 0)
            printf(" ");
        else
            printf("1");
    }
    return;
}
