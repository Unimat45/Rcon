#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
	Response = 0,
	Command = 2,
	Auth
} MessageType;

typedef struct {
	int length;
	int type;
	char payload[4110];
	MessageType id;
} Message;

void encode_message(uint8_t *buf, Message *m);
bool decode_message(Message *m, uint8_t *data);
bool create_message(Message *m, char *payload, MessageType type);

#endif // MESSAGE_H