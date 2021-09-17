#include "Jerry.h"


Jerry *createJerry(char *ID, int happiness, Origin *origin) {
    //check if happiness is not in given range and return NULL if it is not
    if (happiness > 100 || happiness < 0) {
        printf("JEEZ MORTY *burp* WHY YOU GIVE ME THIS DEFECTIVE JERRY?! \n");
        return NULL;
    }
    //allocate memory for new Jerry
    Jerry *jerry = (Jerry *) malloc(sizeof(Jerry));
    //if memory was not allocated return NULL
    if (jerry == NULL) {
        printf("RICK MEMORY WAS NOT ALLOCATED \n");
        return NULL;
    }
    //initialize new jerry
    strcpy(jerry->ID, ID);
    jerry->happiness = happiness;
    jerry->origin = origin;
    jerry->characteristicsNum = 0;
    jerry->phc = NULL;
    //return pointer to new jerry
    return jerry;
}

void deleteJerry(Jerry *jerry) {
    //free all physicalCharacteristics of given Jerry
    for (int i = 0; i < jerry->characteristicsNum; ++i) {
        deletePC(jerry->phc[i]);
    }
    //free memory allocated for physicalCharacteristics of Jerry
    free(jerry->phc);
    //free jerry itself
    free(jerry);
}

Planet *createPlanet(char *name, double x, double y, double z) {
    //allocate memory for new Jerry
    Planet *planet = (Planet *) malloc(sizeof(Planet));
    //if memory was not allocated return NULL
    if (planet == NULL) {
        printf("RICK MEMORY WAS NOT ALLOCATED \n");
        return NULL;
    }
    //initialize new planet
    strcpy(planet->name, name);
    planet->x = x;
    planet->y = y;
    planet->z = z;
    return planet;
}

void deletePlanet(Planet *planet) {
    //free planet itself
    free(planet);
}

PhysicalCharacteristics *createPC(char *name, double value) {
    //allocate memory for new physicalCharacteristic
    PhysicalCharacteristics *pc = (PhysicalCharacteristics *) malloc(sizeof(PhysicalCharacteristics));
    //if memory was not allocated return NULL
    if (pc == NULL) {
        printf("RICK MEMORY WAS NOT ALLOCATED \n");
        return NULL;
    }
    //initialize new physicalCharacteristic
    strcpy(pc->name, name);
    pc->value = value;
    return pc;
}

void deletePC(PhysicalCharacteristics *pc) {
    //free physicalCharacteristic itself
    free(pc);
}

Origin *createOrigin(Planet *planet, char *dimensionName) {
    //allocate memory for new Origin
    Origin *origin = (Origin *) malloc(sizeof(Origin));
    //if memory was not allocated return NULL
    if (origin == NULL) {
        printf("RICK MEMORY WAS NOT ALLOCATED \n");
        return NULL;
    }
    //initialize new origin
    origin->planet = planet;
    strcpy(origin->dimensionName, dimensionName);
    return origin;
}

void deleteOrigin(Origin *origin) {
    //free origin itself
    free(origin);
}

bool checkIfHasPC(Jerry *jerry, char *pcName) {
    //iterate through physicalCharacteristics of given jerry and check if he has given physicalCharacteristic by its name
    int i;
    for (i = 0; i < jerry->characteristicsNum; ++i) {
        if (strcmp(jerry->phc[i]->name, pcName) == 0) {
            return true;
        }
    }
    return false;
}

void addPC(Jerry *jerry, PhysicalCharacteristics *pc) {
    //check if given Jerry has given physicalCharacteristic, if does not
    if (!checkIfHasPC(jerry, pc->name)) {
        //realloc memory for new physicalCharacteristic
        jerry->phc = realloc(jerry->phc, sizeof(PhysicalCharacteristics) * ((jerry->characteristicsNum) + 1));
        //add new physicalCharacteristic to physicalCharacteristics of given Jerry
        jerry->phc[jerry->characteristicsNum] = pc;
        //increment number of physicalCharacteristics of given Jerry
        ++jerry->characteristicsNum;
    } else {
        //if given Jerry already has given physicalCharacteristic free memory that was allocated for physicalCharacteristic
        deletePC(pc);
    }
}

void deletePCfromJerry(Jerry *jerry, char *pcName) {
    //check if given Jerry has given physicalCharacteristic, if he does
    if (checkIfHasPC(jerry, pcName)) {
        int i;
        int j;
        int indexToDelete = 0;
        //find index of physicalCharacteristic we need to delete
        for (i = 0; i < jerry->characteristicsNum; ++i) {
            if (strcmp(jerry->phc[i]->name, pcName) == 0) {
                indexToDelete = i;
                break;
            }
        }
        //create new array of physicalCharacteristics with a size smaller by 1
        PhysicalCharacteristics **temp = malloc(sizeof(PhysicalCharacteristics) * (jerry->characteristicsNum) - 1);
        //if memory was not allocated
        if (temp == NULL) {
            printf("RICK MEMORY WAS NOT ALLOCATED \n");
            return;
        }
        //free memory allocated for given physicalCharacteristic
        deletePC(jerry->phc[indexToDelete]);
        //initialize new array of physicalCharacteristics without physicalCharacteristic we should delete
        for (i = 0, j = 0; i < jerry->characteristicsNum; ++i, ++j) {
            if (i == indexToDelete) {
                --j;
            } else {
                temp[j] = jerry->phc[i];
            }
        }
        //decrement number of physicalCharacteristics of given Jerry
        --jerry->characteristicsNum;
        //free old array of physicalCharacteristics of given Jerry
        free(jerry->phc);
        //give him new array without physicalCharacteristic we should delete
        jerry->phc = temp;
    }
}

