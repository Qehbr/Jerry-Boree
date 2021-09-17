#ifndef DEFS_H_
#define DEFS_H_
#define STRING_LIMIT 300
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//bool enum
typedef enum e_bool {
    false, true
} bool;

//status enum
typedef enum e_status {
    success, failure
} status;



typedef void *Element;

typedef Element(*CopyFunction)(Element);

typedef status(*FreeFunction)(Element);

typedef status(*PrintFunction)(Element);

typedef int(*TransformIntoNumberFunction)(Element);

typedef bool(*EqualFunction)(Element, Element);

#endif /* DEFS_H_ */
