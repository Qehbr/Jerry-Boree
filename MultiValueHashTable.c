#include "MultiValueHashTable.h"

struct MultiValueHashTable_s {
    //MultiValueHashTable uses hashTable
    struct hashTable_s *hashArray;
    //function for elements stored in hash
    CopyFunction ck;
    CopyFunction cv;
    FreeFunction fk;
    FreeFunction fv;
    PrintFunction pk;
    PrintFunction pv;
    EqualFunction ek;
    EqualFunction ev;
    TransformIntoNumberFunction tr;
};

MultiValueHashTable
createMultiValueHashTable(CopyFunction copyKey, FreeFunction freeKey, PrintFunction printKey, CopyFunction copyValue,
                          FreeFunction freeValue, PrintFunction printValue, EqualFunction equalKey,
                          EqualFunction equalValue,
                          TransformIntoNumberFunction transformIntoNumber, int hashNumber) {
    //check if functions are not NULL
    if (copyKey == NULL || freeKey == NULL || printKey == NULL || copyValue == NULL || freeValue == NULL ||
        printValue == NULL || equalKey == NULL || transformIntoNumber == NULL) {
        printf("FAILURE, MultiValueHashTable is not created, functions are NULL \n");
        return NULL;
    }
    //allocate memory for MultiHash
    struct MultiValueHashTable_s *multiHash = (struct MultiValueHashTable_s *) malloc(
            sizeof(struct MultiValueHashTable_s));
    //if memory  was not allocated
    if (multiHash == NULL) {
        printf("MEMORY FAILURE, createMultiValueHashTable \n");
        return NULL;
    }
    //create hashArray with !linked list! of values in order to store some values by single key
    multiHash->hashArray = createHashTable(copyKey, freeKey, printKey, (CopyFunction) copyList,
                                           (FreeFunction) destroyList,
                                           (PrintFunction) displayList, equalKey, transformIntoNumber, hashNumber);
    //if memory  was not allocated
    if (multiHash->hashArray == NULL) {
        printf("MEMORY FAILURE, createHashTable in createMultiValueHashTable \n");
        //free memory allocated for MultiHash
        free(multiHash);
        return NULL;
    }
    //initialize functions of elements
    multiHash->ck = copyKey;
    multiHash->cv = copyValue;
    multiHash->fk = freeKey;
    multiHash->fv = freeValue;
    multiHash->pk = printKey;
    multiHash->pv = printValue;
    multiHash->ek = equalKey;
    multiHash->ev = equalValue;
    multiHash->tr = transformIntoNumber;
    //return new empty Multi Hash
    return multiHash;
}

status destroyMultiValueHashTable(MultiValueHashTable multiHash) {
    //if parameter is NULL
    if (multiHash == NULL) {
        printf("FAILURE, cannot destroyMultiValueHashTable, parameter is NULL \n");
        return failure;
    }
    //free memory allocated by hashArray
    destroyHashTable(multiHash->hashArray);
    //free memory allocated by multiHash
    free(multiHash);
    return success;
}

status addToMultiValueHashTable(MultiValueHashTable multiHash, Element key, Element value) {
    //if parameter is NULL
    if (multiHash == NULL || key == NULL || value == NULL) {
        printf("FAILURE, cannot addToMultiValueHashTable, parameter is NULL \n");
        return failure;
    }
    //get list of value by given key
    LinkedList *listToAppend = lookupInMultiValueHashTable(multiHash, key);
    //if given key is not found in hash
    if (listToAppend == NULL) {
        //create newList
        LinkedList *newList = createLinkedList(multiHash->cv, multiHash->fv, multiHash->pv, multiHash->ev, NULL);
        //append value to new list of values
        appendNode(newList, value);
        //add newList with linked list of value to hash by given key (copyList is called, but size of list is 1, then it takes O(1) time)
        addToHashTable(multiHash->hashArray, key, newList);
        //destroyList because copy was created in addToHashTable
        destroyList(newList);
        return success;
    }
        //if given key already exists then add value to list of values of given key
    else {
        return appendNode(listToAppend, value);
    }
}

LinkedList *lookupInMultiValueHashTable(MultiValueHashTable multiHash, Element key) {
    //if parameter is NULL
    if (multiHash == NULL || key == NULL) {
        printf("FAILURE, cannot lookupInMultiValueHashTable, parameter is NULL \n");
        return NULL;
    }
    //get VALUE of pair returned by lookupInHashTable (because we want to return linked list but not pair)
    return getValue(lookupInHashTable(multiHash->hashArray, key));
}


status removeFromMultiValueHashTable(MultiValueHashTable multiHash, Element key, void *value) {
    //if parameter is NULL
    if (multiHash == NULL || key == NULL || value == NULL) {
        printf("FAILURE, cannot removeFromMultiValueHashTable, parameter is NULL \n");
        return failure;
    }
    //get list of values by given key
    LinkedList *listToDelete = lookupInMultiValueHashTable(multiHash, key);
    if (listToDelete != NULL) {
        //delete given value from list
        deleteNode(listToDelete, value);
        //if there are not values
        if (getLengthList(listToDelete) == 0) {
            //delete given key from hash
            removeFromHashTable(multiHash->hashArray, key);
        }
        return success;
    }
    //if list of values of given key is NULL then we cannot remove value
    return failure;
}

status displayMultiValueHashElementsByKey(MultiValueHashTable multiHash, void *key) {
    //if parameter is NULL
    if (multiHash == NULL || key == NULL) {
        printf("FAILURE, cannot displayMultiValueHashElementsByKey, parameter is NULL \n");
        return failure;
    }
    //get list of values by given key
    LinkedList *listToPrint = lookupInMultiValueHashTable(multiHash, key);
    //if key is not in MultiHash
    if (listToPrint == NULL) {
        printf("FAILURE, cannot displayMultiValueHashElementsByKey, key is not in MultiHash \n");
        return failure;
    }
    //print key
    multiHash->pk(key);
    //print list of values by given key
    displayList(listToPrint);
    return success;
}