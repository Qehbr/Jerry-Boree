#include "stdlib.h"
#include <stdio.h>
#include "string.h"
#include "Defs.h"

//string limit is maximum chars in all of strings in program
#ifndef ASSIGNMENT3_JERRY_H
#define ASSIGNMENT3_JERRY_H


////structs////

typedef struct Planet_ {
    char name[STRING_LIMIT];
    double x, y, z;
} Planet;

typedef struct Origin_ {
    Planet *planet;
    char dimensionName[STRING_LIMIT];
} Origin;


typedef struct PhysicalCharacteristics_ {
    char name[STRING_LIMIT];
    double value;
} PhysicalCharacteristics;

typedef struct Jerry_ {
    char ID[STRING_LIMIT];
    int happiness; //(0-100)
    Origin *origin;
    PhysicalCharacteristics **phc;
    int characteristicsNum;
} Jerry;

////functions declaration////


//createJerry gets all information about new Jerry and returns pointer to it
Jerry *createJerry(char *ID, int happiness, Origin *origin);

//deleteJerry gets pointer to Jerry and frees all memory allocated for given jerry
void deleteJerry(Jerry *jerry);

//createPlanet gets all information about new Planet and returns pointer to it
Planet *createPlanet(char *name, double x, double y, double z);

//deletePlanet gets pointer to Planet and frees all memory allocated for given planet
void deletePlanet(Planet *planet);

//createPC gets all information about new PhysicalCharacteristic and returns pointer to it
PhysicalCharacteristics *createPC(char *name, double value);

//deletePC gets pointer to PhysicalCharacteristic and frees all memory allocated for given physicalCharacteristic
void deletePC(PhysicalCharacteristics *pc);

//createOrigin gets all information about new Origin and returns pointer to it
Origin *createOrigin(Planet *planet, char *dimensionName);

//deleteOrigin gets pointer to Origin and frees all memory allocated for given origin
void deleteOrigin(Origin *origin);

//checkIfHasPC checks if given jerry has given physicalCharacteristic by its name
bool checkIfHasPC(Jerry *jerry, char *pcName);

//addPC add given physicalCharacteristic to given jerry
void addPC(Jerry *jerry, PhysicalCharacteristics *pc);

//deletePCfromJerry deletes from given jerry physicalCharacteristic by its name
void deletePCfromJerry(Jerry *jerry, char *pcName);

//printJerry prints all information about given Jerry
void printJerry(Jerry *jerry);

//printPlanet prints all information about given Planet
void printPlanet(Planet *planet);

//utility function used for uppercase all letters in given string
char *stringUpper(char *str);

double diff(double a, double b);

Jerry* returnJerry(Jerry* jerry);

bool equalJerry(Jerry* jerry1, Jerry* jerry2);

Planet* returnPlanet(Planet* planet);

bool equalPlanet(Planet* planet1, Planet* planet2);

Origin* returnOrigin(Origin* origin);

bool equalOrigin(Origin* origin1, Origin* origin2);

status printOrigin(Origin* origin);

char *copyJerryID(char *id);

status freeJerryID(char *id);

status printJerryID(char* id);

status freeJerry(Jerry* jerry);

bool equalsID(char* id1, char* id2);

int transformStringIntoInt(const char* string);

char *copyPC(char *pc);

status freePC(char *pc);

bool equalPC(char *pc1, char *pc2);

status printPC(char *pc);

bool checkPlanetName(Planet *planet, char *planetName);

bool checkPCName(PhysicalCharacteristics *pc, char *pcName);

#endif
