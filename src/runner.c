#include "../include/io.h"

#include <Python.h>
#include <string.h>
#include <stdbool.h>
#include "../include/settings.h"
#include "../include/pyhp.h"
#include "../include/main.h"
#include "../include/log.h"

#define MAX_WEIRD_FORMATTING 30


void import_defaults() {
    char *environmental_variables = malloc(2);  // Start with an empty string
    environmental_variables[0] = '[';
    environmental_variables[1] = '\0';

    // Loop through the environ array
    for (int i = 0; environ[i] != NULL; i++) {
        // Get the current environmental variable
        char *env = environ[i];

        // Format the HTML list item and append it to the string
        char *list_item = malloc(strlen(env) + 14);  // Allocate memory for the list item
        sprintf(list_item, "'%s',", env);
        char *environmental_variables_tmp = realloc(environmental_variables,
                                                    strlen(environmental_variables) + strlen(list_item) + 2);
        if (environmental_variables_tmp == NULL) {
            return_error("Internal Server Error.", 500);
            log_error("Couldn't make array of environmental variables: realloc fail!");
        }
        environmental_variables = environmental_variables_tmp;

        strcat(environmental_variables, list_item);

        // Free the memory allocated for the list item
        free(list_item);
    }

    strcat(environmental_variables, "]");

    char *imports = (char *) malloc((strlen(environmental_variables) + 2000) * sizeof(char));

    sprintf(imports,
            ""
            "from urllib.parse import urlparse, parse_qs\n"
            "import html\n"
            "__escape_html__ = html.escape\n"
            "__ENVIRONMENTAL_VARIABLES__ = %s\n"
            "def __get_env__(env_name):\n"
            "    for env in __ENVIRONMENTAL_VARIABLES__:\n"
            "        if env.startswith(env_name + '='):\n"
            "            return env[len(env_name) + 1:]\n"
            "    return None\n"
            "__pyhp_info__ = {"
            "'Version_PyHP': '%s',"
            "'Version_C': '%s',"
            "'Compilation_Date': '%s %s',"
            "'IS_FCGI': '%s'"
            "}\n"
            "__get__ = parse_qs('%s')\n"
            "__post__ = parse_qs('%s')\n",
            environmental_variables, PYHP_VERSION, get_c_version(), __DATE__, __TIME__, IS_FAST_CGI,
            getenv("QUERY_STRING"), getenv("REQUEST_BODY"));

    PyRun_SimpleString(
//            "def __info__():\n"
//            "    print('INFORMATION')\n"
//            "__get__ = ['getParams']\n"
//            "__post__ = ['postParams']\n"
//            "__files__ = ['fileParams']\n"
//            "__session__ = ['sessionParams']\n"
            imports
    );

    free(imports);
    free(environmental_variables);
}

