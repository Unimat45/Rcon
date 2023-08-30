#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdint.h>

typedef enum {
	Response = 0,
	Command = 2,
	Auth
} MessageType;

typedef struct {
	int length;
	int type;
	char *payload;
	MessageType id;
} Message;

void free_message(Message *m);
uint8_t *encode_message(Message *m);
Message *decode_message(uint8_t *data);
Message *create_message(char *payload, MessageType type);

#endif // MESSAGE_H