#include "message.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define HEADER_SIZE 10

void removeBytes(char* payload, char byte_to_remove, int bytes_to_remove);

int lastID = 1;

bool create_message(Message *m, char *payload, MessageType type) {
	if (m == NULL) {
		return false;
	}

	size_t len = strlen(payload);

  memcpy(m->payload, payload, len + 1);
	m->payload[len] = 0;

	m->length = (int)len + HEADER_SIZE;
	m->id = lastID++;
	m->type = type;

	return true;
}

void encode_message(uint8_t *buf, Message *m) {
	if (buf == NULL) {
		return;
	}

	memcpy(buf, &m->length, sizeof(int));
	memcpy(buf + 4, &m->id, sizeof(int));
	memcpy(buf + 8, &m->type, sizeof(int));

	size_t len = strlen(m->payload);

  memcpy((char*)(buf + 12), m->payload, len + 1);
	buf[12 + len] = 0;
	buf[13 + len] = 0;
}

bool decode_message(Message *m, uint8_t *data) {
	if (m == NULL) {
		return false;
	}

	memcpy(&m->length, data, sizeof(int));
	memcpy(&m->id, data + 4, sizeof(int));
	memcpy(&m->type, data + 8, sizeof(int));

  memcpy(m->payload, (char*)(data + 12), m->length - HEADER_SIZE);
	m->payload[m->length - HEADER_SIZE] = 0;

	removeBytes(m->payload, -62, 3);
	removeBytes(m->payload, -62, 3);

	return true;
}

void removeBytes(char* payload, char byte_to_remove, int bytes_to_remove) {
	size_t len = strlen(payload);
	size_t trunc = 0;

	for (size_t i = 0; i < len; i++) {
		if (payload[i] == byte_to_remove) {
			(void)memmove(payload + i, payload + i + bytes_to_remove, len - i - bytes_to_remove);
			i += bytes_to_remove - 1;
			trunc += bytes_to_remove;
		}
	}

	payload[len - trunc] = 0;
}
