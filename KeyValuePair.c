#include "KeyValuePair.h"

struct KeyValuePair_ {
    //pointers to key and value of KeyValuePair
    void *key;
    void *value;
    //function for elements stored in KeyValuePair
    CopyFunction ck;
    CopyFunction cv;
    FreeFunction fk;
    FreeFunction fv;
    PrintFunction pk;
    PrintFunction pv;
    EqualFunction ek;
};

KeyValuePair *
createKeyValuePair(void *key, void *value, CopyFunction ck, CopyFunction cv, FreeFunction fk, FreeFunction fv,
                   PrintFunction pk, PrintFunction pv, EqualFunction ek) {
    //check if functions are not NULL
    if (key == NULL || value == NULL || ck == NULL || cv == NULL || fk == NULL || fv == NULL || pk == NULL ||
        pv == NULL || ek == NULL) {
        printf("FAILURE, KeyValuePair is not created, functions are NULL \n");
        printf("FAILURE");
        return NULL;
    }
    //allocate memory for KeyValuePair
    KeyValuePair *pair = (KeyValuePair *) malloc(sizeof(KeyValuePair));
    //if memory  was not allocated
    if (pair == NULL) {
        printf("MEMORY FAILURE, createKeyValuePair \n");
        return NULL;
    }
    //initialize functions of KeyValuePair
    pair->ck = ck;
    pair->cv = cv;
    pair->fk = fk;
    pair->fv = fv;
    pair->pk = pk;
    pair->pv = pv;
    pair->ek = ek;
    //copy key and value to KeyValuePair
    pair->key = pair->ck(key);
    pair->value = pair->cv(value);
    //return pointer to new KeyValuePair
    return pair;
}

status displayValue(KeyValuePair *pair) {
    //if parameter is NULL
    if (pair == NULL) {
        printf("FAILURE, cannot displayValue, parameter is NULL \n");
        return failure;
    }
    //print value of given pair
    pair->pv(pair->value);
    return success;
}

status displayKey(KeyValuePair *pair) {
    //if parameter is NULL
    if (pair == NULL) {
        printf("FAILURE, cannot displayValue, parameter is NULL \n");
        return failure;
    }
    //print key of given pair
    pair->pk(pair->key);
    return success;
}

void *getValue(KeyValuePair *pair) {
    //if parameter is NULL
    if (pair == NULL) {
        return NULL;
    }
    //return value of given pair
    return pair->value;
}

void *getKey(KeyValuePair *pair) {
    //if parameter is NULL
    if (pair == NULL) {
        return NULL;
    }
    //return key of given pair
    return pair->key;
}

bool isEqualKey(KeyValuePair *pair, void *keyToCompare) {
    //if parameter is NULL
    if (pair == NULL || keyToCompare == NULL) {
        printf("FAILURE, cannot isEqualKey, parameter is NULL \n");
        return false;
    }
    //if keys are equal
    if (pair->ek(pair->key, keyToCompare) == true) {
        return true;
    }
    return false;

}

status destroyKeyValuePair(KeyValuePair *pair) {
    //if parameter is NULL
    if (pair == NULL) {
        printf("FAILURE, cannot destroyKeyValuePair, parameter is NULL \n");
        return failure;
    }
    //free memory allocated by elements stored in pair
    pair->fk(pair->key);
    pair->fv(pair->value);
    //free memory allocated by pair
    free(pair);
    return success;
}

void *copyPair(KeyValuePair *pair) {
    //create new pair with the same key, value and functions
    return createKeyValuePair(getKey(pair), getValue(pair), pair->ck, pair->cv, pair->fk, pair->fv, pair->pk, pair->pv,
                              pair->ek);
}
