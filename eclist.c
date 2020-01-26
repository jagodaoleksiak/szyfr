#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include "eclist.h"

void node_append(Node *first, Node *second)
{
    first->next = second;
    second->prev = first;
}

Node *node_create(EncodedCharacter ec)
{
    Node * node = malloc(sizeof (Node));
    node->prev = NULL;
    node->next = NULL;
    node->ec = ec;
    return node;
}

void node_free(Node *node)
{
    if(node->prev)
    {
        node->prev->next = node->next;
    }

    if(node->next)
    {
        node->next->prev = node->prev;
    }

    ec_free(node->ec);
    free(node);
}

void list_init(EcList * list)
{
    list->head = list->tail = NULL;
}

bool list_empty(EcList list)
{
    return list.head == NULL;
}

void list_append(EcList *list, EncodedCharacter ec)
{
    Node * newNode = node_create(ec);

    if(list_empty(*list))
    {
        list->head = list->tail = newNode;
    }
    else
    {
        node_append(list->tail, newNode);
        list->tail = newNode;
    }
}

void list_writeToFile(EcList list, const char *fname)
{
    FILE * f = fopen(fname, "w");

    if(list_empty(list))
    {
        fclose(f);
        return;
    }

    Node * node = list.head;
    while(node)
    {
        ec_print(node->ec);
        fwrite(node->ec.buf, 1, node->ec.bufSize, f);
        fwrite(" ", 1, 1, f);
        node = node->next;
    }

    fclose(f);
}

void list_free(EcList *list)
{
    if(list_empty(*list))
    {
        return;
    }

    if(list->head == list->tail)
    {
        node_free(list->head);
        list_init(list);
    }
    else
    {
        list->tail = list->tail->prev;
        node_free(list->tail->next);
    }
}
