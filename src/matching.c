#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "types.h"
#include "template.h"
#include "connection.h"
#include "matching.h"

// Returns 1 if both pixel have the same value, else 0
// The board is thresholded to 0 or 255 while templates hold 0 or 1, so white needs special handling
static int pixel_is_same(uint8_t template_pixel, uint8_t *board_pixel)
{
    if ((template_pixel == 1 && board_pixel[0] == 255) || (template_pixel == board_pixel[0]))
    {
        return 1;
    }
    return 0;
}

// Finds if the area of interest is the same as the template, if so returns 1, else 0
// Returns on the first mismatch, which keeps the full scan fast
static int is_template(Template template, uint8_t ***board_pixels, int board_row, int board_col)
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

// Positions are the outer loops and templates the inner one, so components are found already
// sorted by row then column and no separate sort is needed
static void find_components(Template_array template_array, Bmp board_image, Component_Node **component_node_head)
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

void print_components(Component_Node *component_node_head)
{
    printf("Found %i components:\n", count_components(component_node_head));
    for (; component_node_head != NULL; component_node_head = component_node_head->next)
        printf("type: %d, row: %d, column: %d\n", component_node_head->component.type, 
            component_node_head->component.row, component_node_head->component.column);
}