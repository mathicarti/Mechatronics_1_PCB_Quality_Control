#ifndef TEMPLATE_H
#define TEMPLATE_H

#include <stdio.h>
#include "types.h"

Template_array read_template_array(FILE *template_fp);
void get_template_at_index(Template template, int template_index, FILE *template_fp);
void free_template_array(Template_array *template_array);
void print_template(Template template);

#endif