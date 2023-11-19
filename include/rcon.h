#ifndef RCON_H
#define RCON_H

#include "message.h"

void free_client(void);
bool send_command(Message *r, char *command);
bool rcon_authenticate(char *password);
bool connect_to_server(char *address, uint16_t port);

#endif // RCON_H