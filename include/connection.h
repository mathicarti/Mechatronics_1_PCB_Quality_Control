#ifndef CONNECTION_H
#define CONNECTION_H

#include "types.h"
#include "bitmap.h"

// Owner map value for pixels not covered by any component box
#define NO_OWNER -1

// Up, down, left, right only; diagonals do not count as connected
#define NEIGHBOUR_COUNT 4

// A pixel position on the board (row 0 is the bottom)
typedef struct
{
    int row;
    int col;
} Point;

// Fills in the connected[] list of every component in the list
// board_image: must already be thresholded
// If memory allocation fails the components are left with no connections
void find_all_connections(Component_Node **component_node_head, Bmp board_image);

// Finds the connections of a single component and stores them in ascending id order
// owner: map from create_owner_map, one int per pixel
void find_connections(Component *origin, int component_count, Bmp board_image, int *owner);

// Prints one "Component X connected to ..." line per component
void print_component_connections(Component_Node *component_node_head);

// Returns the number of nodes in the list (0 for an empty list)
int count_components(Component_Node *component_node_head);

// Appends component_id to connected[] and increments *count
// Does nothing if the list is already full (MAX_COMPONENT entries)
void add_connection(int *connected, uint8_t *count, uint8_t component_id);

#endif