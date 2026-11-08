#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "types.h"
#include "bitmap.h"
#include "connection.h"

// Converts a point to its index in the flat visited/owner arrays
static int pixel_index(Point point, Bmp board_image)
{
    return point.row * board_image.width + point.col;
}

static int is_in_bounds(Point point, Bmp board_image)
{
    return (point.row >= 0 && point.row < (int) board_image.height
        && point.col >= 0 && point.col < (int) board_image.width);
}

// Counts the components in the linked list
int count_components(Component_Node *component_node_head)
{
    int count = 0;
    for (; component_node_head != NULL; component_node_head = component_node_head->next)
        count++;
    return count;
}

// Makes a flat map of the board holding the id of the component whose 32x32 box covers
// each pixel, or NO_OWNER if no box covers it. Returns NULL if allocation fails
static int *create_owner_map(Component_Node *component_node_head, Bmp board_image)
{
    size_t pixel_count = (size_t) board_image.width * board_image.height;
    int *owner = (int *) malloc(sizeof(int) * pixel_count);
    if (owner == NULL)
        return NULL;

    for (size_t i = 0; i < pixel_count; i++)
        owner[i] = NO_OWNER;

    for (; component_node_head != NULL; component_node_head = component_node_head->next)
    {
        Component component = component_node_head->component;
        for (int row = 0; row < TEMPLATE_HEIGHT; row++)
        {
            for (int col = 0; col < TEMPLATE_WIDTH; col++)
            {
                Point point = {component.row + row, component.column + col};
                owner[pixel_index(point, board_image)] = component.id;
            }
        }
    }
    return owner;
}

// Check if a point is in bounds, a white pixel, not inside a component box and unvisited
static int is_open_point(Point point, Bmp board_image, int *owner, uint8_t *visited)
{
    if (!is_in_bounds(point, board_image))
        return 0;

    int index = pixel_index(point, board_image);
    return (board_image.pixels[point.row][point.col][RED] == 255
        && owner[index] == NO_OWNER && !visited[index]);
}

// Marks a point as visited and adds it to the back of the queue
static void enqueue_point(Point point, Bmp board_image, uint8_t *visited, Point *queue, int *tail)
{
    visited[pixel_index(point, board_image)] = 1;
    queue[*tail] = point;
    (*tail)++;
}

// Starts the queue with every open pixel directly touching the outside of the component's box
static void seed_queue_from_component(Component origin, Bmp board_image, int *owner, 
    uint8_t *visited, Point *queue, int *tail)
{
    for (int row = 0; row < TEMPLATE_HEIGHT; row++)
    {
        for (int col = 0; col < TEMPLATE_WIDTH; col++)
        {
            for (int i = 0; i < NEIGHBOUR_COUNT; i++)
            {
                Point neighbour = {origin.row + row + dir_row[i], origin.column + col + dir_col[i]};
                if (is_open_point(neighbour, board_image, owner, visited))
                    enqueue_point(neighbour, board_image, visited, queue, tail);
            }
        }
    }
}

// Flags every other component whose box touches the given trace pixel
static void mark_touching_components(Point current, Component origin, Bmp board_image, 
    int *owner, uint8_t *touched)
{
    for (int i = 0; i < NEIGHBOUR_COUNT; i++)
    {
        Point neighbour = {current.row + dir_row[i], current.col + dir_col[i]};
        if (!is_in_bounds(neighbour, board_image))
            continue;

        int neighbour_owner = owner[pixel_index(neighbour, board_image)];
        if (neighbour_owner != NO_OWNER && neighbour_owner != origin.id)
            touched[neighbour_owner] = 1;
    }
}

// BFS over the traces from one component, flagging every component that is reached
static void flood_fill_from_component(Component origin, Bmp board_image, int *owner, uint8_t *touched)
{
    size_t pixel_count = (size_t) board_image.width * board_image.height;
    uint8_t *visited = (uint8_t *) calloc(pixel_count, sizeof(uint8_t));
    Point *queue = (Point *) malloc(sizeof(Point) * pixel_count);

    if (visited == NULL || queue == NULL)
    {
        free(visited);
        free(queue);
        return;
    }

    int head = 0, tail = 0;
    seed_queue_from_component(origin, board_image, owner, visited, queue, &tail);

    while (head < tail)
    {
        Point current = queue[head];
        head++;

        mark_touching_components(current, origin, board_image, owner, touched);

        // Search all 4 neighboring pixels
        for (int i = 0; i < NEIGHBOUR_COUNT; i++)
        {
            Point neighbour = {current.row + dir_row[i], current.col + dir_col[i]};
            if (is_open_point(neighbour, board_image, owner, visited))
                enqueue_point(neighbour, board_image, visited, queue, &tail);
        }
    }

    free(visited);
    free(queue);
}

// Find all the connections between all the components
void find_all_connections(Component_Node **component_node_head, Bmp board_image)
{
    int component_count = count_components(*component_node_head);
    int *owner = create_owner_map(*component_node_head, board_image);
    if (owner == NULL)
        return;

    // For each component find all their connections
    for (Component_Node *origin_pointer = *component_node_head; origin_pointer != NULL; 
        origin_pointer = origin_pointer->next)
    {
        find_connections(&(origin_pointer->component), component_count, board_image, owner);
    }
    free(owner);
}

// Find all the connections of one component, stored in ascending id order
void find_connections(Component *origin, int component_count, Bmp board_image, int *owner)
{
    uint8_t *touched = (uint8_t *) calloc(component_count, sizeof(uint8_t));
    if (touched == NULL)
        return;

    flood_fill_from_component(*origin, board_image, owner, touched);

    for (int id = 0; id < component_count; id++)
    {
        if (touched[id])
            add_connection(origin->connected, &(origin->count), id);
    }
    free(touched);
}

void print_component_connections(Component_Node *component_node_head)
{
    for (; component_node_head != NULL; component_node_head = component_node_head->next)
    {
        printf("Component %d connected to ", component_node_head->component.id);

        if (component_node_head->component.count == 0)
        {
            printf("nothing\n");
            continue;
        }
        for (int j = 0; j < component_node_head->component.count; j++)
        {
            printf("%d ", component_node_head->component.connected[j]);
        }
        printf("\n");
    }
}

// Adds a component id to the end of a component's connection list
void add_connection(int *connected, uint8_t *count, uint8_t component_id)
{
    if (*count >= MAX_COMPONENT)
        return;

    connected[*count] = component_id;
    (*count)++;
}
