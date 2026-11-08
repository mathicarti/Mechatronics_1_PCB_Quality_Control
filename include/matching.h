#ifndef MATCHING_H
#define MATCHING_H

#include <stdio.h>
#include "bitmap.h"
#include "types.h"

Component_Node *get_component_list(FILE *template_fp, Bmp board_image);
void add_component_node(Component component, Component_Node **component_node_head);
void print_components(Component_Node *component_node_head);
void free_component_node(Component_Node *component_node_head);

#endif