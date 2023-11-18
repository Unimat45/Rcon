#include "rcon.h"

#include <stdlib.h>
#define MAX_LEN 4110

#if defined(_WIN32) || defined(_WIN64)
#define rev_str _strrev
#define MAX_PORT_STR_SIZE 6
#define WIN32_LEAN_AND_MEAN

#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

WSADATA wsaData;
SOCKET sock = INVALID_SOCKET;
struct addrinfo *result = NULL, *ptr = NULL, hints;


size_t int_to_char(uint16_t a, char *out) {
	if (out == NULL) {
		return 0;
	}

	uint8_t i = 0;

	while (a) {
		out[i++] = (a % 10) + '0';
		a /= 10;
	}

	out[i] = 0;

	(void)rev_str(out);

	return i;
}

bool send_command(Message *r, char *command) {
	Message m;
	bool err = create_message(&m, command, Command);

	if (!err) {
		return false;
	}

	uint8_t encoded[4110];
	encode_message(encoded, &m);

	if (encoded == NULL) {
		return false;
	}

	if (send(sock, (const char *)encoded, m.length + 4, 0) < 0) {
		return false;
	}

	uint8_t buf[MAX_LEN];
	int bytes_read = recv(sock, (char *)buf, MAX_LEN, 0);

	if (bytes_read <= 0) {
		return false;
	}

	err = decode_message(r, buf);

	if (!err) {
		return false;
	}

	return r;
}

bool rcon_authenticate(char *password) {
	Message m;
	bool err = create_message(&m, password, Auth);

	if (!err) {
		return false;
	}

	uint8_t encoded[4110];
	encode_message(encoded, &m);

	if (encoded == NULL) {
		return false;
	}

	if (send(sock, (const char *)encoded, m.length + 4, 0) < 0) {
		return false;
	}


	uint8_t buf[MAX_LEN];
	int bytes_read = recv(sock, (char *)buf, MAX_LEN, 0);

	if (bytes_read <= 0) {
		return false;
	}

	Message r;
	
	err = decode_message(&r, buf);

	if (!err) {
		return false;
	}

	return m.id == r.id;
}

bool connect_to_server(char *address, uint16_t port) {
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		return false;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	char p[MAX_PORT_STR_SIZE];
	(void)int_to_char(port, p);

	if (getaddrinfo(address, p, &hints, &result) != 0) {
		(void)WSACleanup();
		return false;
	}

	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
		sock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

		if (sock == INVALID_SOCKET) {
			(void)WSACleanup();
			return false;
		}

		if (connect(sock, ptr->ai_addr, (int)ptr->ai_addrlen) == SOCKET_ERROR) {
			(void)closesocket(sock);
			sock = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (sock == SOCKET_ERROR) {
		(void)closesocket(sock);
		(void)WSACleanup();
		return false;
	}

	return true;
}

void free_client() {
	(void)closesocket(sock);
	(void)WSACleanup();
}

#else // Unix

#include <arpa/inet.h>
#include <unistd.h>

int client_fd;
struct sockaddr_in serv_addr;

bool connect_to_server(char *address, uint16_t port) {
	if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		return false;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);

	if (inet_pton(AF_INET, address, &serv_addr.sin_addr) <= 0) {
		return false;
	}

	int status = connect(client_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	if (status < 0) {
		return false;
	}

	return true;
}

bool rcon_authenticate(char *password) {
	Message m;
	bool err = create_message(&m, password, Auth);

	if (!err) {
		return false;
	}

	uint8_t encoded[4110];
	encode_message(encoded, &m);

	if (send(client_fd, encoded, m.length + 4, 0) < 0) {
		return false;
	}

	uint8_t buf[MAX_LEN];
	if (read(client_fd, buf, MAX_LEN) < 0) {
		return false;
	}

	Message r;
	err = decode_message(&r, buf);

	if (!err) {
		return false;
	}

	return m.id == r.id;
}

bool send_command(Message *r, char *command) {
	Message m;
	bool err = create_message(&r, command, Command);

	if (!err) {
		return NULL;
	}

	uint8_t encoded[4110];
	encode_message(encoded, &m);

	if (encoded == NULL) {
		return false;
	}

	if (send(client_fd, encoded, m.length + 4, 0) < 0) {
		return false;
	}

	uint8_t buf[MAX_LEN];
	if (read(client_fd, buf, MAX_LEN) < 0) {
		return false;
	}

	Message r;
	err = decode_message(&r, buf);

	if (!err) {
		return false;
	}

	return true;
}

void free_client() {
	(void)close(client_fd);
}
#endif