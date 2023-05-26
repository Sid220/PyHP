#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "../include/runner.h"
#include "../include/pyhp.h"
#include "../include/settings.h"
#include "../include/log.h"
#include <stdbool.h>

#include "../include/io.h"
#include "../include/conf.h"

const char *get_c_version() {
#ifdef __STDC_VERSION__
    long version = __STDC_VERSION__;
    if (version == 199901) {
        return "C99";
    }
    if (version == 201112) {
        return "C11";
    }
    if (version == 201710) {
        return "C18";
    } else {
        return "Unknown";
    }
#else
    return "C90";
#endif
};

bool return_desired = false;

void denitialise() {
    denitialise_py();
}

void initialise() {
    initialise_py();

    log_set_quiet(true);

    FILE *main_log_file;
    main_log_file = fopen(LOG_FILE, "a");
    FILE *err_log_file;
    err_log_file = fopen(LOG_FILE_ERR, "a");

    if (main_log_file == NULL) {
        return_error("Internal Server Error.\n", 500);
    } else {
        log_add_fp(main_log_file, LOG_TRACE);
        if (err_log_file == NULL) {
            log_warn("Unable to open ERROR file; All logs will still be written here.");

        } else {
            log_add_fp(err_log_file, LOG_ERROR);
        }
        log_trace("Started logging to log file");
    }

    initialise_config();
}

void return_error(char message[], int status) {
    printf("Status: %d\n"
           "Content-type: text/plain\r\n\r\n"
           "%s", status, message);
    return_desired = true;
}

/// NOTE: DOES NOT CLOSE FILE! YOU THE USER ARE 100% RESPONSIBLE FOR CLOSING THE FILE
void parse_file(FILE *file) {
    char character;
    //TODO: Change content size dynamically
    int current_content_size = 10000;
    char content[10000];
    int content_index = 0;
    bool in_tag = false;
    bool has_read = false;

    printf("Content-type: text/html\r\n\r\n");

    // Read the file character by character
    while ((character = fgetc(file)) != EOF) {
        if (!in_tag && character == '<') {
            // Start of potential tag
            if ((character = fgetc(file)) == '?') {
                // Tag has started
                in_tag = true;
                content_index = 0;
            } else {
                // Not a tag, print the character
                if (!return_desired) {
                    putchar('<');
                    has_read = true;
                }
                ungetc(character, file);
            }
        } else if (in_tag && character == '?') {
            // End of potential tag
            if ((character = fgetc(file)) == '>') {
                // Tag has ended, print the content
                content[content_index] = '\0';
                char *cleaned = remove_excess_indentation(content);
                if (!return_desired) {
                    run_py_code(content);
                    has_read = true;
                }
//                free(cleaned);

                in_tag = false;
            } else {
                // Not a tag, add characters to content
                content[content_index++] = '?';
                content[content_index++] = character;
            }
        } else if (in_tag) {
            // Inside the tag, add characters to content
            content[content_index++] = character;
            // Resize content buffer if necessary
            if (content_index >= 10000 - 1) {
                return_error("Internal Server Error.\n", 500);
                log_error("The python content in a file extends past the maximum!");
            }
        } else {
            // Not in a tag, print the character
            if (!return_desired) {
                putchar(character);
                has_read = true;
            }
        }
    }

    if (has_read) {
        return_desired = true;
    }
}

void read_file(const char *filename) {
    struct stat st;

    if (stat(filename, &st) == 0) {
        if ((st.st_mode & S_IFMT) != S_IFDIR) {
            FILE *file = fopen(filename, "r");
            if (file) {
                parse_file(file);
                fclose(file);
                log_trace("Loaded file %s", filename);
            } else {
                return_error("Failed to open file", 500);
                log_error("Failed to open file which exists: %s (permissions?)", filename);
            }
        } else {
            return_error("File not found", 404);
            log_warn("Directory given %s", filename);
        }
    } else {
        return_error("File not found", 404);
        log_info("File not found: %s", filename);
    }
}
