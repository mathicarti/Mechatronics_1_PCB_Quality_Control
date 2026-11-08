#ifndef MATCHING_H
#define MATCHING_H

#include <stdio.h>
#include "bitmap.h"
#include "types.h"

// Reads the template library and scans the board for every template
// Returns a list ordered by row then column (NULL if nothing is found)
// Caller must free it with free_component_node
// board_image: must already be thresholded
Component_Node *get_component_list(FILE *template_fp, Bmp board_image);

// Appends a copy of component to the end of the list, creating the list if empty
// The list is appended (not prepended) to keep the required output order
void add_component_node(Component component, Component_Node **component_node_head);

// Prints "Found N components:" followed by one line per component
void print_components(Component_Node *component_node_head);

// Frees every node in the list
void free_component_node(Component_Node *component_node_head);

#endif