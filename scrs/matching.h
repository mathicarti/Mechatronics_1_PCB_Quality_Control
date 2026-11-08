#ifndef _MATCHING_H
#define _MATCHING_H

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include "bitmap.h"
#include "connections.h"
#include "template.h"

void handle_l_mode(Input_request request, FILE *template_fp);
void threshold_board_image(Bmp *board_image);
Component_Node *get_component_list(FILE *template_fp, Bmp board_image);

void print_components(Component_Node *component_node_head);
void add_component_node(Component component, Component_Node **component_node_head);
void free_component_node(Component_Node *component_node_head);
#endif