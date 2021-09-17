JerryBoree:HashTable.o JerryBoreeMain.o Jerry.o KeyValuePair.o LinkedList.o MultiValueHashTable.o
	gcc HashTable.o JerryBoreeMain.o Jerry.o KeyValuePair.o LinkedList.o MultiValueHashTable.o -o JerryBoree
HashTable.o: HashTable.c HashTable.h Defs.h LinkedList.h KeyValuePair.h
	gcc -c HashTable.c
JerryBoreeMain.o: JerryBoreeMain.c Jerry.h Defs.h MultiValueHashTable.h LinkedList.h KeyValuePair.h HashTable.h
	gcc -c JerryBoreeMain.c
Jerry.o: Jerry.c Jerry.h Defs.h
	gcc -c Jerry.c
KeyValuePair.o: KeyValuePair.c KeyValuePair.h Defs.h
	gcc -c KeyValuePair.c
LinkedList.o: LinkedList.c LinkedList.h Defs.h
	gcc -c LinkedList.c
MultiValueHashTable.o: MultiValueHashTable.c MultiValueHashTable.h Defs.h LinkedList.h KeyValuePair.h HashTable.h
	gcc -c MultiValueHashTable.c
clean:
	rm -rf *o JerryBoree