/// NOTE: YOU THE USER ARE 100% RESPONSIBLE FOR FREEING THE MEMORY USED BY THIS FUNCTION! YOU HAVE BEEN WARNED
char *remove_excess_indentation(char *code) {
//    size_t codeLength = strlen(code);
//    size_t bufferSize = codeLength + 1;
//    char *tempCode = (char *) malloc(bufferSize);
//    if (tempCode == NULL) {
//        printf("Failed to allocate memory.\n");
//        return NULL;
//    }//    PyObject* fobj;
//    int fd = PyObject_AsFileDescriptor(fobj);
//    if (fd < 0) {
//        printf("err!");
//    }
//    PyObject* sys_module = PyFile_FromFd(fd, "/tmp/di.txt",
//                                         "w", -1, NULL, NULL, NULL, 1);
////    PyObject* stdout_obj = PySys_GetObject("stdout", sys_module);
////    printf("%s", stdout_obj->ob_refcnt);
////    PySys_SetObject("stdout", capture_obj);
//    PySys_SetObject("stdout", sys_module);
//    strcpy(tempCode, code);
//    char weirdIndentation[MAX_WEIRD_FORMATTING];
//    bool has_set_weird_indentation = false;
//
//    for (size_t i = 0; i < codeLength; i++) {
//        if (tempCode[i] == '\n') {
//            while (tempCode[i + 1] == '\n') {
//                for (size_t l = (i + 2), m = (i + 1); l <= codeLength; l++, m++) {
//                    tempCode[m] = tempCode[l];
//                }
//            }
//            size_t j = i + 1;
//
//            if ((tempCode[j] == ' ' || tempCode[j] == '\t') && !has_set_weird_indentation) {
//                while (j < codeLength && (tempCode[j] == ' ' || tempCode[j] == '\t')) {
//                    if (tempCode[j] == ' ') {
//                        strcat(weirdIndentation, "S");
//                    } else if (tempCode[j] == '\t') {
//                        strcat(weirdIndentation, "T");
//                    }
//                    j++;
//                }
//                has_set_weird_indentation = true;
//            }
//            if (has_set_weird_indentation) {
//                size_t indent_start = i + 1;
//                size_t num_shifts = strlen(weirdIndentation);
//                size_t shift_start = indent_start + num_shifts;
//                size_t shift_end = codeLength - num_shifts;
//
//                for (size_t l = shift_start, m = indent_start; l <= codeLength; l++, m++) {
//                    tempCode[m] = tempCode[l];
//                }
//                tempCode[shift_end] = '\0';
//            }
//        }
//    }

//    printf("WEIRD INDENT: %s\n", weirdIndentation);
//    printf("TMP: %s\n", tempCode);
    return code;
}


void run_py_code(char *code) {
    PyRun_SimpleString("__PYHP_STDOUT__ = ''");
    PyRun_SimpleString("__PYHP_STDERR__ = ''");

    PyRun_SimpleString(code);

    PyObject *main_module = PyImport_AddModule("__main__");
    PyObject *pyhp_stdout = PyObject_GetAttrString(main_module, "__PYHP_STDOUT__");
    const char *pyhp_stdout_value = PyUnicode_AsUTF8(pyhp_stdout);
    add_content(pyhp_stdout_value);

    PyObject *pyhp_stderr = PyObject_GetAttrString(main_module, "__PYHP_STDERR__");
    const char *pyhp_stderr_value = PyUnicode_AsUTF8(pyhp_stderr);

    if (*pyhp_stderr_value != '\0') {
        if (CONF_DEBUG) {
            char *err_container = malloc(strlen(pyhp_stderr_value) + 50);
            if(err_container == NULL) {
                log_error("Unable to log error");
                return_error("Internal Server Error.", 500);
            }
            sprintf(err_container, "<span style='color: red'>%s</span>", pyhp_stderr_value);
            add_content(err_container);
            free(err_container);
        } else {
            return_error("Internal Server Error.", 500);
        }
    }

    Py_DECREF(pyhp_stdout);
    Py_DECREF(pyhp_stderr);
}

void denitialise_py() {
    Py_Finalize();
}

void initialise_py() {
    Py_Initialize();
    import_defaults();
    PyRun_SimpleString("import sys\n"
                       "__PYHP_STDOUT__ = \"\"\n"
                       "def __PYHP_PRINT_STDOUT__(*args, **kwargs):\n"
                       "    global __PYHP_STDOUT__\n"
                       "    output = \" \".join(map(str, args))\n"
                       "    __PYHP_STDOUT__ += output\n"
                       "sys.stdout.write = __PYHP_PRINT_STDOUT__\n"
                       "__builtins__.print = __PYHP_PRINT_STDOUT__\n"
                       "__PYHP_STDERR__ = \"\"\n"
                       "def __PYHP_PRINT_STDERR__(*args, **kwargs):\n"
                       "    global __PYHP_STDERR__\n"
                       "    output = \" \".join(map(str, args))\n"
                       "    __PYHP_STDERR__ += output\n"
                       "sys.stderr.write = __PYHP_PRINT_STDERR__"
    );
}

const char *get_py_version() {
    return Py_GetVersion();
}