#include <stdlib.h>
#include <fcgi_stdio.h>
#include "../include/pyhp.h"
#include "../include/log.h"
#include "../include/main.h"

int main() {
    while (FCGI_Accept() >= 0) {
        initialise();

        log_info("SCRIPT_FILENAME %s", getenv("SCRIPT_FILENAME"));
        read_file(getenv("SCRIPT_FILENAME"));


        // Fallback if file is empty
        if (!return_desired) {
            log_warn("Fell through with file %s", getenv("SCRIPT_FILENAME"));
            printf("Content-type: text/plain\r\n\r\n");
            return_desired = true;
        }
        return_desired = false;

        denitialise();
    }
}
