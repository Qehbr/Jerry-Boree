#ifndef ASSIGNMENT2_MULTIVALUEHASHTABLE_H
#define ASSIGNMENT2_MULTIVALUEHASHTABLE_H

#include "Defs.h"
#include "LinkedList.h"
#include "KeyValuePair.h"
#include "HashTable.h"

typedef struct MultiValueHashTable_s *MultiValueHashTable;

MultiValueHashTable
createMultiValueHashTable(CopyFunction copyKey, FreeFunction freeKey, PrintFunction printKey, CopyFunction copyValue,
                          FreeFunction freeValue, PrintFunction printValue, EqualFunction equalKey, EqualFunction ev,
                          TransformIntoNumberFunction transformIntoNumber, int hashNumber);

status destroyMultiValueHashTable(MultiValueHashTable);

status addToMultiValueHashTable(MultiValueHashTable, Element key, Element value);

LinkedList *lookupInMultiValueHashTable(MultiValueHashTable, Element key);

status removeFromMultiValueHashTable(MultiValueHashTable, Element key, void *value);

status displayMultiValueHashElementsByKey(MultiValueHashTable, void *key);


#endif
