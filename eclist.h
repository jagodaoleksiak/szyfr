#ifndef ECLIST_H
#define ECLIST_H
#include "encodedcharacter.h"
#include <stdbool.h>

typedef struct _Node {
  EncodedCharacter ec;

  struct _Node *next;
  struct _Node *prev;
} Node;

void node_append(Node *first, Node *second);
Node *node_create(EncodedCharacter ec);
void node_free(Node *node);

typedef struct _EcList {
  Node *head;
  Node *tail;
} EcList;

void list_init(EcList *list);
bool list_empty(EcList list);
void list_append(EcList *list, EncodedCharacter ec);
void list_writeToFile(EcList list, const char *fname);
void list_free(EcList *list);

#endif // ECLIST_H
