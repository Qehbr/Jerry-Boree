#include "LinkedList.h"

struct LinkedList_ {
    //size variable represents number of nodes in linked list
    int size;
    //pointers to head and tail of linked list
    Node *head;
    Node *tail;
    //function for elements stored in linked list
    CopyFunction copyElement;
    FreeFunction freeElement;
    PrintFunction printElement;
    EqualFunction equalsElement;
    EqualFunction equalsValue;
};

struct Node_ {
    //node has pointer to next node
    Node *next;
    //generic data stored in linked list
    void *data;
};


LinkedList *createLinkedList(CopyFunction copy, FreeFunction free, PrintFunction print, EqualFunction equals,
                             EqualFunction equalsValue) {
    //check if functions are not NULL
    //NOTE: equalsValue CAN be NULL but function "getDataByIndex" will NOT be implemented
    if (copy == NULL || free == NULL || print == NULL || equals == NULL) {
        printf("FAILURE, Linked list is not created, functions are NULL \n");
        return NULL;
    }
    //allocate memory for linked list
    LinkedList *list = (LinkedList *) malloc(sizeof(LinkedList));
    //if memory  was not allocated
    if (list == NULL) {
        printf("MEMORY FAILURE, createLinkedList \n");
        return NULL;
    }
    //initialize functions of linked list
    list->copyElement = copy;
    list->freeElement = free;
    list->printElement = print;
    list->equalsElement = equals;
    list->equalsValue = equalsValue;
    //make linked list empty
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    //return pointer to new empty linked list
    return list;
}

status appendNode(LinkedList *list, void *element) {
    //if parameter is NULL
    if (list == NULL || element == NULL) {
        printf("FAILURE, cannot appendNode, parameter is NULL \n");
        return failure;
    }
    //allocate memory for new node
    Node *nodeToAppend = (Node *) malloc(sizeof(Node));
    //if memory was not allocated
    if (nodeToAppend == NULL) {
        printf("MEMORY FAILURE, node \n");
        return failure;
    }
    //initialize variables of node
    nodeToAppend->data = list->copyElement(element);
    nodeToAppend->next = NULL;
    //increment size of linked list
    list->size++;
    //add node to the list
    if (list->head == NULL) {
        list->head = nodeToAppend;
        list->tail = nodeToAppend;
    } else {
        list->tail->next = nodeToAppend;
        list->tail = nodeToAppend;
    }
    return success;
}

status displayList(LinkedList *list) {
    //if parameter is NULL
    if (list == NULL) {
        printf("FAILURE, cannot displayList, parameter is NULL \n");
        return failure;
    }
    //iterate through list
    Node *temp = list->head;
    int i;
    for (i = 0; i < list->size; ++i) {
        //print elements
        list->printElement(temp->data);
        if (temp->next != NULL) {
            temp = temp->next;
        }
    }
    return success;
}

void *getDataByIndex(LinkedList *list, int index) {
    //if parameter is NULL
    if (list == NULL) {
        printf("FAILURE, cannot getDataByIndex, parameter is NULL \n");
        return NULL;
    }
    //if linked list does not have index's element
    if (index > list->size - 1) {
        printf("LinkedList does not have %d's element \n", index);
        return NULL;
    }
    //iterate through list
    Node *temp = list->head;
    int i;
    for (i = 0; i < index; ++i) {
        if (temp->next != NULL) {
            temp = temp->next;
        }
    }
    //return found data
    return temp->data;
}

int getLengthList(LinkedList *list) {
    //if parameter is NULL
    if (list == NULL) {
        printf("FAILURE, cannot getLengthList, parameter is NULL \n");
        return -1;
    }
    //return size variable
    return list->size;
}

