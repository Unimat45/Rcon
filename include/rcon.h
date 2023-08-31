#ifndef RCON_H
#define RCON_H

#include <stdbool.h>
#include "message.h"

void free_client(void);
Message *send_command(char *command);
bool rcon_authenticate(char *password);
bool connect_to_server(char *address, uint16_t port);

#endif // RCON_H