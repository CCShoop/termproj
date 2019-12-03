

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "dictionary.h"


//skip search function to search through skip list
//pseudo code taken from SkipLists.ppt
SkipListNode *skipSearch(SkipListNode *startPoint, char *key){
    
    //get starting point
    SkipListNode *p = startPoint;
    
    
    while(p->below != NULL){//if there is a list below
        
        p = p->below;
        
        while(strcmp(key, p->next->word) < 0){//checking down the line
            
            p = p->next;
            
        }
        
    }
    
    //returning found node, either correct node or node closest to key that is less than key
    return p;
    
}

//idea taken from doublyLinkedList.c HW5, inserts in a doubly linked list with a head and tail node
void insert(SkipList *l, SkipListNode *n){
	
	SkipList *list = l;
	SkipListNode *new = n;
	
	if(list->head == NULL){ //empty list
		
		list->head = new;
		list->tail = new;
	
	}else if(strcmp(new->word, list->head->word) < 0){ //at beginning
		
		SkipListNode *first = list->head;
		
		new->next = first;
		list->head = new;
		first->prev = new;
	
	}else{ //at middle or end
		
		SkipListNode *currentNode = list->head;
		
		while((currentNode->next != NULL) && (strcmp(new->word, currentNode->next->word) > 0)){
			currentNode = currentNode->next;
		}
		
		if(currentNode->next != NULL){ //in middle
			
			new->prev = currentNode;
			new->next = currentNode->next;
			currentNode->next->prev = new;
		
		}else{ //at end
			
			new->next = NULL;
			new->prev = currentNode;
			list->tail = new;
		
		}
		
		currentNode->next = new;
	
	}

}

//finds node in given level of skip list
SkipListNode *find(SkipList *list, char *key){

	SkipListNode *current = list->head;
	
	//goes to node with given key or closest node
	while((current != NULL) && (strcmp(key, current->word) > 0)){
	
		current = current->next;
	
	}
	
	//checks if node is found
	if((current != NULL) && (strcmp(key, current->word) == 0)){
		
		return current;
		
	}else{
	
		return NULL;
	
	}

}

void initDictionary(SkipList *list){

	SkipList *current = list, *new;
	int i;
	
	//initializes each level of the skip list
	for(i = 1; i < LIST_HEIGHT; i++){
	
		new = (SkipList*)malloc(sizeof(SkipList));
		new->head = NULL;
		current->below = new;
		new->above = current;
		current = current->below;
	
	}
	
	current = list;
	SkipListNode *inf;
	
	//sets up bookend infinite values for the skip list
	for(i = 0; i < LIST_HEIGHT; i++){
		
		inf = (SkipListNode*)malloc(sizeof(SkipListNode));
		strcpy(inf->word, NEG_INF);
		inf->used = false;
		insert(current, inf);
		inf = (SkipListNode*)malloc(sizeof(SkipListNode));
		strcpy(inf->word, POS_INF);
		inf->used = false;
		insert(current, inf);
		
		if(current->below != NULL){
            current = current->below;
        }
	
	}
	
	SkipListNode *prevInf, *prevNeg;
	SkipListNode *currentInf = find(current, POS_INF);
	SkipListNode *currentNeg = find(current, NEG_INF);
	
	//link vertically pos and neg infinity values
	while(current->above != NULL){
	
		current = current->above;
		
		prevInf = currentInf;
		currentInf = find(current, POS_INF);
		prevNeg = currentNeg;
		currentNeg = find(current, NEG_INF);
		
		currentInf->below = prevInf;
		currentNeg->below = prevNeg;
		prevInf->above = currentInf;
		prevNeg->above = currentNeg;
				
	}

}

void addWord(SkipList *listHead, char *w, int firstW){

	SkipList *current = listHead;
	SkipListNode *end, *b, *new;
	
	//goes to bottom of skip list
	while(current->below != NULL){
	
		current = current->below;
	
	}
	
	end = current->tail;
	
	//create new node
	new = (SkipListNode*)malloc(sizeof(SkipListNode));
	strcpy(new->word, w);
	new->used = false;
	
	//insert on bottom level
	new->prev = end->prev;
	new->next = end;
	end->prev->next = new;
	end->prev = new;
	
	b = new;
	
	int height;
	
	//if it is first word, add to each level
	if(firstW == 1){
		height = LIST_HEIGHT - 1;
	}else{
		height = rand() % LIST_HEIGHT;
	}
	
	//add nodes in above lists with respect to random height
	for(int i = 0; i < height; i++){
		
		//move up a list
		current = current->above;
		end = current->tail;
		
		//create new node
		new = (SkipListNode*)malloc(sizeof(SkipListNode));
		strcpy(new->word, w);
		new->used = false;
		
		//insert on level
		new->prev = end->prev;
		new->next = end;
		end->prev->next = new;
		end->prev = new;
		
		//link nodes vertically
		new->below = b;
		b->above = new;
		
		//set below pointer to current for next level
		b = new;
	
	}
	
// 	Each inserted word is assumed to be in alphabetical order to the last.
// 	Therefore we know we can always add to the end of each level.

}

bool isValidWord(SkipList *dictionary, char *foundWord){

	//skip search to find node
	SkipListNode *result = skipSearch(dictionary->head, foundWord);
	
	//if correct node is found
	if(strcmp(result->word, foundWord) == 0){
		
		result->used = true;
		return true;
	
	}else{ //not found
	
		return false;
	
	}

}

