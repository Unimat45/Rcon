#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "rcon.h"
#include "argparse.h"

#ifdef _MSC_VER

#define CASE_CMP(a, b) (_strcmpi(a, b) == 0)
#pragma execution_character_set( "utf-8" )

#else

#include <strings.h>
#define CASE_CMP(a, b) (strcasecmp(a, b) == 0)

#endif

static const char *const usages[] = {
    "rcon [options]",
    NULL,
};

int main(int argc, char **argv) {
    uint16_t port = 25575;
    char *address = "10.0.0.106";
    char *password = "7y6t5r4e3w";

    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_STRING('H', "host", &address, "Host address of the server", NULL, 0, 0),
        OPT_STRING('P', "password", &password, "Password of the server", NULL, 0, 0),
        OPT_END(),
    };

    struct argparse argparse;
    argparse_init(&argparse, options, usages, 0);
    argparse_describe(&argparse, "Interacts with a minecraft server using the RCON protocol", "");
    argc = argparse_parse(&argparse, argc, argv);
    
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
        (void)scanf_s(" %4109[^\n]", buf, 4110);
#else
        (void)scanf(" %4109[^\n]", buf);
#endif

        if (CASE_CMP(buf, "Q") || CASE_CMP(buf, "QUIT")) {
            break;
        }

        Message res;
        bool err = send_command(&res, buf);

        if (!err) {
            (void)printf("Error sending message\n");
        }
        else {
            (void)printf("%s\n", res.payload);
        }
    }

    free_client();

    return 0;
}