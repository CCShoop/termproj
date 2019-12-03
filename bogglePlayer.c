/*

  Authors (group members): Brandon DePalma, Vedant Jani, Cael Shoop
  Email addresses of group members: 
  Group name: 2c

  Course:
  Section:

  Description of the overall algorithm and main data structures:


*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "bogglePlayer.h"
#include "word.h"

#include "dictionary.h"

//dictionary pointer
SkipList *dictionary;

// initialize BogglePlayer with a file of English words
void initBogglePlayer(char* word_file) {

	//open file
	FILE *fp = fopen(word_file, "r");
	
	if(!fp){
		printf("file not found\n");
		exit(-1);
	}

	//create head of dictionary
	dictionary = (SkipList*)malloc(sizeof(SkipList));
	dictionary->head = NULL;

	//initialize dictionary
	initDictionary(dictionary);
	
	//input variable
	char input[40], *tok;
	int i;
	int first = 1;
	
	//get words
	while(fgets(input, sizeof(input), fp)){
		
		//remove return character
		tok = strtok(input, "\r");
		
		//if word is of valid length
		if((strlen(tok) >= 3) && (strlen(tok) <= 16)){
			//printf("%s\n", tok);
			i = 0;
			
			//make all letter uppercase
			while(tok[i] != '\0'){
				if(islower(tok[i])){
					tok[i] = toupper(tok[i]);
				}
				i++;
			}
			
			//add word to skip list dictionary
			addWord(dictionary, tok, first);
			
			//after first word is inserted set indicator to 0
			if(first == 1){
			
				first = 0;
		
			}
			
		}
		
		
	}
	
	//close file
	fclose(fp);
	
}

//void      sampleWordList(WordList* myWords);   // a sample function to populate a word list

// based on the board, find valid words
//
// board: 4x4 board, each element is a letter, 'Q' represents "QU",
//    first dimension is row, second dimension is column
//    ie, board[row][col]
//
// Return at most 20 valid words in UPPERCASE and
//    their paths of locations on the board in myWords
//
// See word.h for details of the struct for Word, WordList, Location, and access functions

WordList* getWords(char board[4][4]) {
	
	printf("  0123\n");
    WordList* myWords = getNewWordList(); 
    
    for(int i = 0; i < 4; i++){
    	printf("%d ", i);
    	for(int j = 0; j < 4; j++){
    	
    		printf("%c", board[i][j]);
    	
    	}
    	
    	printf("\n");
    
    }
    
    //FIND THE WORDS
    
    
    
    return myWords;
    
}

/*
// a sample list of one word
void sampleWordList(WordList* myWords)  {
    setWordListLength(myWords, 1);      // number of words on the list 
    Word *word = getWordFromWordList(myWords, 0);   // first word

    setWord(word, "HALO");               // word string

    setPathLength(word, 4);             // word path
    setLetterRowAndCol(word, 0, 1, 2);  // letter 0 is at row 1 and column 2
    setLetterRowAndCol(word, 1, 1, 3);
    setLetterRowAndCol(word, 2, 2, 3);
    setLetterRowAndCol(word, 3, 2, 2);
}
*/