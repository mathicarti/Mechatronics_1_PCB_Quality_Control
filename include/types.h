#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

#define TEMPLATE_HEIGHT 32
#define TEMPLATE_WIDTH 32
#define TEMPLATE_WIDTH_BYTE 4
#define BYTE 8
#define THRESHOLD 127.0
#define MAX_COMPONENT 128
#define FILENAME_BUFFER 512

typedef uint8_t **Template;

typedef struct
{
    // The number of templates in array
    uint8_t count;

    // Array with templates that have rows and columns with a byte for a single pixel value (1 white, 0 black)
    Template *template;
} Template_array;

typedef enum
{
    UNDEFINED,
    TEMPLATE,
    COMPONENT_LIST,
    CHECK_CONNECTIONS
} Mode_type;

typedef struct
{
    Mode_type mode;
    char template_filename[FILENAME_BUFFER];
    int index;
    char board_image_filename[FILENAME_BUFFER];
} Input_request;

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

#endif