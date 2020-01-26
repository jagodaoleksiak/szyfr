#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>

#include "encodedcharacter.h"
#include "eclist.h"

int nextSystem(int previousNumber)
{
    return (previousNumber % 8) + 2;
}

void exit_on_error(char * error_msg, EcList * ecList)
{
    if(error_msg)
    {
        printf("error: %s", error_msg);
        list_free(ecList);
        exit(1);
    }
}

void exit_on_decode_error(char * error_msg, char * buffer) {
    if (error_msg) {
        printf("error: %s", error_msg);
        free(buffer);
        exit(1);
    }
}

void readWholeFile(const char * fname, char ** buffer, size_t* size)
{
	FILE* f = fopen(fname, "rb");
	fseek(f, 0, SEEK_END);

    size_t fsize = (size_t)ftell(f);
    fseek(f, 0, SEEK_SET);  /* same as rewind(f); */
	
    *buffer = malloc(fsize + 1);
    fread(*buffer, 1, fsize, f);
    fclose(f);

    (*buffer)[fsize] = 0;
    *size = fsize;
}

void writeWholeBuffer(const char * fname, char * buffer, size_t * size) {
    FILE* f = fopen(fname, "w");

    fwrite(buffer, sizeof(char), *size, f);

    fclose(f);
}

bool skip(int c)
{
    return c == '\r' || c == '\n' || c == EOF; // EOF probably unnecessary
}

void encode(const char * inputFile, const char * outputFile)
{
    char * data;
    size_t dataLen;
    readWholeFile(inputFile, &data, &dataLen);

    int previousNumber = 0;
    EcList ecList;
    list_init(&ecList);
    char * error_msg = NULL;
    for(size_t i = 0; i < dataLen; ++i)
    {
        if(skip(data[i]))
            continue;

        int system = nextSystem(previousNumber);
        previousNumber = data[i];
        EncodedCharacter ec = encodeCharacter(data[i], system, &error_msg);
        exit_on_error(error_msg, &ecList);
        list_append(&ecList, ec);
    }

    list_writeToFile(ecList, outputFile);
    list_free(&ecList);
}

void decode(const char * inputFile, const char * outputFile)
{
    char * data;
    size_t dataLen;
    char * output = malloc(1);
    size_t outputLen;

    readWholeFile(inputFile, &data, &dataLen);

    int actualSystem = 2;

    char * error_msg = NULL;
    char * actualNumber = strtok(data, " ");

    while (actualNumber != NULL) {
        EncodedCharacter ec = loadEncodedCharacter(actualNumber);
        ec.system = actualSystem;

        const char * a = decodeCharacter(ec, &error_msg);
        exit_on_decode_error(error_msg, output);

        char * actualOutput = malloc(strlen(output) + 1);
        strcpy(actualOutput, output);
        free(output);
        output = malloc(strlen(actualOutput) + 2);
        strcpy(output, actualOutput);
        strcat(output, a);
        free(actualOutput);

        actualSystem = nextSystem(*a);
        actualNumber = strtok(NULL, " ");
    }

    outputLen = strlen(output);
    writeWholeBuffer(outputFile, output, &outputLen);
    free(output);
}

int main()
{
	int wybor;
	printf("Wybierz, co chcesz zrobić:\n1 - Chcę zaszyfrować plik\n2 - Chcę rozszyfrować plik\n3-Chcę wyjść z programu\n");
	scanf("%d", &wybor);
	if (wybor == 1)
	{
		printf("encoding...\n");
		encode("input.txt", "output.txt");
		printf("done!\n");
	}
	else if (wybor == 2) {
		printf("decoding...\n");
		decode("output.txt", "old_input.txt");
        printf("done!\n");
    }
	//wyjscie z programu option
	else if (wybor == 3) {
		printf("Program teraz zakończy swoje działanie...\n");
		exit(0);
	}
    return 0;
}
