#include "Jerry.h"
#include "MultiValueHashTable.h"


int main(int argc, char **argv) {

    //check arguments
    if(argc!=3){
        printf("MORTY GIVE ME NORMAL ARGUMENTS \n");
        return -1;
    }

    //data structures used by program
    LinkedList *allJerries = createLinkedList((CopyFunction) returnJerry, (FreeFunction) deleteJerry,
                                              (PrintFunction) printJerry, (EqualFunction) equalJerry, NULL);
    LinkedList *allPlanets = createLinkedList((CopyFunction) returnPlanet, (FreeFunction) deletePlanet,
                                              (PrintFunction) printPlanet, (EqualFunction) equalPlanet,
                                              (EqualFunction) checkPlanetName);
    LinkedList *allOrigins = createLinkedList((CopyFunction) returnOrigin, (FreeFunction) deleteOrigin,
                                              (PrintFunction) printOrigin, (EqualFunction) equalOrigin, NULL);
    struct hashTable_s *hashJerriesID;
    struct MultiValueHashTable_s *multiHashPCs;

    //flags and iterators used in program
    int exit = 0;
    int i;
    int j;
    Node *temp;
    //open configuration file
    FILE *file = fopen(argv[2], "r");
    //if wasn't opened
    if (!file) {
        printf("MORTY I CAN'T OPEN THE FILE \n");
        return -1;
    }
    //line gets each line of the configuration file
    char line[128];
    //flags for proper working of reading the configuration file
    int planetsFileFlag = 0;
    int jerriesFileFlag = 0;
    int fileEndFlag = 0;
    //get first line
    fgets(line, 128, file);
    while (true) {
        //if line is planets then
        if (strcmp("Planets\n", line) == 0) {
            //now we are on planets
            planetsFileFlag = 1;
            //get next line and if it's empty stop reading
            if (!fgets(line, 128, file)) {
                planetsFileFlag = 0;
                jerriesFileFlag = 0;
                fileEndFlag = 1;
            }
            continue;
        }
        //if line is jerry then
        if (strcmp("Jerries\n", line) == 0) {
            //stop with planets, now we are on jerries
            jerriesFileFlag = 1;
            planetsFileFlag = 0;
            //get next line and if it's empty stop reading
            if (!fgets(line, 128, file)) {
                fileEndFlag = 1;
            }
            continue;
        }
        //if we are on planets then
        if (planetsFileFlag == 1) {
            //get info about planet
            char *planetName = strtok(line, ",");
            double x = strtod(strtok(NULL, ","), NULL);
            double y = strtod(strtok(NULL, ","), NULL);
            double z = strtod((strtok(strtok(NULL, ","), "\n")), NULL);
            //create newPlanet
            Planet *newPlanet = createPlanet(planetName, x, y, z);
            //add it to linked list of all planets in program
            appendNode(allPlanets, newPlanet);
            //get next line and if it's empty stop reading
            if (!fgets(line, 128, file)) {
                planetsFileFlag = 0;
                jerriesFileFlag = 0;
                fileEndFlag = 1;
            }
            continue;
        }
        //if we are on planets then
        if (jerriesFileFlag == 1) {
            //get info about jerry
            char *id = strtok(line, ",");
            char *dimensionName = strtok(NULL, ",");
            char *planet = strtok(NULL, ",");
            int happiness = atoi((strtok(strtok(NULL, ","), "\n")));
            Origin *newOrigin = NULL;
            //find pointer for given planet for jerry
            for (i = 0; i < getLengthList(allPlanets); i++) {
                Planet *planetToCheck = getDataByIndex(allPlanets, i);
                if (strcmp(planetToCheck->name, planet) == 0) {
                    //create newOrigin
                    newOrigin = createOrigin(getDataByIndex(allPlanets, i), dimensionName);
                    //add it to array of all origins in program
                    appendNode(allOrigins, newOrigin);
                }
            }
            //if we did not found planet for Jerry then configuration file is incorrect
            if (newOrigin == NULL) {
                printf("RICK WHERES THE PLANET? \n");
                return -1;
            }
            //create newJerry
            Jerry *newJerry = createJerry(id, happiness, newOrigin);
            //add it to linked list of all jerries in program
            appendNode(allJerries, newJerry);
            //get next line and if it's empty stop reading
            if (!fgets(line, 128, file)) {
                planetsFileFlag = 0;
                jerriesFileFlag = 0;
                fileEndFlag = 1;
                break;
            }
            //now iterate through all physicalCharacteristics of new Jerry
            while (line[0] == '\t') {
                //get info about physicalCharacteristic
                char *PCName = strtok(line, ":");
                double PCValue = strtod((strtok(strtok(NULL, ":"), "\n")), NULL);
                PCName = strtok(PCName, "\t");
                //create newPC
                PhysicalCharacteristics *newPC = createPC(PCName, PCValue);
                //add newPC to jerry
                addPC(newJerry, newPC);
                //get next line and if it's empty stop reading
                if (!fgets(line, 128, file)) {
                    planetsFileFlag = 0;
                    jerriesFileFlag = 0;
                    fileEndFlag = 1;
                    break;
                }
            }
        }
        //if we stopped reading we can exit from loop
        if (fileEndFlag == 1) {
            break;
        }
    }

    //close the configuration file, if can't close then free all memory allocated and return -1
    //also if configuration file does not match number of planets or jerries then free all memory allocated and return -1

    if (fclose(file) || atoi(argv[1]) != getLengthList(allPlanets)) {

        printf("RICK SOMETHING WRONG WITH INFO YOU GAVE ME! \n");
        destroyList(allPlanets);
        destroyList(allJerries);
        destroyList(allOrigins);
        return -1;
    }

    // create hash for jerries
    // Key: jerryID, type:string (copied in Hash)
    // Value: jerry, type:jerry (not copied in Hash, only pointer stored)
    // size of hash as number of jerries in program * 3 (for θ(1) because we can add Jerries to program)
    hashJerriesID = createHashTable((CopyFunction) copyJerryID, (FreeFunction) freeJerryID,
                                    (PrintFunction) printJerryID, (CopyFunction) returnJerry,
                                    (FreeFunction) freeJerry,
                                    (PrintFunction) printJerry, (EqualFunction) equalsID,
                                    (TransformIntoNumberFunction) transformStringIntoInt,
                                    getLengthList(allJerries) * 3);

    //count number of physical characteristics in program (with repetitions)
    int numOfPCsInProgram = 0;
    //iterate through all jerries
    temp = getHead(allJerries);
    for (i = 0; i < getLengthList(allJerries); ++i) {
        Jerry *tempJerry = getData(temp);
        //add number of his characteristics to numOfPCsInProgram
        numOfPCsInProgram += tempJerry->characteristicsNum;
        //add jerry to hashJerriesID with key its id, and value jerry itself
        addToHashTable(hashJerriesID, tempJerry->ID, tempJerry);
        temp = getNext(temp);
    }

    // create hash for physicalCharacteristics
    // Key: name of physicalCharacteristic, type:string (copied in Hash)
    // Value: jerry, type:jerry (not copied in Hash, only pointer stored)
    // size of hash as number of physical characteristics in program
    // (because with repetitions, then θ(1) average, because we can add new Physical characteristics)
    multiHashPCs = createMultiValueHashTable((CopyFunction) copyPC,
                                             (FreeFunction) freePC,
                                             (PrintFunction) printPC,
                                             (CopyFunction) returnJerry,
                                             (FreeFunction) freeJerry,
                                             (PrintFunction) printJerry,
                                             (EqualFunction) equalPC,
                                             (EqualFunction) equalJerry,
                                             (TransformIntoNumberFunction) transformStringIntoInt,
                                             numOfPCsInProgram);
    //iterate through  all jerries
    temp = getHead(allJerries);
    for (i = 0; i < getLengthList(allJerries); ++i) {
        Jerry *tempJerry = getData(temp);
        //iterate through its physicalCharacteristics
        for (j = 0; j < tempJerry->characteristicsNum; ++j) {
            //add jerry to addToMultiValueHashTable with key its physicalCharacteristic, and value jerry itself
            addToMultiValueHashTable(multiHashPCs, tempJerry->phc[j]->name, tempJerry);
        }
        temp = getNext(temp);
    }


    /////Start main menu////
    while (exit == 0) {
        printf("Welcome Rick, what are your Jerry's needs today ? \n");
        printf("1 : Take this Jerry away from me \n");
        printf("2 : I think I remember something about my Jerry \n");
        printf("3 : Oh wait. That can't be right \n");
        printf("4 : I guess I will take back my Jerry now \n");
        printf("5 : I can't find my Jerry. Just give me a similar one \n");
        printf("6 : I lost a bet. Give me your saddest Jerry \n");
        printf("7 : Show me what you got \n");
        printf("8 : Let the Jerries play \n");
        printf("9 : I had enough. Close this place \n");
        char input[2];
        scanf("%s", input);

        ////Take this Jerry away from me ////
        if (strcmp(input, "1") == 0) {
            printf("What is your Jerry's ID ? \n");
            char id[STRING_LIMIT];
            scanf("%s", id);

            if (getValue(lookupInHashTable(hashJerriesID, id)) == NULL) {
                printf("What planet is your Jerry from ? \n");
                char planetString[STRING_LIMIT];
                scanf("%s", planetString);
                Planet *planetOfJerry = searchByKeyInList(allPlanets, planetString);

                if (planetOfJerry != NULL) {
                    char dimensionString[STRING_LIMIT];
                    printf("What is your Jerry's dimension ? \n");
                    scanf("%s", dimensionString);

                    int happiness = 0;
                    printf("How happy is your Jerry now ? \n");
                    scanf("%d", &happiness);

                    Origin *newOrigin = createOrigin(planetOfJerry, dimensionString);
                    appendNode(allOrigins, newOrigin);

                    Jerry *newJerry = createJerry(id, happiness, newOrigin);
                    appendNode(allJerries, newJerry);
                    addToHashTable(hashJerriesID, id, newJerry);
                    printJerry(newJerry);

                } else {
                    printf("%s is not a known planet ! \n", planetString);
                }

            } else {
                printf("Rick did you forgot ? you already left him here ! \n");
            }

            //// I think I remember something about my Jerry////
        } else if (strcmp(input, "2") == 0) {
            //get id of Jerry to add physical characteristic
            printf("What is your Jerry's ID ? \n");
            char id[STRING_LIMIT];
            scanf("%s", id);
            //find jerry by its id in hash in θ(1)
            Jerry *jerryToAddPC = getValue(lookupInHashTable(hashJerriesID, id));
            //if found
            if (jerryToAddPC != NULL) {
                //get physical characteristic to add
                printf("What physical characteristic can you add to Jerry - %s ? \n", id);
                char pcToAdd[STRING_LIMIT];
                scanf("%s", pcToAdd);
                //if jerry doesn't have given physical characteristic
                if (!checkIfHasPC(jerryToAddPC, pcToAdd)) {
                    //get value of physical characteristic
                    printf("What is the value of his %s ? \n", pcToAdd);
                    double valueOfPC = 0;
                    scanf("%lf", &valueOfPC);
                    //create newPC
                    PhysicalCharacteristics *newPC = createPC(pcToAdd, valueOfPC);
                    //add newPC to jerry
                    addPC(jerryToAddPC, newPC);
                    //add jerry to multiHashPCs by physical characteristic's name
                    addToMultiValueHashTable(multiHashPCs, pcToAdd, jerryToAddPC);
                    //display all jerries with given physical characteristic
                    displayMultiValueHashElementsByKey(multiHashPCs, pcToAdd);
                }
                    //if jerry already has given physical characteristic
                else {
                    printf("The information about his %s already available to the daycare ! \n", pcToAdd);
                }
            }
                //if jerry is not stored in Boree
            else {
                printf("Rick this Jerry is not in the daycare ! \n");
            }

            ////Oh wait. That can't be right////
        } else if (strcmp(input, "3") == 0) {
            //get id of Jerry to delete physical characteristic
            printf("What is your Jerry's ID ? \n");
            char id[STRING_LIMIT];
            scanf("%s", id);
            //find jerry by its id in hash in θ(1)
            Jerry *jerryToDeletePC = getValue(lookupInHashTable(hashJerriesID, id));
            //if found
            if (jerryToDeletePC != NULL) {
                //get value of physical characteristic to delete
                printf("What physical characteristic do you want to remove from Jerry - %s ? \n", id);
                char pcToDelete[STRING_LIMIT];
                scanf("%s", pcToDelete);
                //if jerry has given physical characteristic
                if (checkIfHasPC(jerryToDeletePC, pcToDelete)) {
                    //delete jerry from multiHashPCs by physical characteristic's name
                    removeFromMultiValueHashTable(multiHashPCs, pcToDelete, jerryToDeletePC);
                    //delete physical characteristic from jerry himself
                    deletePCfromJerry(jerryToDeletePC, pcToDelete);
                    //print jerry without deleted physical characteristic
                    printJerry(jerryToDeletePC);


                }
                    //if jerry does not have given physical characteristic
                else {
                    printf("The information about his %s not available to the daycare ! \n", pcToDelete);
                }
            }
                //if jerry is not stored in Boree
            else {
                printf("Rick this Jerry is not in the daycare ! \n");
            }

            ////I guess I will take back my Jerry now////
        } else if (strcmp(input, "4") == 0) {
            //get id of Jerry to take from Boree
            printf("What is your Jerry's ID ? \n");
            char id[STRING_LIMIT];
            scanf("%s", id);
            //find jerry by its id in hash in θ(1)
            Jerry *jerryToDelete = getValue(lookupInHashTable(hashJerriesID, id));
            //if found
            if (jerryToDelete != NULL) {
                //iterate through all physical characteristics of given jerry
                for (i = 0; i < jerryToDelete->characteristicsNum; ++i) {
                    //delete jerry from multiHashPCs by physical characteristic's name
                    removeFromMultiValueHashTable(multiHashPCs, jerryToDelete->phc[i]->name, jerryToDelete);
                }
                //delete jerry from hashJerriesID by his id
                removeFromHashTable(hashJerriesID, jerryToDelete->ID);
                //delete jerry from list of all jerries in program
                deleteNode(allJerries, jerryToDelete);
                printf("Rick thank you for using our daycare service ! Your Jerry awaits ! \n");
            }
                //if jerry is not stored in Boree
            else {
                printf("Rick this Jerry is not in the daycare ! \n");
            }

            //// I can't find my Jerry. Just give me a similar one////
        } else if (strcmp(input, "5") == 0) {
            //get physical characteristic of Jerry to search
            printf("What do you remember about your Jerry ? \n");
            char pc[STRING_LIMIT];
            scanf("%s", pc);
            //get list of all jerries with given physical characteristic
            LinkedList *listOfPC = lookupInMultiValueHashTable(multiHashPCs, pc);
            //if there are jerries
            if (listOfPC != NULL) {
                //get value of his physical characteristic
                printf("What do you remember about the value of his %s ? \n", pc);
                double valueOfPC = -1;
                //value of physical characteristic must be not negative for proper working
                while (valueOfPC < 0) {
                    scanf("%lf", &valueOfPC);
                }
                //make arrayOfDiffs storing differences of each jerry with given physical characteristic and given value
                double *arrayOfDiffs = malloc(sizeof(double) * getLengthList(listOfPC));
                //iterate through all jerries with physical characteristic
                temp = getHead(listOfPC);
                for (i = 0; i < getLengthList(listOfPC); ++i) {
                    Jerry *currJerry = getData(temp);
                    //add difference between given value and value of given physical characteristic of current jerry
                    for (j = 0; j < currJerry->characteristicsNum; ++j) {
                        if (checkPCName(currJerry->phc[j], pc)) {
                            arrayOfDiffs[i] = diff(currJerry->phc[j]->value, valueOfPC);
                            break;
                        }
                    }
                    temp = getNext(temp);
                }

                //find minimum in arrayOfDiffs
                double min = 2147483647; //max int
                int indexOfMin = 0;
                for (int k = 0; k < getLengthList(listOfPC); ++k) {
                    if (arrayOfDiffs[k] < min) {
                        min = arrayOfDiffs[k];
                        indexOfMin = k;
                    }
                }
                //free memory allocated for arrayOfDiffs
                free(arrayOfDiffs);
                //find jerry with minimum difference
                Jerry *jerryToDelete = getDataByIndex(listOfPC, indexOfMin);
                //print jerry
                printf("Rick this is the most suitable Jerry we found : \n");
                printJerry(jerryToDelete);
                //iterate through all physical characteristics of given jerry
                for (i = 0; i < jerryToDelete->characteristicsNum; ++i) {
                    //delete jerry from multiHashPCs by physical characteristic's name
                    removeFromMultiValueHashTable(multiHashPCs, jerryToDelete->phc[i]->name, jerryToDelete);
                }
                //delete jerry from hashJerriesID by his id
                removeFromHashTable(hashJerriesID, jerryToDelete->ID);
                //delete jerry from list of all jerries in program
                deleteNode(allJerries, jerryToDelete);
                printf("Rick thank you for using our daycare service ! Your Jerry awaits ! \n");
            }
                //if there are not jerries with given physical characteristic
            else {
                printf("Rick we can not help you - we do not know any Jerry's %s ! \n", pc);
            }

            //// I lost a bet. Give me your saddest Jerry////
        } else if (strcmp(input, "6") == 0) {
            //check if there are jerries in boree
            if (getLengthList(allJerries) > 0) {
                int min = 101;
                int indexOfSaddest = 0;
                //iterate through all jerries and find saddest :(
                temp = getHead(allJerries);
                for (i = 0; i < getLengthList(allJerries); ++i) {
                    Jerry *currJerry = getData(temp);
                    if (currJerry->happiness < min) {
                        min = currJerry->happiness;
                        indexOfSaddest = i;
                    }
                    temp = getNext(temp);
                }
                //get saddest jerry :(
                Jerry *jerryToDelete = getDataByIndex(allJerries, indexOfSaddest);
                //print saddest jerry :(
                printf("Rick this is the most suitable Jerry we found : \n");
                printJerry(jerryToDelete);
                //iterate through all physical characteristics of given jerry
                for (i = 0; i < jerryToDelete->characteristicsNum; ++i) {
                    //delete jerry from multiHashPCs by physical characteristic's name
                    removeFromMultiValueHashTable(multiHashPCs, jerryToDelete->phc[i]->name, jerryToDelete);
                }
                //delete jerry from hashJerriesID by his id
                removeFromHashTable(hashJerriesID, jerryToDelete->ID);
                //delete jerry from list of all jerries in program
                deleteNode(allJerries, jerryToDelete);
                printf("Rick thank you for using our daycare service ! Your Jerry awaits ! \n");
            }
                //if there are not jerries in Boree
            else {
                printf("Rick we can not help you - we currently have no Jerries in the daycare ! \n");
            }

            ////Show me what you got////
        } else if (strcmp(input, "7") == 0) {
            printf("What information do you want to know ? \n");
            printf("1 : All Jerries \n");
            printf("2 : All Jerries by physical characteristics \n");
            printf("3 : All known planets \n");
            scanf("%s", input);

            ////All Jerries////
            if (strcmp(input, "1") == 0) {
                //if there are jerries in Boree
                if (getLengthList(allJerries) > 0) {
                    //display list of all jerries in program
                    displayList(allJerries);
                }
                    //if there are not jerries in Boree
                else {
                    printf("Rick we can not help you - we currently have no Jerries in the daycare ! \n");
                }

                ////All Jerries by physical characteristics////
            } else if (strcmp(input, "2") == 0) {
                //get name of physical characteristic
                printf("What physical characteristics ? \n");
                char pc[STRING_LIMIT];
                scanf("%s", pc);
                //get list of all jerries with given physical characteristic
                LinkedList *listOfPC = lookupInMultiValueHashTable(multiHashPCs, pc);
                //if there are jerries with given physical characteristic
                if (listOfPC != NULL) {
                    //print physical characteristic
                    printf("%s : \n", pc);
                    //print list of all jerries with given physical characteristic
                    displayList(listOfPC);
                }
                    //if there are not jerries with given physical characteristic
                else {
                    printf("Rick we can not help you - we do not know any Jerry's %s ! \n", pc);
                }
                ////All known planets////
            } else if (strcmp(input, "3") == 0) {
                //print list of all planets in program
                displayList(allPlanets);
            }
                ////Incorrect input////
            else {
                printf("Rick this option is not known to the daycare ! \n");
            }

        }
            ////Let the Jerries play////
        else if (strcmp(input, "8") == 0) {
            //if there are jerries in Boree
            if (getLengthList(allJerries) > 0) {
                printf("What activity do you want the Jerries to partake in ? \n");
                printf("1 : Interact with fake Beth \n");
                printf("2 : Play golf \n");
                printf("3 : Adjust the picture settings on the TV \n");
                scanf("%s", input);
                ////Interact with fake Beth////
                if (strcmp(input, "1") == 0) {
                    //iterate through all jerries and update their happiness
                    temp = getHead(allJerries);
                    for (i = 0; i < getLengthList(allJerries); ++i) {
                        Jerry *currJerry = getData(temp);
                        if (currJerry->happiness > 19) {
                            currJerry->happiness += 15;
                        } else {
                            currJerry->happiness -= 5;
                        }
                        //if happiness is not in allowed range
                        if (currJerry->happiness > 100) {
                            currJerry->happiness = 100;
                        }
                        //if happiness is not in allowed range
                        if (currJerry->happiness < 0) {
                            currJerry->happiness = 0;
                        }
                        temp = getNext(temp);
                    }
                    printf("The activity is now over ! \n");
                    //display all jerries with updated happiness
                    displayList(allJerries);
                    ////Play golf////
                } else if (strcmp(input, "2") == 0) {
                    //iterate through all jerries and update their happiness
                    temp = getHead(allJerries);
                    for (i = 0; i < getLengthList(allJerries); ++i) {
                        Jerry *currJerry = getData(temp);
                        if (currJerry->happiness > 49) {
                            currJerry->happiness += 10;
                        } else {
                            currJerry->happiness -= 10;
                        }
                        //if happiness is not in allowed range
                        if (currJerry->happiness > 100) {
                            currJerry->happiness = 100;
                        }
                        //if happiness is not in allowed range
                        if (currJerry->happiness < 0) {
                            currJerry->happiness = 0;
                        }
                        temp = getNext(temp);
                    }
                    printf("The activity is now over ! \n");
                    //display all jerries with updated happiness
                    displayList(allJerries);
                    ////Adjust the picture settings on the TV////
                } else if (strcmp(input, "3") == 0) {
                    //iterate through all jerries and update their happiness
                    temp = getHead(allJerries);
                    for (i = 0; i < getLengthList(allJerries); ++i) {
                        Jerry *currJerry = getData(temp);
                        currJerry->happiness += 20;
                        //if happiness is not in allowed range
                        if (currJerry->happiness > 100) {
                            currJerry->happiness = 100;
                        }
                        temp = getNext(temp);
                    }
                    printf("The activity is now over ! \n");
                    //display all jerries with updated happiness
                    displayList(allJerries);
                }
                    ////Incorrect Input////
                else {
                    printf("Rick this option is not known to the daycare ! \n");
                }
            }
                //if there are not jerries in Boree
            else {
                printf("Rick we can not help you - we currently have no Jerries in the daycare ! \n");
            }

        }
            ////I had enough. Close this place////
        else if (strcmp(input, "9") == 0) {
            printf("The daycare is now clean and close ! \n");
            //free memory allocated for multiHashPCs
            destroyMultiValueHashTable(multiHashPCs);
            //free memory allocated for hashJerriesID
            destroyHashTable(hashJerriesID);
            //free memory allocated for lists
            destroyList(allPlanets);
            destroyList(allJerries);
            destroyList(allOrigins);
            exit = 1;
        }
            ////If input is incorrect////
        else {
            printf("Rick this option is not known to the daycare ! \n");
        }
    }
    return 0;
}
