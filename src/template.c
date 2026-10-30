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
    // Load the binary file as a bmp style template array
    Template_array template_array = read_template_array(template_fp);

    // Check if index in range
    if (request.index < 0 || request.index >= template_array.count)
    {
        printf("Template index out of range\n");
        return;
    }
    // Print template at index
    print_template(template_array.template[request.index]);

    free_template_array(&template_array);
}

// Transforms a binary template to a bmp arranged template array
Template_array read_template_array(FILE *template_fp)
{
    Template_array template_array;
    fread(&template_array.count, sizeof(uint8_t), 1, template_fp);

    template_array.template = (uint8_t ***) malloc(sizeof(uint8_t **) * template_array.count);

    for (int i = 0; i < template_array.count; i++)
    {
        // Allocate memory to each template at their respective index
        template_array.template[i] = (uint8_t **) malloc(TEMPLATE_HEIGHT * sizeof(uint8_t *));

        for (int row = 0; row < TEMPLATE_HEIGHT; row++)
        {
            template_array.template[i][row] = (uint8_t *) malloc(TEMPLATE_WIDTH * sizeof(uint8_t));
        }
        // Set binary template data to each template at their respective index
        get_template_at_index(template_array.template[i], i, template_fp);
    }  
    return template_array;
}

void get_template_at_index(Template template, int template_index, FILE *template_fp)
{
    // 8 pixel per byte; 32 pixels per row = 4 bytes per row; 32 rows = 128 bytes
    uint8_t template_buffer[128];
    fseek(template_fp, 1 + template_index * 128, SEEK_SET);
    fread(template_buffer, sizeof(uint8_t), 128, template_fp);

    arrange_template(template, template_buffer);
}

// Arranges a binary template to a bmp style template
void arrange_template(Template template, uint8_t *template_buffer)
{
    for (int row = TEMPLATE_HEIGHT - 1; row >= 0; row--)
    {
        for (int col_byte = 0; col_byte < TEMPLATE_WIDTH_BYTE; col_byte++)
        {
            // Each byte has 8 pixels, and for the in the format one pixel is a byte of its own
            // Bitmask each bit to get pixel value; reverse the printing order to be MSB to LSB for bmp style format
            for (int i = 0; i < BYTE; i++)
            {
                // A 1 is a white pixel and 0 is a black pixel
                template[row][col_byte * BYTE + i] = template_buffer[row * TEMPLATE_WIDTH_BYTE + col_byte] & 1 << (BYTE -1 - i);
            }
        }
    }
}

// Frees a template array
void free_template_array(Template_array *template_array)
{
    for (int i = 0; i < template_array->count; i++)
    {
        for (int row = 0; row < TEMPLATE_HEIGHT; row++)
        {
            free(template_array->template[i][row]);
        }
        free(template_array->template[i]);
    }
    free(template_array->template);
}

void print_template(Template template)
{
    printf("Template data:\n");

    // Traverse image from bottom to top and get bytes
    for (int row = TEMPLATE_HEIGHT - 1; row >= 0; row--)
    {
        for (int col = 0; col < TEMPLATE_WIDTH; col++)
        {
            print_pixel(template[row][col]);
        }
        printf("\n");
    }
}

void print_pixel(uint8_t pixel)
{
    // Pixel either 1 for white or 0 for black
    if (pixel)
        printf("1");
    else
        printf(" ");
}
