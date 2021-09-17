#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "Defs.h"

typedef struct LinkedList_ LinkedList;
typedef struct Node_ Node;

LinkedList *createLinkedList(CopyFunction copy, FreeFunction free, PrintFunction print, EqualFunction equal,
                             EqualFunction equalValue);

status destroyList(LinkedList *listToDelete);

status appendNode(LinkedList *list, void *element);

status deleteNode(LinkedList *list, void *elementToDelete);

status displayList(LinkedList *list);

void *getDataByIndex(LinkedList *list, int index);

int getLengthList(LinkedList *list);

void *searchByKeyInList(LinkedList *list, void *element);

LinkedList *copyList(LinkedList *list);

Node *getHead(LinkedList *list);

void *getData(Node *node);

Node *getNext(Node *node);

#endif
