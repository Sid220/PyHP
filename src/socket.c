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
//        read_file("/var/www/html/info.pyhp");
        done();

        return_desired = false;

        denitialise();
    }
}
