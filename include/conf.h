//
// Created by sid on 5/26/23.
//

#ifndef PYHP_CONF_H
#define PYHP_CONF_H

#include "cJSON.h"
#include <stdbool.h>

extern cJSON *conf_json;

void initialise_config();

char *get_config_str(const char *name, char *default_val);

bool get_config_bool(const char *name, bool default_val);

bool get_config_bool_req(const char *name);

#endif //PYHP_CONF_H
