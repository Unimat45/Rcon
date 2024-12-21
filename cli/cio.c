#include "cio.h"

#include <string.h>

#ifdef _WIN32
#include <stdlib.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

void write_string(const char *const s)
{
    if (s == NULL)
        return;

    int len = MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, s, -1, NULL, 0);
    if (len <= 0)
        return;

    WCHAR *buf = (WCHAR *)malloc(len * sizeof(WCHAR));
    if (buf == NULL)
        return;

    if (MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, s, -1, buf, len) == 0)
    {
        free(buf);
        return;
    }

    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    if (out == INVALID_HANDLE_VALUE || out == NULL)
    {
        free(buf);
        return;
    }

    DWORD written;
    if (!WriteConsoleW(out, buf, len - 1, &written, NULL))
    {
        free(buf);
        return;
    }

    free(buf);
}

size_t read_string(char *s)
{
    WCHAR buf[4110];
    DWORD read;

    HANDLE in = GetStdHandle(STD_INPUT_HANDLE);

    ReadConsoleW(in, buf, 4109, &read, NULL);
    buf[read] = L'\0';

    // Ignores the \r\n
    read -= 2;

    int bytes = WideCharToMultiByte(CP_UTF8, 0, buf, read, s, 4110, NULL, NULL);
    s[bytes] = 0;

    return read;
}

#else

#include <unistd.h>

void write_string(const char *const s) { write(STDOUT_FILENO, s, strlen(s)); }

size_t read_string(char *s)
{
    size_t bytes = read(STDIN_FILENO, s, 4110);

    // Ignores the \n
    s[bytes - 1] = 0;

    return bytes;
}

#endif