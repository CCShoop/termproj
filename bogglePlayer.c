/*
	Authors (group members): Cael Shoop, Brandon DePalma, Vedant Jani
	Email addresses of group members: cshoop2018@my.fit.edu, bdepalma2017@my.fit.edu, vjani2018@my.fit.edu
	Group name: 2c - "The Cool Can Code Project (CCCP)"
 
	Course: CSE 2010 Data Structures and Algorithms
	Section: 2
 
	Description of the overall algorithm and main data structures:
	We are storing the words in a skip list to provide for faster word searching, and
	organizing them based on string compare return values.
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
#define MAX_WORD 35
#define MIN_LIST 64
#define MAX_LIST 133
 
typedef struct words {
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
void initBogglePlayer(char *wordFile);
void add_word(char word[]);
void add_level(int height);
void sampleWordList(WordList *myWords);
WordList *getWords(char board[4][4]);
 
int main(int argc, char argv[]) {
	initDictionary();
 
	return (0);
}

void initDictionary() { //initializes dictionary skiplist
	dictionary = malloc(sizeof(book));
	dictionary->list = malloc(sizeof(words));
	dictionary->list->up = NULL;
	dictionary->list->prev = NULL;
	dictionary->list->down = NULL;
	dictionary->list->next = malloc(sizeof(words));
	dictionary->list->next->prev = dictionary->list;
	dictionary->list->next->up = NULL;
	dictionary->list->next->next = NULL;
	dictionary->list->next->down = NULL;
}

// initialize BogglePlayer with a file of English words
void initBogglePlayer(char *wordFile) {
 
}

void add_word(char word[]) { //adds an word to the dictionary
	words *temp = dictionary->list->right;
	while (temp->down != NULL) //moves temp node to bottom left
		temp = temp->down;
	words *temp2 = temp->prev;
	int height = rand() % 11; //sets height
	if (dictionary->height <= height)
		add_level(height + 1); //adds a level if the dictionary is not tall enough already
	printf("\n");
	while (temp->up != NULL) {
		temp->prev->next = NULL;
		temp->prev = NULL;
		temp = temp->up;
	}
	temp = temp2->right;
	for (int i = 0; i < height; i++) { //moves temp up to starting height
		temp = temp->up;
	}
	temp2 = temp->left;
	words *prev_new = NULL;
	while (temp->down != NULL) { //works down from top of height to bottom of list, adding nodes
		words *new = malloc(sizeof(words)); //new node
		temp2->next = new; //lots of pointers to insert new node
		new->prev = temp2;
		temp->prev = new;
		new->next = temp;
		new->up = prev_new;
		if (prev_new != NULL)
			prev_new->down = new;
		prev_new = new;
		strcpy(new->word, word);
		temp = temp->down;
		temp2 = temp2->down;
	}
}


void add_level(int height) { //adds levels as required
	for (int i = dictionary->height; i < height; i++) { //loop in case it needs multiple levels added
		dictionary->height = height;
		dictionary->list->up = malloc(sizeof(words));
		dictionary->list->up->down = dictionary->list;
		dictionary->list = dictionary->list->up;
		dictionary->list->up = NULL;
		dictionary->list->prev = NULL;
		dictionary->list->next = malloc(sizeof(words)); //pointers to create double nodes at top for days
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
 
WordList *getWords(char board[4][4]) {
 
	WordList* myWords = getNewWordList();
 
	sampleWordList(myWords);

	return (myWords);
}
//void sampleWordList(WordList *myWords) // a sample function to populate a word list
//	based on the board, find valid words
//
// board: 4x4 board, each element is a letter, 'Q' represents "QU",
//	first dimension is row, second dimension is column
//	ie, board[row][col]
//
// Return at most 20 valid words in UPPERCASE and
//	their paths of locations on the board in myWords
//
// See word.h for details of the struct for Word, WordList, Location, and access functions
 
// a sample list of one word
void sampleWordList(WordList *myWords) {
	setWordListLength(myWords, 1);		// number of words on the list
	Word *word = getWordFromWordList(myWords, 0);	// first word
 
	setWord(word, "cat");			 // word string
 
	setPathLength(word, 3);			 // word path
	setLetterRowAndCol(word, 0, 1, 2);// letter 0 is at row 1 and column 2
	setLetterRowAndCol(word, 1, 1, 3);
	setLetterRowAndCol(word, 2, 2, 3);
}