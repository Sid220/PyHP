//
// Created by sid on 5/24/23.
//

#ifndef PYHP_PYHP_H
#define PYHP_PYHP_H

#include <stdbool.h>

#include "io.h"

const char *get_c_version();

extern bool return_desired;

void add_header(char *head);

void add_content(char *con);

void done();

void initialise(void);

void denitialise(void);

void return_error(char message[], int status);

void parse_file(FILE *file);

void read_file(const char *filename);

#endif //PYHP_PYHP_H