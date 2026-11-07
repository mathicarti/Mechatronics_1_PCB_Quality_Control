#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include "bitmap.h"
#include "connections.h"
#include "matching.h"
#include "template.h"

double average_colour(uint8_t *pixel)
{
    return (pixel[0] + pixel[1] + pixel[2]) / 3.0;
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
            if (average_colour(pixel) > THRESHOLD)
                set_pixel_white(pixel);
            else
                set_pixel_black(pixel);
        }
    }
}

// Returns 1 if both pixel have the same value, else 0
int pixel_is_same(uint8_t template_pixel, uint8_t *board_pixel)
{
    if ((template_pixel == 1 && board_pixel[0] == 255) || (template_pixel == board_pixel[0]))
    {
        return 1;
    }
    return 0;
}

// Finds if the area of interest is the same as the template, if so returns 1, else 0
int is_template(Template template, uint8_t ***board_pixels, int board_row, int board_col)
{
    for (int row = 0; row < TEMPLATE_HEIGHT; row++)
    {
        for (int col = 0; col < TEMPLATE_WIDTH; col++)
        {
            if (!pixel_is_same(template[row][col], board_pixels[board_row + row][board_col + col]))
                return 0;
        }
    }
    return 1;
}

void find_components(Template template, uint8_t template_index, Bmp board_image, Component_Node **component_node_head)
{
    // Go through all possible row where the template could fit
    for (int board_row = 0; board_row < board_image.height - TEMPLATE_HEIGHT; board_row++)
    {
        // Go through all possible column where the template could fit
        for (int board_col = 0; board_col < board_image.width - TEMPLATE_WIDTH; board_col++)
        {
            if (is_template(template, board_image.pixels, board_row, board_col))
            {
                Component new_component = { template_index, board_row, board_col };
                add_component_node(new_component, component_node_head);
            }
        }
    }
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
        find_components(template_array.template[i], i, board_image, &component_node_head);
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
    for (Component_Node *node_pointer = component_node_head; node_pointer != NULL; count++) node_pointer = node_pointer->next;

    printf("Found %i components:\n", count);
    for (; component_node_head != NULL; component_node_head = component_node_head->next)
        printf("type: %d, row: %d, column: %d\n", component_node_head->component.type, 
            component_node_head->component.row, component_node_head->component.column);
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
