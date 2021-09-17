#ifndef KEYVALUEPAIR_H
#define KEYVALUEPAIR_H

#include "Defs.h"

typedef struct KeyValuePair_ KeyValuePair;

KeyValuePair *
createKeyValuePair(void *key, void *value, CopyFunction ck, CopyFunction cv, FreeFunction fk, FreeFunction fv,
                   PrintFunction pk, PrintFunction pv, EqualFunction ek);

status destroyKeyValuePair(KeyValuePair *pair);

status displayValue(KeyValuePair *pair);

status displayKey(KeyValuePair *pair);

void *getValue(KeyValuePair *pair);

void *getKey(KeyValuePair *pair);

bool isEqualKey(KeyValuePair *pair, void *keyToCompare);

void *copyPair(KeyValuePair *pair);

#endif
