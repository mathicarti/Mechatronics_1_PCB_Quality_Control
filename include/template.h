#ifndef TEMPLATE_H
#define TEMPLATE_H

#include <stdio.h>
#include "types.h"

// Loads every template in the library file
// Caller must free the result with free_template_array
Template_array read_template_array(FILE *template_fp);

// Decodes the template at template_index into an already allocated 32x32 template
void get_template_at_index(Template template, int template_index, FILE *template_fp);

// Frees all memory owned by the array (not the struct itself)
void free_template_array(Template_array *template_array);

// Prints "Template data:" then the template, top row first, as '1' and ' '
void print_template(Template template);

#endif