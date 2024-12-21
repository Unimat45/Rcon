#include "message.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HEADER_SIZE 10

static uint32_t lastID = 1;
static const char *const colors[] = {
    "\033[0;30m",   /* 00 BLACK     0x30 */
    "\033[0;34m",   /* 01 BLUE      0x31 */
    "\033[0;32m",   /* 02 GREEN     0x32 */
    "\033[0;36m",   /* 03 CYAN      0x33 */
    "\033[0;31m",   /* 04 RED       0x34 */
    "\033[0;35m",   /* 05 PURPLE    0x35 */
    "\033[0;33m",   /* 06 GOLD      0x36 */
    "\033[0;37m",   /* 07 GREY      0x37 */
    "\033[0;1;30m", /* 08 DGREY     0x38 */
    "\033[0;1;34m", /* 09 LBLUE     0x39 */
    "\033[0;1;32m", /* 10 LGREEN    0x61 */
    "\033[0;1;36m", /* 11 LCYAN     0x62 */
    "\033[0;1;31m", /* 12 LRED      0x63 */
    "\033[0;1;35m", /* 13 LPURPLE   0x64 */
    "\033[0;1;33m", /* 14 YELLOW    0x65 */
    "\033[0;1;37m", /* 15 WHITE     0x66 */
    "\033[4m"       /* 16 UNDERLINE 0x6e */
};

void removeBytes(uint8_t *data, size_t data_size, uint8_t toRemove,
                 size_t removeLen);

void transformBytes(uint8_t *data, size_t data_size, uint8_t toTransform,
                    size_t transformLen);

Message *create_message(MessageType type, const char *payload, size_t size)
{
    Message *m;

    m = malloc(sizeof(Message));
    if (m == NULL)
        return NULL;

    memset(m, 0, sizeof(Message));

    m->length = (uint32_t)size + HEADER_SIZE;
    m->id = lastID++;
    m->type = type;
    m->payload = malloc(size);

    if (m->payload == NULL)
    {
        free(m);
        return NULL;
    }

    memcpy(m->payload, payload, size);

    return m;
}

size_t encode_message(Message *m, uint8_t *buf)
{
    uint8_t *base = buf;

    memcpy(buf, &m->length, sizeof(m->length));
    buf += sizeof(m->length);

    memcpy(buf, &m->id, sizeof(m->id));
    buf += sizeof(m->id);

    memcpy(buf, &m->type, sizeof(m->type));
    buf += sizeof(m->type);

    memcpy(buf, m->payload, m->length - HEADER_SIZE);
    buf += m->length - HEADER_SIZE;

    *buf++ = 0;
    *buf++ = 0;

    return buf - base;
}

Message *decode_message(uint8_t *data, ColorBehavior behavior)
{
    uint32_t len;
    uint32_t payload_len;
    uint32_t id;
    MessageType type;
    char *payload = NULL;
    Message *m;

    memcpy(&len, data, sizeof(len));

    payload_len = len - HEADER_SIZE;

    if (len >= SC_MAX_PAYLOAD)
    {
        return NULL;
    }

    data += sizeof(len);
    memcpy(&id, data, sizeof(id));

    data += sizeof(id);
    memcpy(&type, data, sizeof(type));

    if (payload_len != 0)
    {
        data += sizeof(type);
        payload = malloc(payload_len + 1);

        if (payload == NULL)
        {
            return NULL;
        }

        memcpy(payload, data, payload_len);
        payload[payload_len] = 0;

        if (behavior == Strip)
        {
            removeBytes((uint8_t *)payload, payload_len, 0xC2, 3);
        }
        // else
        // {
        //     transformBytes((uint8_t *)data, payload_len, 0xC2, 2);
        // }
    }

    m = malloc(sizeof(Message));

    if (m == NULL && payload != NULL)
    {
        free(payload);
        return NULL;
    }

    m->length = len;
    m->id = id;
    m->type = type;
    m->payload = payload;

    return m;
}

void append_message(Message *m, uint8_t *data, ColorBehavior behavior)
{
    uint32_t len;
    uint32_t payload_len;

    char *payload = NULL;

    memcpy(&len, data, sizeof(len));

    payload_len = len - HEADER_SIZE;

    if (len >= SC_MAX_PAYLOAD)
    {
        return;
    }

    data += sizeof(len) * 2;

    if (payload_len != 0)
    {
        data += sizeof(uint32_t);
        payload = malloc(payload_len + m->length - HEADER_SIZE + 1);

        if (payload == NULL)
        {
            return;
        }

        memcpy(payload, m->payload, m->length - HEADER_SIZE);
        memcpy(payload + (m->length - HEADER_SIZE), data, payload_len);

        payload_len += m->length - HEADER_SIZE;
        payload[payload_len] = 0;

        if (behavior == Strip)
        {
            removeBytes((uint8_t *)payload, payload_len, 0xC2, 3);
        }
    }

    m = malloc(sizeof(Message));

    if (m == NULL && payload != NULL)
    {
        free(payload);
        return;
    }

    m->length = payload_len;

    free(m->payload);
    m->payload = payload;
}

void free_message(Message *m)
{
    if (m != NULL)
    {
        if (m->payload != NULL)
        {
            free(m->payload);
        }

        free(m);
    }
}

void removeBytes(uint8_t *data, size_t data_size, uint8_t toRemove,
                 size_t removeLen)
{
    size_t writeIndex = 0; // Index for where to write the remaining data
    size_t readIndex = 0;  // Index for reading from the original data

    while (readIndex < data_size)
    {
        if (data[readIndex] == toRemove)
        {
            readIndex += removeLen;
        }
        else
        {
            data[writeIndex++] = data[readIndex++];
        }
    }
}

void transformBytes(uint8_t *data, size_t data_size, uint8_t toTransform,
                    size_t transformLen)
{
    size_t writeIndex = 0; // Index for where to write the remaining data
    size_t readIndex = 0;  // Index for reading from the original data
    bool flag = false;

    while (readIndex < data_size)
    {
        if (data[readIndex] == toTransform)
        {
            readIndex += transformLen;
            flag = true;
        }
        else if (flag)
        {
            size_t colorIdx = data[readIndex];

            data[writeIndex] = 0; //(uint8_t)colors[colorIdx];
            flag = false;
        }
        else
        {
            data[writeIndex++] = data[readIndex++];
        }
    }
}
