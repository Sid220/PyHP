//
// Created by sid on 5/23/23.
//

#ifndef PYHP_RUNNER_H
#define PYHP_RUNNER_H

void import_defaults(void);

char *remove_excess_indentation(char *code);

void run_py_code(char *code);

void initialise_py(void);

void denitialise_py(void);

const char *get_py_version(void);

#endif //PYHP_RUNNER_H