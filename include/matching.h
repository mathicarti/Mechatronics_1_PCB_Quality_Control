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

#define THRESHOLD 127.00
#define MAX_COMPONENT 128

// Struct that keeps track of the components id, type, position and its connections
typedef struct
{
    uint8_t id;
    uint8_t type;
    uint16_t row;
    uint16_t column;
    int connected[MAX_COMPONENT];
    uint8_t count;
} Component;

// Linked list to track each component on board
typedef struct Component_Node
{
    Component component;
    struct Component_Node *next;
} Component_Node;

void handle_l_mode(Input_request request, FILE *template_fp);
void threshold_board_image(Bmp *board_image);
Component_Node *get_component_list(FILE *template_fp, Bmp board_image);

void print_components(Component_Node *component_node_head);
void add_component_node(Component component, Component_Node **component_node_head);
void free_component_node(Component_Node *component_node_head);
#endif