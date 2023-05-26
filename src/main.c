#include <stdio.h>
#include "../include/pyhp.h"
#include "../include/runner.h"
#include "../include/main.h"

int main() {
    initialise();
    read_file("/var/www/html/info.pyhp");
    done();
    denitialise();
}