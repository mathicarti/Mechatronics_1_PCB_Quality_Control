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

void find_components(Template_array template_array, Bmp board_image, Component_Node **component_node_head)
{
    // id used to differentiate between each components
    uint8_t id = 0;
    // Go through all possible row where the template could fit
    for (int board_row = 0; board_row <= board_image.height - TEMPLATE_HEIGHT; board_row++)
    {
        // Go through all possible column where the template could fit
        for (int board_col = 0; board_col <= board_image.width - TEMPLATE_WIDTH; board_col++)
        {
            // Search through specific area for any template match and add components as needed
            for (int i = 0; i < template_array.count; i++)
            {
                if (is_template(template_array.template[i], board_image.pixels, board_row, board_col))
                {
                    Component new_component = {id, i, board_row, board_col, {-1}, 0};
                    add_component_node(new_component, component_node_head);
                    id++;
                }
            }
        }
    }
}

void handle_l_mode(Input_request request, FILE *template_fp)
{
    // load bmp and threshold it
    Bmp board_image = read_bmp(request.board_image_filename);
    threshold_board_image(&board_image);
    
    // Search through board for any matching templates and add it to list
    // Get the list of components and positions on the board
    Component_Node *component_node_head = get_component_list(template_fp, board_image);

    // print component type and positions
    print_components(component_node_head);

    // free variables
    free_bmp(board_image);
    free_component_node(component_node_head);
    return;
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

// Return a list of all the components on a thresholded board
Component_Node *get_component_list(FILE *template_fp, Bmp board_image)
{
    Component_Node *component_node_head = NULL;

    // Load all templates in an array
    Template_array template_array = read_template_array(template_fp);
    
    // Search through board for any matching templates and add it to list
    find_components(template_array, board_image, &component_node_head);
    
    free_template_array(&template_array);
    return component_node_head;
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

// Adds a Component to the end of a linked list
void add_component_node(Component component, Component_Node **component_node_head)
{
    // Make a new end component node
    Component_Node *component_node = (Component_Node *) malloc(sizeof(Component_Node));
    component_node->component = component;
    component_node->next = NULL;

    // Check of list is empty
    if (*component_node_head == NULL)
    {
        *component_node_head = component_node;
        return;
    }
    // Go to the end of the list
    Component_Node *node_pointer = *component_node_head;
    for (; node_pointer->next != NULL; node_pointer = node_pointer->next);
    // Set it at the end of list
    node_pointer->next = component_node;
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
