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

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        printf("Invalid arguements!\n");
        exit(1);
    }

    Input_request request;
    
    if (!strcmp(argv[1], "t"))
    {
        request.mode = TEMPLATE;
        request.index = atoi(argv[3]);
    }
    else if (!strcmp(argv[1], "l"))
    {
        request.mode = COMPONENT_LIST;
        strcpy(request.board_image_filename, argv[3]);
    }
    else if (!strcmp(argv[1], "c"))
    {
        request.mode = CHECK_CONNECTIONS;
        strcpy(request.board_image_filename, argv[3]);
    }
    else
    {
        request.mode = UNDEFINED;
        printf("Invalid mode selected!\n");
        exit(2);
    }

    strcpy(request.template_filename, argv[2]);

    FILE *template_fp = fopen(request.template_filename, "rb");
    
    if (template_fp == NULL)
    {
        printf("Can't load template file\n");
        exit(3);
    }

    switch (request.mode)
    {
    case TEMPLATE:
        if (request.index < 0)
        {
            printf("Template index out of range\n");
            fclose(template_fp);
            exit(2);
        }
        handle_t_mode(request, template_fp);
        break;

    case COMPONENT_LIST:
        handle_l_mode(request, template_fp);
        break;
    
    case CHECK_CONNECTIONS:
        handle_c_mode(request, template_fp);
        break;

    default:
        fclose(template_fp);
        exit(3);
        break;
    }

    fclose(template_fp);
    return 0;
}
