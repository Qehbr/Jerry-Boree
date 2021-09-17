#include "HashTable.h"
#include "LinkedList.h"
#include "KeyValuePair.h"

struct hashTable_s {
    //array of linked lists representing hash
    LinkedList **hashArray;
    //size of hashArray
    int hashArraySize;
    //function for elements stored in hash
    CopyFunction ck;
    CopyFunction cv;
    FreeFunction fk;
    FreeFunction fv;
    PrintFunction pk;
    PrintFunction pv;
    EqualFunction ek;
    TransformIntoNumberFunction tr;
};

//hash function
int hashFunction(struct hashTable_s *hash, void *keyToHash) {
    return (hash->tr(keyToHash) % hash->hashArraySize);
}


hashTable createHashTable(CopyFunction copyKey, FreeFunction freeKey, PrintFunction printKey, CopyFunction copyValue,
                          FreeFunction freeValue, PrintFunction printValue, EqualFunction equalKey,
                          TransformIntoNumberFunction transformIntoNumber, int hashNumber) {
    //check if functions are not NULL
    if (copyKey == NULL || freeKey == NULL || printKey == NULL || copyValue == NULL || freeValue == NULL ||
        printValue == NULL || equalKey == NULL || transformIntoNumber == NULL) {
        printf("FAILURE, Hash Table is not created, functions are NULL \n");
        return NULL;
    }
    //allocate memory for hash table
    struct hashTable_s *hash = (struct hashTable_s *) malloc(sizeof(struct hashTable_s));
    //if memory  was not allocated
    if (hash == NULL) {
        printf("MEMORY FAILURE, createHashTable \n");
        return NULL;
    }
    //allocate memory for hashArray
    hash->hashArray = (LinkedList **) malloc(sizeof(LinkedList *) * hashNumber);
    //if memory  was not allocated
    if (hash->hashArray == NULL) {
        printf("MEMORY FAILURE, createHashArray in createHashTable \n");
        //free memory allocated for hash
        free(hash);
        return NULL;
    }
    //iterate through hashArray and initialize each cell with linked list
    int i;
    for (i = 0; i < hashNumber; ++i) {
        hash->hashArray[i] = createLinkedList((CopyFunction) copyPair, (FreeFunction) destroyKeyValuePair,
                                              (PrintFunction) displayValue,
                                              (EqualFunction) equalKey, (EqualFunction) isEqualKey);
    }
    //initialize size of hashArray
    hash->hashArraySize = hashNumber;
    //initialize functions of elements
    hash->ck = copyKey;
    hash->cv = copyValue;
    hash->fk = freeKey;
    hash->fv = freeValue;
    hash->pk = printKey;
    hash->pv = printValue;
    hash->ek = equalKey;
    hash->tr = transformIntoNumber;
    //return new empty hash
    return hash;
}

status destroyHashTable(hashTable hash) {
    //if parameter is NULL
    if (hash == NULL) {
        printf("FAILURE, cannot destroyHashTable, parameter is NULL \n");
        return failure;
    }
    //iterate through each cell in HashArray
    int i;
    for (i = 0; i < hash->hashArraySize; ++i) {
        //destroy list in each cell
        destroyList(hash->hashArray[i]);
    }
    //free memory allocated for hashArray
    free(hash->hashArray);
    //free hash itself
    free(hash);
    return success;
}

status addToHashTable(hashTable hash, Element key, Element value) {
    //if parameter is NULL
    if (hash == NULL || key == NULL || value == NULL) {
        printf("FAILURE, cannot addToHashTable, parameter is NULL \n");
        return failure;
    }
    //get list by given key
    LinkedList *list = hash->hashArray[hashFunction(hash, key)];
    if (list==NULL){
        return failure;
    }
    //iterate through list and find pair with given key
    if (searchByKeyInList(list, key))
    {
        //if pair by given key found, do NOT add it to hash
        return failure;
    }
    //create newPair with given key and value
    KeyValuePair *pairToAppend = createKeyValuePair(key, value, hash->ck, hash->cv, hash->fk, hash->fv, hash->pk,
                                                    hash->pv, hash->ek);
    //append newPair to list by given key
    appendNode(list, pairToAppend);
    //destroy pair (copy of pair created in appendNode)
    destroyKeyValuePair(pairToAppend);
    return success;
}

Element lookupInHashTable(hashTable hash, Element key) {
    //if parameter is NULL
    if (hash == NULL || key == NULL) {
        printf("FAILURE, cannot lookupInHashTable, parameter is NULL \n");
        return NULL;
    }
    //get list by given key
    LinkedList *list = hash->hashArray[hashFunction(hash, key)];
    if (list==NULL){
        return NULL;
    }
    //iterate through list and find pair with given key
    return searchByKeyInList(list, key);
}

status removeFromHashTable(hashTable hash, Element key) {
    //if parameter is NULL
    if (hash == NULL || key == NULL) {
        printf("FAILURE, cannot removeFromHashTable, parameter is NULL \n");
        return failure;
    }
    //find pair by given key and delete it from list by given key
    return deleteNode(hash->hashArray[hashFunction(hash, key)], lookupInHashTable(hash, key));
}

status displayHashElements(hashTable hash) {
    //if parameter is NULL
    if (hash == NULL) {
        printf("FAILURE, cannot displayHashElements, parameter is NULL \n");
        return failure;
    }
    //iterate through hashArray
    int i;
    for (i = 0; i < hash->hashArraySize; ++i) {
        //display list in each cell
        displayList(hash->hashArray[i]);
    }
    return success;
}
