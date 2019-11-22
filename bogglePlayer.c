/*
	Authors (group members): Cael Shoop, Brandon DePalma, Vedant Jani
	Email addresses of group members: cshoop2018@my.fit.edu, bdepalma2017@my.fit.edu, vjani2018@my.fit.edu
	Group name: 2c - "The Cool Can Code Project (CCCP)"
 
	Course: CSE 2010 Data Structures and Algorithms
	Section: 2
 
	Description of the overall algorithm and main data structures:
	We are storing the words in a skip list to provide for faster word searching, and
	organizing them based on ASCII key sum values.
	A DFS search is used to check all possible words on the board and put into a
	second list.
*/
 
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "bogglePlayer.h"
#include "word.h"
define MAX_WORD 35
define MIN_LIST 64
define MAX_LIST 133
 
typedef struct words {
	int key;
	char word[MAX_WORD];
	struct words *up;
	struct words *down;
	struct words *prev;
	struct words *next;
}words;
 
typedef struct book {
	int height;
	words *list;
}book;
 
book *dictionary = NULL;
 
void initDictionary();
void initBogglePlayer(char *word_file);
 
int keyGen(char word[MAX_WORD]);
void sampleWordList(WordList *my_words);
WordList *getWords(char board[4][4]);
 
int main(int argc, char argv[]) {
	srand((unsigned)time(NULL));
	initDictionary();
 
	return (0);
}

void initDictionary() {
	dictionary = malloc(sizeof(book));
	dictionary->list = malloc(sizeof(words));
	dictionary->list->key = MIN_LIST;
	dictionary->list->up = NULL;
	dictionary->list->prev = NULL;
	dictionary->list->down = NULL;
	dictionary->list->next = malloc(sizeof(words));
	dictionary->list->next->key = MAX_LIST;
	dictionary->list->next->prev = dictionary->list;
	dictionary->list->next->up = NULL;
	dictionary->list->next->next = NULL;
	dictionary->list->next->down = NULL;
}

// initialize BogglePlayer with a file of English words
void initBogglePlayer(char *wordFile) {
 
}
 
