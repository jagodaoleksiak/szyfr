#ifndef ENCODEDCHARACTER_H
#define ENCODEDCHARACTER_H

#include <stdlib.h>

typedef struct _EncodedCharacter {
	char character; 
	int system;
  size_t bufSize;
  char *buf;
} EncodedCharacter;

EncodedCharacter encodeCharacter(char c, int system, char **error_msg);

EncodedCharacter loadEncodedCharacter(const char *data);

const char * decodeCharacter(EncodedCharacter ec, char **error_msg);

void ec_free(EncodedCharacter ec);

void ec_print(EncodedCharacter ec);

#endif // ENCODEDCHARACTER_H
