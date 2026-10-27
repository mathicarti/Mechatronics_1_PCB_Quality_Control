#ifndef _CONNECTION_H
#define _CONNECTION_H

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include "bitmap.h"
#include "matching.h"
#include "template.h"

void handle_c_mode(Input_request request, FILE *template_fp);

#endif