void *searchByKeyInList(LinkedList *list, void *element) {
    //if parameter is NULL
    if (list == NULL || element == NULL) {
        printf("FAILURE, cannot searchByKeyInList, parameter is NULL \n");
        return NULL;
    }
    //if searchByKeyInList is not implemented in given list
    if (list->equalsValue == NULL) {
        printf("FAILURE, searchByKeyInList is not implemented, equalsValue is NULL \n");
        return NULL;
    }
    //iterate through list
    Node *temp = list->head;
    int i;
    for (i = 0; i < list->size; ++i) {
        if (list->equalsValue(temp->data, element)) {
            //return found data
            return temp->data;
        }
        if (temp->next != NULL) {
            temp = temp->next;
        }
    }
    //if data is not found return NULL
    return NULL;
}

status deleteNode(LinkedList *list, void *elementToDelete) {
    //if list is NULL
    if (list == NULL) {
        printf("FAILURE, cannot deleteNode, list is NULL \n");
        return failure;
    }
    //if element is NULL
    if (elementToDelete == NULL) {
        return failure;
    }
    //if there are no elements in list
    if (list->size == 0) {
        printf("FAILURE, cannot deleteNode, list size is 0 \n");
        return failure;
    }
    //if elementToDelete is head
    if (list->equalsElement(list->head->data, elementToDelete)) {
        //free memory allocated by element
        list->freeElement(list->head->data);
        Node *tempNext = list->head->next;
        //free node itself
        free(list->head);
        list->head = tempNext;
        if (list->size == 1) {
            list->tail = NULL;
        }
        //decrement size variable
        list->size--;
        return success;
    }
    //if size of list is >1
    if (list->size > 1) {
        //iterate through list
        Node *beforeTemp = list->head;
        Node *temp = list->head->next;
        int i;
        for (i = 0; i < (list->size) - 1; ++i) {
            //if current item is elementToDelete
            if (list->equalsElement(elementToDelete, temp->data)) {
                if (list->tail == temp) {
                    list->tail = beforeTemp;
                }
                //free memory allocated by element
                list->freeElement(temp->data);
                beforeTemp->next = temp->next;
                //free node itself
                free(temp);
                //decrement size variable
                --list->size;
                return success;
            }
            if (temp->next != NULL) {
                beforeTemp = temp;
                temp = temp->next;
            }
        }
    }
    return failure;
}

status destroyList(LinkedList *listToDelete) {
    //if parameter is NULL
    if (listToDelete == NULL) {
        printf("FAILURE, cannot destroyList, parameter is NULL \n");
        return failure;
    }
    //iterate through list
    int numberOfNodes = listToDelete->size;
    int i;
    for (i = 0; i < numberOfNodes; ++i) {
        //delete head (the most optimal way to delete list in O(n) time)
        deleteNode(listToDelete, listToDelete->head->data);
    }
    //free memory allocated by list
    free(listToDelete);
    return success;
}


LinkedList *copyList(LinkedList *list) {
    //if parameter is NULL
    if (list == NULL) {
        printf("FAILURE, cannot copyList, parameter is NULL \n");
        return NULL;
    }
    //create new list
    LinkedList *newList = createLinkedList(list->copyElement, list->freeElement, list->printElement,
                                           list->equalsElement, list->equalsValue);
    //if list was not created
    if (newList == NULL) {
        printf("MEMORY FAILURE, copyList");
        return NULL;
    }
    //iterate through original list
    Node *temp = list->head;
    int i;
    for (i = 0; i < getLengthList(list); ++i) {
        //append nodes to new list O(n)
        appendNode(newList, temp->data);
        temp = temp->next;
    }
    //return copy of given list
    return newList;
}


/** THE NEXT FUNCTIONS USED FOR ITERATING THROUGH LIST OUTSIDE OF LinkedList.c
 FOR BETTER TIME COMPLEXITY AND PERFORMANCE **/
Node *getHead(LinkedList *list) {
    //return head of the list
    return list->head;
}

void *getData(Node *node) {
    //return data stored by given node
    return node->data;
}

Node *getNext(Node *node) {
    //return next node of given node
    return node->next;
}




