#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "argparse.h"
#include "cio.h"
#include "rcon.h"

#ifdef _MSC_VER
#define CASE_CMP(a, b) (_strcmpi(a, b) == 0)
#else
#include <strings.h>
#define CASE_CMP(a, b) (strcasecmp(a, b) == 0)
#endif

static const char *const usages[] = {
    "rcon [options]",
    NULL,
};

int main(int argc, const char **argv)
{
    uint16_t port = 25575;
    char *address = "127.0.0.1";
    char *password = "minecraft";

    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_STRING('H', "host", &address, "Host address of the server", NULL, 0,
                   0),
        OPT_STRING('p', "password", &password, "Password of the server", NULL,
                   0, 0),
        OPT_INTEGER('P', "port", &port, "Host port of the server", NULL, 0, 0),
        OPT_END(),
    };

    struct argparse argparse;
    argparse_init(&argparse, options, usages, 0);
    argparse_describe(
        &argparse, "Interacts with a minecraft server using the RCON protocol",
        "");
    argc = argparse_parse(&argparse, argc, argv);

    if (!connect_to_server(address, port))
    {
        fprintf(stderr, "Failed to connect to server\n");
        return 1;
    }

    if (!rcon_authenticate(password))
    {
        fprintf(stderr, "Failed to authenticate\n");
        return 1;
    }

    char buf[4110];
    write_string("Connected! Use 'Q' or 'Ctrl+C' to exit.\n");

    while (true)
    {
        write_string("> ");

        read_string(buf);

        if (CASE_CMP(buf, "Q") || CASE_CMP(buf, "QUIT"))
        {
            break;
        }

        Message *res = send_command(buf, Strip);

        if (res == NULL)
        {
            write_string("Error sending message\n");
        }
        else
        {
            write_string(res->payload);
            write_string("\n");
        }
    }

    free_client();

    return 0;
}
