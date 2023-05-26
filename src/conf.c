//
// Created by sid on 5/26/23.
//
#include "../include/conf.h"
#include "../include/cJSON.h"
#include "../include/settings.h"
#include <stdlib.h>
#include "../include/io.h"
#include "../include/log.h"
#include "../include/pyhp.h"

cJSON *conf_json;

void initialise_config() {
    char *buffer = 0;
    long length;
    FILE *f = fopen(CONF_FILE, "rb");
    if (f) {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);
        buffer = malloc(length);
        if (buffer) {
            fread(buffer, 1, length, f);
        }
        fclose(f);
    } else {
        log_error("Unable to load config file. Looked in %s", CONF_FILE);
        return_error("Internal Server Error.", 500);
        free(buffer);
        return;
    }
    if (buffer) {
        // TODO: Fix invalid read
        conf_json = cJSON_Parse(buffer);
        if (conf_json == NULL) {
            const char *error_ptr = cJSON_GetErrorPtr();
            if (error_ptr != NULL) {
                log_error("Error before: %s while parsing config file %s\n", error_ptr, CONF_FILE);
                return_error("Internal Server Error.", 500);
            }
        }
    } else {
        log_error("Unable to load config file. Looked in %s. Config file may exist.", CONF_FILE);
        return_error("Internal Server Error.", 500);
    }
    free(buffer);
}

bool get_config_bool_req(const char *name) {
    const cJSON *obj = NULL;
    obj = cJSON_GetObjectItemCaseSensitive(conf_json, name);
    if (cJSON_IsBool(obj)) {
        return obj->valueint;
    } else {
        log_fatal("Invalid config for value %s, which IS REQUIRED", name);
        exit(523);
    }
}

bool get_config_bool(const char *name, bool default_val) {
    const cJSON *obj = NULL;
    obj = cJSON_GetObjectItemCaseSensitive(conf_json, name);
    if (cJSON_IsBool(obj)) {
        return obj->valueint;
    } else {
        return default_val;
    }
}

char *get_config_str(const char *name, char *default_val) {
    const cJSON *obj = NULL;
    obj = cJSON_GetObjectItemCaseSensitive(conf_json, name);
    if (cJSON_IsString(obj) && (obj->valuestring != NULL)) {
        return obj->valuestring;
    } else {
        return default_val;
    }
}
