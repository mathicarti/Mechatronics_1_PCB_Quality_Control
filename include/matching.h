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

typedef struct
{
    uint8_t type;
    uint8_t row;
    uint8_t column;
} Component;

// Linked list to track each component on pcb
typedef struct Component_Node
{
    Component component;
    struct Component_Node *next;
} Component_Node;

void handle_l_mode(Input_request request, FILE *template_fp);

void print_components(Component_Node *component_node_head);
void add_component_node(Component component, Component_Node **component_node_head);
void free_component_node(Component_Node *component_node_head);
#endif