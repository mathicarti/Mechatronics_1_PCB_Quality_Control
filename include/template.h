#ifndef _TEMPLATE_H
#define _TEMPLATE_H

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

#define FILENAME_BUFFER 512
#define TEMPLATE_HEIGHT 32
#define TEMPLATE_WIDTH 32
#define TEMPLATE_WIDTH_BYTE 4
#define BYTE 8

typedef uint8_t** Template;

typedef struct
{
    // The number of templates in array
    uint8_t count;

    // Array with templates that have rows and columns with a byte for a single pixel value (1 white, 0 black)
    Template *template;
} Template_array;

typedef enum
{
    UNDEFINED,
    TEMPLATE,
    COMPONENT_LIST,
    CHECK_CONNECTIONS
} Mode_type;

typedef struct
{
    Mode_type mode;
    char template_filename[FILENAME_BUFFER];
    int index;
    char board_image_filename[FILENAME_BUFFER];
} Input_request;

void handle_t_mode(Input_request request, FILE *template_fp);
void print_template(Template template);
void print_pixel(uint8_t pixel);

Template_array read_template_array(FILE *template_fp);
void get_template_at_index(Template template, int template_index, FILE *template_fp);
void arrange_template(Template template, uint8_t *template_buffer);
void free_template_array(Template_array *template_array);

#endif