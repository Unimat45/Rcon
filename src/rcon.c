#include "rcon.h"

#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_LEN 4110

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
	Message *m = create_message(password, Auth);
	if (m == NULL) {
		return false;
	}

	uint8_t *encoded = encode_message(m);

	if (encoded == NULL) {
		free_message(m);
		return false;
	}

	if (send(client_fd, encoded, m->length + 4, 0) < 0) {
		free_message(m);
		free(encoded);
		return false;
	}

	uint8_t buf[MAX_LEN];
	if (read(client_fd, buf, MAX_LEN) < 0) {
		free_message(m);
		free(encoded);
		return false;
	}

	Message *r = decode_message(buf);

	if (r == NULL) {
		free_message(m);
		free(encoded);
		return false;
	}

	bool isOk = m->id == r->id;

	free(encoded);
	free_message(m);
	free_message(r);

	return isOk;
}

Message *send_command(char *command) {
	Message *m = create_message(command, Command);
	if (m == NULL) {
		return NULL;
	}

	uint8_t *encoded = encode_message(m);

	if (encoded == NULL) {
		free_message(m);
		return NULL;
	}

	if (send(client_fd, encoded, m->length + 4, 0) < 0) {
		free_message(m);
		free(encoded);
		return NULL;
	}

	uint8_t buf[MAX_LEN];
	if (read(client_fd, buf, MAX_LEN) < 0) {
		free_message(m);
		free(encoded);
		return NULL;
	}

	Message *r = decode_message(buf);

	if (r == NULL) {
		free_message(m);
		free(encoded);
		return NULL;
	}

	free_message(m);
	free(encoded);

	return r;
}