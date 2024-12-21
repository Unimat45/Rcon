#ifndef MESSAGE_H
#define MESSAGE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


/*
 * Maximum payload sizes for client-server and server-client packets
 */
#define CS_MAX_PAYLOAD 1446
#define SC_MAX_PAYLOAD 4096

    typedef enum
    {
        Strip = 0,
        Keep
    } ColorBehavior;

    typedef enum
    {
        Response = 0,
        Command = 2,
        Auth = 3
    } MessageType;

    typedef struct
    {
        uint32_t length;
        uint32_t id;
        MessageType type;
        char *payload;
    } Message;

    Message *create_message(MessageType type, const char *payload, size_t size);
    size_t encode_message(Message *m, uint8_t *buf);
    Message *decode_message(uint8_t *data, ColorBehavior behavior);
    void append_message(Message *m, uint8_t *data, ColorBehavior behavior);
    void free_message(Message *m);

#ifdef __cplusplus
}
#endif

#endif // MESSAGE_H