void add_event(int key, char event[]) { //adds an event to the dictionary
	if (key < 100000)
		printf("AddEvent 0%d %s", key, event); //printing a 0 in front to maintain key length
	else
		printf("AddEvent %d %s", key, event);
	words *temp = dictionary->list;
	while (temp->down != NULL) //moves temp node to bottom left
		temp = temp->down;
	words *temp2 = temp;
	while (temp2->key < key) { //moves temp2 node to node after where new event will go
		temp2 = temp2->next;
		if (temp2->key == key) {
			printf(" ExistingEventError:%s\n", temp2->event_name); //if there is an event at the provided key
			return;
		}
	}
	int height = rand() % 15; //sets height
	if (dictionary->height <= height)
		add_level(height + 1); //adds a level if the dictionary is not tall enough already
	printf("\n");
	if (height >= 0) { //always runs, but keeps code neater and easier to copy paste
		words *new = malloc(sizeof(words)); //new node
		new->up = NULL;
		temp2->prev->next = new; //lots of pointers to insert new node
		new->prev = temp2->prev;
		new->next = temp2;
		temp2->prev = new;
		new->down = NULL;
		new->key = key;
		strcpy(new->event_name, event);
		if (height >= 1) { //moves up in list, inserting nodes at every height applicable
			temp = temp->up;
			temp2 = temp;
			while (temp2->key < key)
				temp2 = temp2->next;
			words *new1 = malloc(sizeof(words));
			new1->up = NULL;
			new1->down = new;
			new->up = new1;
			temp2->prev->next = new1;
			new1->prev = temp2->prev;
			new1->next = temp2;
			temp2->prev = new1;
			new1->key = key;
			strcpy(new1->event_name, event);
			if (height >= 2) { //etc.
				temp = temp->up;
				temp2 = temp;
				while (temp2->key < key)
					temp2 = temp2->next;
				words *new2 = malloc(sizeof(words));
				new2->up = NULL;
				new2->down = new1;
				new1->up = new2;
				temp2->prev->next = new2;
				new2->prev = temp2->prev;
				new2->next = temp2;
				temp2->prev = new2;
				new2->key = key;
				strcpy(new2->event_name, event);
				if (height >= 3) { //etc.
					temp = temp->up;
					temp2 = temp;
					while (temp2->key < key)
						temp2 = temp2->next;
					words *new3 = malloc(sizeof(words));
					new3->up = NULL;
					new3->down = new2;
					new2->up = new3;
					temp2->prev->next = new3;
					new3->prev = temp2->prev;
					new3->next = temp2;
					temp2->prev = new3;
					new3->key = key;
					strcpy(new3->event_name, event);
					if (height >= 4) { //etc.
						temp = temp->up;
						temp2 = temp;
						while (temp2->key < key)
							temp2 = temp2->next;
						words *new4 = malloc(sizeof(words));
						new4->up = NULL;
						new4->down = new3;
						new3->up = new4;
						temp2->prev->next = new4;
						new4->prev = temp2->prev;
						new4->next = temp2;
						temp2->prev = new4;
						new4->key = key;
						strcpy(new4->event_name, event);
						if (height >= 5) { //etc.
							temp = temp->up;
							temp2 = temp;
							while (temp2->key < key)
								temp2 = temp2->next;
							words *new5 = malloc(sizeof(words));
							new5->up = NULL;
							new5->down = new4;
							new4->up = new5;
							temp2->prev->next = new5;
							new5->prev = temp2->prev;
							new5->next = temp2;
							temp2->prev = new5;
							new5->key = key;
							strcpy(new5->event_name, event);
                        	if (height >= 6) { //etc.
								temp = temp->up;
								temp2 = temp;
								while (temp2->key < key)
									temp2 = temp2->next;
								words *new6 = malloc(sizeof(words));
								new6->up = NULL;
								new6->down = new5;
								new5->up = new6;
								temp2->prev->next = new6;
								new6->prev = temp2->prev;
								new6->next = temp2;
								temp2->prev = new6;
								new6->key = key;
								strcpy(new6->event_name, event);
                	           	if (height >= 7) { //etc.
									temp = temp->up;
									temp2 = temp;
									while (temp2->key < key)
										temp2 = temp2->next;
									words *new7 = malloc(sizeof(words));
									new7->up = NULL;
									new7->down = new6;
									new6->up = new7;
									temp2->prev->next = new7;
									new7->prev = temp2->prev;
									new7->next = temp2;
									temp2->prev = new7;
									new7->key = key;
									strcpy(new7->event_name, event);
									if (height >= 8) { //etc.
										temp = temp->up;
										temp2 = temp;
										while (temp2->key < key)
											temp2 = temp2->next;
										words *new8 = malloc(sizeof(words));
										new8->up = NULL;
										new8->down = new7;
										new7->up = new8;
										temp2->prev->next = new8;
										new8->prev = temp2->prev;
										new8->next = temp2;
										temp2->prev = new8;
										new8->key = key;
										strcpy(new8->event_name, event);
										if (height >= 9) { //etc.
											temp = temp->up;
											temp2 = temp;
											while (temp2->key < key)
												temp2 = temp2->next;
											words *new9 = malloc(sizeof(words));
											new9->up = NULL;
											new9->down = new8;
											new8->up = new9;
											temp2->prev->next = new9;
											new9->prev = temp2->prev;
											new9->next = temp2;
											temp2->prev = new9;
											new9->key = key;
											strcpy(new9->event_name, event);
											if (height >= 10) { //etc.
												temp = temp->up;
												temp2 = temp;
												while (temp2->key < key)
													temp2 = temp2->next;
												words *new10 = malloc(sizeof(words));
												new10->up = NULL;
												new10->down = new9;
												new9->up = new10;
												temp2->prev->next = new10;
												new10->prev = temp2->prev;
												new10->next = temp2;
												temp2->prev = new10;
												new10->key = key;
												strcpy(new10->event_name, event);
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}
 
 
void add_level(int height) { //adds levels as required
	for (int i = dictionary->height; i < height; i++) { //loop in case it needs multiple levels added
		dictionary->height = height;
		dictionary->list->up = malloc(sizeof(words));
		dictionary->list->up->key = MIN_LIST;
		dictionary->list->up->down = dictionary->list;
		dictionary->list = dictionary->list->up;
		dictionary->list->up = NULL;
		dictionary->list->prev = NULL;
		dictionary->list->next = malloc(sizeof(words)); //pointers to create double nodes at top for days
		dictionary->list->next->key = MAX_LIST;
		dictionary->list->next->prev = dictionary->list;
		dictionary->list->next->up = NULL;
		dictionary->list->next->next = NULL;
		words *temp = dictionary->list;
		temp = temp->down;
		while (temp->next != NULL)
			temp = temp->next;
		dictionary->list->next->down = temp;
		temp->up = dictionary->list->next;
	}
	if (height > dictionary->height)
		dictionary->height = height;
}
 
 
int keyGen(char word[35]) {
	int key = 0;
	for (int i = 0; word[i] != "\0"; i++) {
		key += word[i];
	}
	return (key);
}
 
WordList *getWords(char board[4][4]) {
 
	WordList* myWords = getNewWordList();
 
	sampleWordList(myWords);

	return (myWords);
}
/*
void sampleWordList(WordList *myWords) // a sample function to populate a word list based
on the board, find valid words

board: 4x4 board, each element is a letter, 'Q' represents "QU", first dimension is row,
second dimension is column; ie, board[row][col]

Return at most 20 valid words in UPPERCASE and their paths of locations on the board in
myWords

See word.h for details of the struct for Word, WordList, Location, and access functions
*/
 
//A sample list of one word
void sampleWordList(WordList *myWords) {
	setWordListLength(myWords, 1);		// number of words on the list
	Word *word = getWordFromWordList(myWords, 0);	// first word
 
	setWord(word, "cat");			 	// word string
 
	setPathLength(word, 3);				// word path
	setLetterRowAndCol(word, 0, 1, 2);	// letter 0 is at row 1 and column 2
	setLetterRowAndCol(word, 1, 1, 3);
	setLetterRowAndCol(word, 2, 2, 3);
}