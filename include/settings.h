//
// Created by sid on 5/24/23.
//

#ifndef PYHP_SETTINGS_H
#define PYHP_SETTINGS_H

#include "conf.h"

#define PYHP_VERSION "1.0.0"

#define MAX_LINE_LENGTH 1024

#define CONF_FILE "/etc/pyhp/conf.json"

#define CONF_DEBUG get_config_bool("debug", true)

#endif //PYHP_SETTINGS_H
