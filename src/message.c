#include "message.h"

#include <string.h>
#include <stdlib.h>

#define HEADER_SIZE 10

int lastID = 1;

void free_message(Message *m) {
	if (m) {
		if (m->payload) {
			free(m->payload);
		}
		free(m);
	}
}

Message *create_message(char *payload, MessageType type) {
	Message *m = malloc(sizeof(Message));
	if (m == NULL) {
		return m;
	}

	size_t len = strlen(payload);
	m->payload = malloc(len * sizeof(char));

	if (m->payload == NULL) {
		free(m);
		return NULL;
	}

	(void)strncpy(m->payload, payload, len);
	m->payload[len] = 0;

	m->length = len + HEADER_SIZE;
	m->id = lastID++;
	m->type = type;

	return m;
}

uint8_t *encode_message(Message *m) {
	uint8_t *buf = malloc(sizeof(int) * m->length + 4);

	memcpy(buf, &m->length, sizeof(int));
	memcpy(buf + 4, &m->id, sizeof(int));
	memcpy(buf + 8, &m->type, sizeof(int));

	size_t len = strlen(m->payload);
	(void)strncpy(((char *)buf) + 12, m->payload, len);

	buf[12 + len] = 0;
	buf[13 + len] = 0;

	return buf;
}

Message *decode_message(uint8_t *data) {
	Message *m = malloc(sizeof(Message));

	if (m == NULL) {
		return m;
	}

	memcpy(&m->length, data, sizeof(int));
	memcpy(&m->id, data + 4, sizeof(int));
	memcpy(&m->type, data + 8, sizeof(int));

	m->payload = malloc(sizeof(char) * (m->length - HEADER_SIZE));

	if (m->payload == NULL) {
		free(m);
		return NULL;
	}

	(void)strncpy(m->payload, ((char *)data) + 12, m->length - HEADER_SIZE);
	m->payload[m->length - HEADER_SIZE] = 0;

	return m;
}