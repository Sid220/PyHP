//
// Created by sid on 5/27/23.
//
#include "../include/conf.h"
#include "../include/settings.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>

void return_error(char message[], int status) {
    printf("Status: %d\n"
           "Content-type: text/plain\r\n\r\n"
           "%s", status, message);
}

void do_chown(const char *file_path,
              const char *user_name,
              const char *group_name) {
    uid_t uid;
    gid_t gid;
    struct passwd *pwd;
    struct group *grp;

    pwd = getpwnam(user_name);
    if (pwd == NULL) {
        fprintf(stderr, "Failed to get uid");
        exit(3);
    }
    uid = pwd->pw_uid;

    grp = getgrnam(group_name);
    if (grp == NULL) {
        fprintf(stderr, "Failed to get gid");
        exit(3);
    }
    gid = grp->gr_gid;

    if (chown(file_path, uid, gid) == -1) {
        fprintf(stderr, "chown fail; Is your server user (%s) and group (%s) correct?", CONF_SERVER_USER, CONF_SERVER_GROUP);
        exit(4);
    }
}

int main() {
    if (getuid() != 0) {
        fprintf(stderr, "pyhp-cgi-start must be run as root\n");
        exit(1);
    }

    initialise_config();

    printf("Starting the PyHP FastCGI Socket...\n");

    remove(CONF_FCGI_SOCKET_PATH);

    char base_install_command[] = "cgi-fcgi -start -connect %s %s/pyhp-cgi";
    // -2 for "%s"
    // -2 for "%s"
    // +1 for "\0"
    // = -3
    char *install_command = malloc(strlen(CONF_INSTALL_PATH) + strlen(CONF_FCGI_SOCKET_PATH) + strlen(base_install_command) - 3);
    sprintf(install_command, base_install_command, CONF_FCGI_SOCKET_PATH, CONF_INSTALL_PATH);
    system(install_command);
    free(install_command);

    do_chown(CONF_FCGI_SOCKET_PATH, CONF_SERVER_USER, CONF_SERVER_GROUP);
    chmod(CONF_FCGI_SOCKET_PATH, 0660);

    system("lsof -Ua | grep \"pyhp-cgi\" | awk -F '  ' '{print $2}' | head -n 1 | sudo tee /var/run/pyhp/pyhp-cgi.pid");

    return 0;
}