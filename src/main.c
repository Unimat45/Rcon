#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "rcon.h"

#ifdef _MSC_VER

#define CASE_CMP(a, b) (_strcmpi(a, b) == 0)
#pragma execution_character_set( "utf-8" )

#else

#include <strings.h>
#define CASE_CMP(a, b) (strcasecmp(a, b) == 0)

#endif

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
    char *address = "127.0.0.1";
    char *password = "minecraft";

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

#ifdef _MSC_VER
        (void)scanf_s("%s", buf, 4110);
#else
        (void)scanf("%s", buf);
#endif

        if (CASE_CMP(buf, "Q")) {
            break;
        }

        Message *res = send_command(buf);

        (void)printf("%s\n", res->payload);
    }

    free_client();

    return 0;
}