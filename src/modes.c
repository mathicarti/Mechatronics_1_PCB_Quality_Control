#include <stdio.h>

#include "types.h"
#include "modes.h"
#include "template.h"
#include "matching.h"
#include "board.h"
#include "connection.h"

void handle_t_mode(Input_request request, FILE *template_fp)
{
    // Load the binary file as a bmp style template array
    Template_array template_array = read_template_array(template_fp);

    // Check if index in range
    if (request.index < 0 || request.index >= template_array.count)
    {
        printf("Template index out of range\n");
        return;
    }
    // Print template at index
    print_template(template_array.template[request.index]);

    free_template_array(&template_array);
}

void handle_l_mode(Input_request request, FILE *template_fp)
{
    // load bmp and threshold it
    Bmp board_image = read_bmp(request.board_image_filename);
    threshold_board_image(&board_image);
    
    // Search through board for any matching templates and add it to list
    // Get the list of components and positions on the board
    Component_Node *component_node_head = get_component_list(template_fp, board_image);

    // print component type and positions
    print_components(component_node_head);

    // free variables
    free_bmp(board_image);
    free_component_node(component_node_head);
    return;
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