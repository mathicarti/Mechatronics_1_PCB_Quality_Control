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

double average_colour(uint8_t *pixel)
{
    return (pixel[0] + pixel[1] + pixel[2]) / 3;
}

void set_pixel_black(uint8_t *pixel)
{
    pixel[0] = 0;
    pixel[1] = 0;
    pixel[2] = 0;
}

void set_pixel_white(uint8_t *pixel)
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
            if (average_colour(pixel) < THRESHOLD)
                set_pixel_black(pixel);
            else
                set_pixel_white(pixel);
        }
    }
}

void find_components(Template template, Bmp board_image, Component_Node **component_node_head)
{
    ;
}

void handle_l_mode(Input_request request, FILE *template_fp)
{
    Component_Node *component_node_head = NULL;

    // load bmp and threshold it
    Bmp board_image = read_bmp(request.board_image_filename);
    threshold_board_image(&board_image);

    // Load all templates in an array
    Template_array template_array = read_template_array(template_fp);
    
    // For each template go through the pcb to see if it is there add its position to the component list
    for (int i = 0; i < template_array.count; i++)
    {
        // Search through the board and add components as needed
        find_components(template_array.template[i], board_image, component_node_head);
    }
    // print component type and positions
    print_components(component_node_head);

    // free variables
    free_bmp(board_image);
    free_component_node(component_node_head);
    free_template_array(&template_array);
    return;
}

void print_components(Component_Node *component_node_head)
{
    // Go through the linked list and count nodes
    int count = 0;
    for (; component_node_head; count++);

    printf("Found %i components:\n");
    while (component_node_head)
    {
        printf("type: %d, row: %d, column: %d\n", component_node_head->component.type, 
            component_node_head->component.row, component_node_head->component.column);
    }
}

void add_component_node(Component component, Component_Node **component_node_head)
{
    Component_Node *component_node = (Component_Node *) malloc(sizeof(Component_Node));

    component_node->component = component;
    component_node->next = *component_node_head;

    *component_node_head = component_node;
}

void free_component_node(Component_Node *component_node_head)
{
    while (component_node_head)
    {
        Component_Node *temp = component_node_head;
        component_node_head = component_node_head->next;

        free(temp);
    }
}
