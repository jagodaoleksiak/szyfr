#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "encodedcharacter.h"

size_t encodedCharSizeImpl(int number, int system, int x, size_t result)
{
    if(number + 1 <= x)
    {
        return result;
    }
    return encodedCharSizeImpl(number, system, x * system, result + 1);
}

size_t encodedCharSize(char c, int system)
{
    return encodedCharSizeImpl(c, system, system, 1);
}

char digitToChar(unsigned digit)
{
    static char digits[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    return digit > 9 ? '?' : digits[digit];
}

char * encodeNumberToBuffer(char * buf, int system, int number)
{
    int newN = number / system;

    if(newN != 0)
    {
        buf = encodeNumberToBuffer(buf, system, newN);
    }

    *buf = digitToChar((unsigned)(number % system));
    return buf + 1;
}

EncodedCharacter encodeCharacter(char c, int system, char **error_msg)
{
    // printf("encoding character: %c\n", c);

    EncodedCharacter ec;

    if((unsigned)c > 127)
    {
        *error_msg = "cant encode character with ASCII code greater than 127";
        return ec;
    }

    ec.character = c;
    ec.system = system;
    ec.bufSize = encodedCharSize(c, system);
    ec.buf = malloc(ec.bufSize);

    encodeNumberToBuffer(ec.buf, system, c);

    return ec;
}

int decodeEncodedCharacterToChar(EncodedCharacter ec, char * character) {
    int number = strtol(ec.buf, NULL, ec.system);

    if (number < -128 || number > 127) {
        return -1;
    }

    character[0] = (char)number;
    character[1] = 0;

    ec.character = character[0];
    ec_print(ec);

    return 0;
}

EncodedCharacter loadEncodedCharacter(const char* data)
{
	EncodedCharacter ec;
	ec.character = 0;
	ec.system = 0;
	ec.bufSize = strlen(data);
	ec.buf = data;
	return ec;
}

const char * decodeCharacter(EncodedCharacter ec, char **error_msg)
{
    char * c;

    int err = decodeEncodedCharacterToChar(ec, c);

    if (err) {
        *error_msg = "invalid number for given system";
        return c;
    }

    if (c < 0) {
        *error_msg = "cant recognize character with ASCII code lower than 0";
        return c;
    }

	return c;
}

void ec_free(EncodedCharacter ec)
{
    free(ec.buf);
}

void ec_print(EncodedCharacter ec)
{
    char * buf = malloc(ec.bufSize + 1);
    memcpy(buf, ec.buf, ec.bufSize);
    buf[ec.bufSize] = 0;

    printf("EncodedCharacter{%c (%d), %d, %lu, %s}\n", ec.character, ec.character, ec.system, ec.bufSize, ec.buf);

    free(buf);
}
