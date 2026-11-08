#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

// Templates are fixed 32x32 pixel bitmaps, stored 1 bit per pixel in the binary file
#define TEMPLATE_HEIGHT 32
#define TEMPLATE_WIDTH 32
#define TEMPLATE_WIDTH_BYTE 4
#define BYTE 8

// A pixel is white (1) only if its average RGB is strictly greater than this
#define THRESHOLD 127.0

// Upper bound on connections per component, sized for the connected[] array
#define MAX_COMPONENT 128
#define FILENAME_BUFFER 512

// Indexed as template[row][col]; row 0 is the bottom, matching the BMP layout
// Each element is 0 (black) or 1 (white), one byte per pixel
typedef uint8_t **Template;

// All templates loaded from the library file
typedef struct
{
    // The number of templates in array
    uint8_t count;

    // Array with templates that have rows and columns with a byte for a single pixel value (1 white, 0 black)
    Template *template;
} Template_array;

// Which of the program's modes the command line asked for
typedef enum
{
    UNDEFINED,          // Set when the mode argument is invalid
    TEMPLATE,           // 't': print one template
    COMPONENT_LIST,     // 'l': list components found on the board
    CHECK_CONNECTIONS   // 'c': list components and their connections
} Mode_type;

// Parsed command line arguments
typedef struct
{
    Mode_type mode;
    char template_filename[FILENAME_BUFFER];
    int index;                                  // Only used in TEMPLATE mode
    char board_image_filename[FILENAME_BUFFER]; // Only used in 'l' and 'c' modes
} Input_request;

// Struct that keeps track of the components id, type, position and its connections
typedef struct
{
    uint8_t id;             // Order found on the board; also the number printed in connection output
    uint8_t type;           // Index of the matching template in the library
    uint16_t row;           // Bottom-left pixel of the component. 16 bits as columns can exceed 255
    uint16_t column;
    int connected[MAX_COMPONENT]; // Ids of connected components, ascending; only first count entries are valid
    uint8_t count;          // Number of valid entries in connected[]
} Component;

// Linked list to track each component on board
typedef struct Component_Node
{
    Component component;
    struct Component_Node *next;
} Component_Node;

#endif