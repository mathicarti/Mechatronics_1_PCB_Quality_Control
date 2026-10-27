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
#endif