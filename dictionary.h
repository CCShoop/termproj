
#ifndef DICTIONARY_H
#define DICTIONARY_H

#define LIST_HEIGHT 6
#define WORD_LEN 16

#define NEG_INF "@@@@@@@@@@@@@@@@"
#define POS_INF "[[[[[[[[[[[[[[[["

typedef struct skipListNode{
	char word[WORD_LEN + 1];
	bool used;
	struct skipListNode *prev, *next, *above, *below;
}SkipListNode;

typedef struct skipList{
	SkipListNode *head, *tail;
	struct skipList *above, *below;
}SkipList;

//initializes dictionary with bookend inf values
void initDictionary(SkipList *list);

//adds word to the skip list, puts the first word on every level
void addWord(SkipList *list, char *w, int firstW);

//checks if the found word is in the dictionary
bool isValidWord(SkipList *dictionary, char *found_word);

#endif