void printPlanet(Planet *planet) {
    printf("Planet : %s (%0.2lf,%0.2lf,%0.2lf) \n", planet->name, planet->x, planet->y, planet->z);
}

void printJerry(Jerry *jerry) {
    printf("Jerry , ID - %s : \n", jerry->ID);
    printf("Happiness level : %d \n", jerry->happiness);
    printf("Origin : %s \n", jerry->origin->dimensionName);
    printPlanet(jerry->origin->planet);
    if (jerry->characteristicsNum > 0) {
        printf("Jerry's physical Characteristics available : \n");
        printf("\t");
        for (int i = 0; i < jerry->characteristicsNum - 1; ++i) {
            printf("%s : %0.2lf , ", jerry->phc[i]->name, jerry->phc[i]->value);
        }
        if (jerry->characteristicsNum > 0) {
            printf("%s : %0.2lf \n", jerry->phc[jerry->characteristicsNum - 1]->name,
                   jerry->phc[jerry->characteristicsNum - 1]->value);
        }
    }
}

char *stringUpper(char *str) {
    int i = 0;
    //iterate through string and upp each char
    while (str[i] != '\0') {
        if (str[i] >= 'a' && str[i] <= 'z') {
            str[i] = str[i] - 32;
        }
        i++;
    }
    return str;
}

double diff(double a, double b) {
    //absolute difference between 2 doubles
    if (b > a) {
        return b - a;
    }
    return a - b;
}


Jerry *returnJerry(Jerry *jerry) {
    //do not copy jerry, return the same pointer
    return jerry;
}

bool equalJerry(Jerry *jerry1, Jerry *jerry2) {
    //check if jerries have the same address
    return jerry1 == jerry2;
}

Planet *returnPlanet(Planet *planet) {
    //do not copy planet, return the same pointer
    return planet;
}

bool equalPlanet(Planet *planet1, Planet *planet2) {
    //check if the planets have the same address
    return planet1 == planet2;
}

Origin *returnOrigin(Origin *origin) {
    //do not copy origin, return the same pointer
    return origin;
}

bool equalOrigin(Origin *origin1, Origin *origin2) {
    //check if the origins have the same address
    return origin1 == origin2;
}

status printOrigin(Origin *origin) {
    //if parameter is NULL
    if (origin == NULL) {
        printf("FAILURE, cannot printOrigin, parameter is NULL \n");
    }
    printf("Origin: %s \n", origin->dimensionName);
    return success;
}


char *copyJerryID(char *id) {
    //copy name of given id
    char *newID = (char *) malloc(sizeof(char) * (strlen(id) + 1));
    strcpy(newID, id);
    return newID;
}

status freeJerryID(char *id) {
    free(id);
    return success;
}

bool equalsID(char *id1, char *id2) {
    //check if name of pc1 is the same as name of pc2
    return strcmp(id1, id2) == 0;
}

status printJerryID(char *id) {
    printf("Jerry ID : %s \n", id);
    return success;
}

status freeJerry(Jerry *jerry) {
    //freeJerry does nothing because memory was not allocated
    return success;
}


int transformStringIntoInt(const char *string) {
    //transform string into int by using ascii
    int i = 0;
    int value = 0;
    char ch = string[i];
    while (ch != '\0') {
        value += (int) ch;
        i++;
        ch = string[i];
    }
    return value;
}


char *copyPC(char *pc) {
    //copy name of given pc
    char *newPC = (char *) malloc(sizeof(char) * (strlen(pc) + 1));
    strcpy(newPC, pc);
    return newPC;
}

status freePC(char *pc) {
    //free memory allocated by name of given pc
    free(pc);
    return success;
}

bool equalPC(char *pc1, char *pc2) {
    //check if name of pc1 is the same as name of pc2
    return strcmp(pc1, pc2) == 0;
}

status printPC(char *pc) {
    //print physical characteristic as key
    printf("%s : \n", pc);
    return success;
}

bool checkPlanetName(Planet *planet, char *planetName) {
    //if parameter is NULL
    if (planet == NULL) {
        printf("FAILURE, cannot checkPlanetName, parameter is NULL \n");
        return false;
    }
    //check if given planet has the same name
    return strcmp(planet->name, planetName) == 0;
}

bool checkPCName(PhysicalCharacteristics *pc, char *pcName) {
    //if parameter is NULL
    if (pc == NULL) {
        printf("FAILURE, cannot checkPCName, parameter is NULL \n");
        return false;
    }
    //check if given pc has the same name
    return strcmp(pc->name, pcName) == 0;
}



