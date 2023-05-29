//
// Created by sid on 5/24/23.
//

#ifndef PYHP_SETTINGS_H
#define PYHP_SETTINGS_H

#include "conf.h"

#define PYHP_VERSION "1.0.2"

#define MAX_LINE_LENGTH 1024

#define CONF_FILE "/etc/pyhp/conf.json"

#define CONF_DEBUG get_config_bool("debug", true)

#define CONF_SERVER_USER get_config_str("server_user", "www-data")

#define CONF_SERVER_GROUP get_config_str("server_group", "www-data")

#define CONF_INSTALL_PATH get_config_str("install_path", "/usr/bin")

#define CONF_FCGI_SOCKET_PATH get_config_str("fcgi_socket_path", "/var/run/pyhp/pyhp-cgi.sock")

#endif //PYHP_SETTINGS_H
