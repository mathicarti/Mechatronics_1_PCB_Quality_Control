#ifndef MODES_H
#define MODES_H

#include <stdio.h>
#include "types.h"

void handle_t_mode(Input_request request, FILE *template_fp);
void handle_l_mode(Input_request request, FILE *template_fp);
void handle_c_mode(Input_request request, FILE *template_fp);

#endif