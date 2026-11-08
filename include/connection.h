#ifndef CONNECTION_H
#define CONNECTION_H

#include "types.h"
#include "bitmap.h"

#define NO_OWNER -1
#define NEIGHBOUR_COUNT 4

typedef struct
{
    int row;
    int col;
} Point;

// Directions for moving Down, Up, Left, Right (row 0 is the bottom of the image)
const int dir_row[NEIGHBOUR_COUNT] = {-1, 1, 0, 0};
const int dir_col[NEIGHBOUR_COUNT] = {0, 0, -1, 1};

void find_all_connections(Component_Node **component_node_head, Bmp board_image);
void find_connections(Component *origin, int component_count, Bmp board_image, int *owner);
void print_component_connections(Component_Node *component_node_head);
int count_components(Component_Node *component_node_head);
void add_connection(int *connected, uint8_t *count, uint8_t component_id);

#endif