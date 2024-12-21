#include "rcon.h"

#include <stdlib.h>
#include <string.h>

#ifdef _WIN32

#include <stdio.h>

#define WIN32_LEAN_AND_MEAN
#include <WS2tcpip.h>
#include <WinSock2.h>

#pragma comment(lib, "Ws2_32.lib")

static SOCKET sock = INVALID_SOCKET;

bool connect_to_server(const char *address, uint16_t port)
{
    struct addrinfo *result = NULL, *ptr = NULL, hints;
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        return false;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    char p[6];
    sprintf_s(p, sizeof(p), "%u", port);

    if (getaddrinfo(address, p, &hints, &result) != 0)
    {
        (void)WSACleanup();
        return false;
    }

    for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
    {
        sock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

        if (sock == INVALID_SOCKET)
        {
            (void)WSACleanup();
            return false;
        }

        if (connect(sock, ptr->ai_addr, (int)ptr->ai_addrlen) == SOCKET_ERROR)
        {
            (void)closesocket(sock);
            sock = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (sock == SOCKET_ERROR)
    {
        (void)closesocket(sock);
        (void)WSACleanup();
        return false;
    }

    return true;
}

void free_client()
{
    (void)closesocket(sock);
    (void)WSACleanup();
}

#else // Unix

#include <arpa/inet.h>
#include <unistd.h>

static int sock;

bool connect_to_server(const char *address, uint16_t port)
{
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        return false;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, address, &serv_addr.sin_addr) <= 0)
    {
        return false;
    }

    int status =
        connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (status < 0)
    {
        return false;
    }

    return true;
}

void free_client(void) { (void)close(sock); }

#endif

Message *send_command(const char *command, ColorBehavior behavior)
{
    Message *m, *r;
    m = create_message(Command, command, strlen(command));

    if (m == NULL)
    {
        return NULL;
    }

    uint8_t encoded[CS_MAX_PAYLOAD];
    encode_message(m, encoded);

    if (send(sock, (const char *)encoded, m->length + 4, 0) < 0)
    {
        goto stop;
    }

    uint8_t buf[SC_MAX_PAYLOAD];
    int bytes_read = recv(sock, (char *)buf, SC_MAX_PAYLOAD, 0);

    if (bytes_read <= 0)
    {
        goto stop;
    }

    r = decode_message(buf, behavior);

    if (r == NULL)
    {
        goto stop;
    }

    return r;

stop:
    free_message(m);
    return NULL;
}

bool rcon_authenticate(const char *password)
{
    Message *m = create_message(Auth, password, strlen(password));

    if (m == NULL)
    {
        return false;
    }

    uint8_t encoded[CS_MAX_PAYLOAD];
    encode_message(m, encoded);

    if (send(sock, (const char *)encoded, m->length + 4, 0) < 0)
    {
        return false;
    }

    uint8_t buf[SC_MAX_PAYLOAD];
    int bytes_read = recv(sock, (char *)buf, SC_MAX_PAYLOAD, 0);

    if (bytes_read <= 0)
    {
        return false;
    }

    Message *r;

    r = decode_message(buf, Keep);

    if (r == NULL)
    {
        return false;
    }

    bool ret = m->id == r->id;

    free_message(m);
    free_message(r);

    return ret;
}
