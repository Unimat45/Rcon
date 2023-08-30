#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdint.h>
#include "rcon.h"

typedef char * string;

int indexOf(char **haystack, int N, char* needle) {
    for (int i = 0; i < N; i++) {
        if (strcmp(haystack[i], needle) == 0) {
            return i;
        }
    }
    return -1;
}

int main(int argc, char **argv) {
    uint16_t port = 25575;
    string address = "127.0.0.1";
    string password = "minecraft";

    int hostIndex = indexOf(argv, argc, "-H");
    if (hostIndex != -1) {
        if (argc <= hostIndex + 1) {
            (void)fprintf(stderr, "No host specified\n");
            return 1;
        }
        address = argv[hostIndex + 1];
    }

    int passIndex = indexOf(argv, argc, "-p");
    if (passIndex != -1) {
        if (argc <= passIndex + 1) {
            (void)fprintf(stderr, "No password specified\n");
            return 1;
        }
        password = argv[passIndex + 1];
    }
    
    if (!connect_to_server(address, port)) {
        (void)fprintf(stderr, "Failed to connect to server\n");
        return 1;
    }

    if (!rcon_authenticate(password)) {
        (void)fprintf(stderr, "Failed to authenticate\n");
        return 1;
    }

    char buf[4110];
    (void)printf("Connected! Use 'Q' or 'Ctrl+C' to exit.\n");
    while (true) {
        (void)printf("> ");

        scanf("%s", buf);

        if (strcasecmp(buf, "Q") == 0) {
            break;
        }

        Message *res = send_command(buf);

        (void)printf("%s\n", res->payload);
    }

    return 0;
}