#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include "bitmap.h"
#include "connections.h"
#include "matching.h"
#include "template.h"

typedef struct
{
    uint8_t x;
    uint8_t y;
} Point;

// Directions for moving Up, Down, Left, Right
int dir_x[] = {-1, 1, 0, 0};
int dir_y[] = {0, 0, -1, 1};

void add_connection(int *connected, uint8_t *count, uint8_t component_id)
{
    (*count)++;
    connected[*count] = component_id;
}

// Check if new point is in bounds, a white pixel and unvisited
int is_valid_point(Point new_point, Bmp board_image, uint8_t **visited)
{
    return (new_point.x >= 0 && new_point.x < board_image.width && new_point.y >= 0 && new_point.y < board_image.height 
        && board_image.pixels[0] != 0 && !visited[new_point.x][new_point.y]);
}

// BFS to find if there is a valid path to a target point
int BFS_component(Bmp board_image, uint8_t **visited, Point *queue, int head, int tail, Point target)
{
    // Empty queue (not found)
    if (head == tail)
        return 0;
    
    // Dequeue front
    Point current = queue[head];
    
    // Found target point
    if (current.x == target.x && current.x == target.y)
        return 1;

    // Search all 4 neighboring pixels
    for (int i = 0; i < 4; i++)
    {
        Point new_point = {current.x + dir_x[i], current.y + dir_y[i]};

        if (is_valid_point(new_point, board_image, visited)) 
        {
            visited[new_point.x][new_point.y] = 1;
            queue[tail].x = new_point.x;
            queue[tail].y = new_point.y;
            tail++;
        }
    }
    // Move head forward by one and search
    return BFS_component(board_image, visited, queue, head + 1, tail, target);
}

// Find all the connections of one component
void find_connections(Component_Node *origin_pointer, Component_Node *target_pointer, Bmp board_image)
{
    for (; target_pointer != NULL && target_pointer != origin_pointer; target_pointer = target_pointer->next)
    {
        // 2D grid of visited pixels; 1 is visited 0 is not
        // Set up as a VLA for easy freeing
        uint8_t (*visited)[board_image.width] = calloc(board_image.height * sizeof(*visited), 0);
        Point queue[board_image.height * board_image.width];
        int head = 0, tail = 0;
    
        Point origin = {origin_pointer->component.row, origin_pointer->component.column};
        Point target = {target_pointer->component.row, target_pointer->component.column};
    
        visited[origin.x][origin.x] = 1;
        queue[tail] = origin;
        tail++;

        // Do BFS until target coordinate are found. If found add add id do origin list. Move on to next target component
        if (BFS_component(board_image, visited, queue, head, tail, target))
            add_connection(origin_pointer->component.connected, &(origin_pointer->component.count), target_pointer->component.id);
        
        free(visited);
    }
}

// Find all the connections between all the components
void find_all_connections(Component_Node **component_node_head, Bmp board_image)
{
    Component_Node *origin_pointer = *component_node_head;
    Component_Node *target_pointer = *component_node_head;
    
    // For each component find all their connections
    for (; origin_pointer != NULL; origin_pointer = origin_pointer->next)
    {
        find_connections(origin_pointer, target_pointer, board_image);
    }
}

void print_component_connections(Component_Node *component_node_head)
{
    for (; component_node_head != NULL; component_node_head = component_node_head->next)
    {
        printf("Component %d connected to", component_node_head->component.id);
        for (int j = 0; j < component_node_head->component.count; j++)
        {
            printf(" %d", component_node_head->component.connected[j]);
        }
        printf("\n");
    }
}

void handle_c_mode(Input_request request, FILE *template_fp)
{
    // load bmp and threshold it
    Bmp board_image = read_bmp(request.board_image_filename);
    threshold_board_image(&board_image);
    
    // Get the list of components and positions on the board
    Component_Node *component_node_head = get_component_list(template_fp, board_image);

    // print component type and positions
    print_components(component_node_head);

    find_all_connections(&component_node_head, board_image);
    
    print_component_connections(component_node_head);
    
    // free variables
    free_bmp(board_image);
    free_component_node(component_node_head);
    return;
}