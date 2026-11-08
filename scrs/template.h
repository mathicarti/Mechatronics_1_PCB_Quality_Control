#ifndef _TEMPLATE_H
#define _TEMPLATE_H

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

void handle_t_mode(Input_request request, FILE *template_fp);
void print_template(Template template);
void print_pixel(uint8_t pixel);

Template_array read_template_array(FILE *template_fp);
void get_template_at_index(Template template, int template_index, FILE *template_fp);
void arrange_template(Template template, uint8_t *template_buffer);
void free_template_array(Template_array *template_array);

#endif