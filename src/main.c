#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "bitmap.h"

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


int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("Invalid arguements!\n");
        exit(1);
    }

    Input_request request;
    
    if (!strcmp(argv[1], "t"))
        request.mode = TEMPLATE;
    else if (!strcmp(argv[1], "l"))
        request.mode = COMPONENT_LIST;
    else if (!strcmp(argv[1], "l"))
        request.mode = CHECK_CONNECTIONS;
    else
    {
        printf("Invalid mode selected!\n");
        exit(1);
    }

    strcpy(argv[2], request.template_filename);
    strcat(request.template_filename, ".bmp");

    FILE *fp = fopen(request.template_filename, "r");
    
    if (fp == NULL)
    {
        printf("Can't load template file\n");
        exit(1);
    }


    return 0;
}
