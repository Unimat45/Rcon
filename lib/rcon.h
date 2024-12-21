#ifndef RCON_H
#define RCON_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "message.h"

    void free_client(void);
    Message *send_command(const char *command, ColorBehavior behavior);
    bool rcon_authenticate(const char *password);
    bool connect_to_server(const char *address, uint16_t port);

#ifdef __cplusplus
}
#endif

#endif // RCON_H
