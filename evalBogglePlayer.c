/*

  Author: Jesse Torres
  Email: jtorres2015@my.fit.edu
  Pseudocode: Philip Chan

  Usage: evalBogglePlayer wordFile [seed]

  Input:
  wordFile has valid words, one on each line
  seed is for generating different boards [optional]

  Description:

  The goal is to evaluate BogglePlayer
  Validity and points for words are in the assignment.

  The performance of BogglePlayer is measured by:

  a.  totalPoints: total points of found words
  b.  speed: time in second for finding words
  c.  space consumption: memory consumption
  d.  overall score--(totalPoints^2)/sqrt(time * memory)  


  --------Pseudocode for evaluating BogglePlayer---------------

     initBogglePlayer(wordFile) // a list of English words

     board = randomly generate a board
     guessedWordList = getWords(board)
     check validity of wordList according to the four rules in the assignment
     calculate points as in the assignment

     report performance
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <ctype.h>
#include "evalBogglePlayer.h"
#include "bogglePlayer.h"
#include "word.h"


void  preprocessingInBogglePlayer(char* wordFile);
int calculate_points(WordList* words, char board[4][4]);
int checkForWordValidity(Word* word, char board[4][4]);
int squareDistance(Location l1, Location l2);
float getAccuTimeOfBogglePlayer(char *word,
                 clock_t *totalCpuTimeUsed,
                 int *totalNumGuesses);
void  reportPerformance(float accuracySum, clock_t totalCpuTimeUsed,
            int totalNumWords, int totalNumGuesses);
long  getPeakMemory();

char* dictionary[234941];
char boggle_dices[16][7] = {"AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS", "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
                            "DISTTY", "EEGHNW", "EEINSU", "EHRTVW", "EIOSST", "ELRTTY", "HIMNUQ", "HLNNRZ"};

int main (int argc, char* argv[]) {
    if (argc < 2 || argc > 3) {
            fprintf(stderr, "Usage: evalBogglePlayer wordFile [seed]\n");
            exit(-1);
    }

    // Read the dictionary, and convert to uppercase
	FILE* dictFile = fopen(argv[1], "r");
	if (dictFile) {
		for (int i = 0; i < 234941; i++) {
        dictionary[i] = malloc(25);
        fscanf(dictFile, "%s", dictionary[i]);
        int j = 0;
        while (dictionary[i][j]) {
          dictionary[i][j] = toupper(dictionary[i][j]);
          j++;
        }
    }
	} else {
		fprintf(stderr, "Dictionary file could not be opened, exiting program\n");
		exit(-1);
	}

    preprocessingInBogglePlayer(argv[1]);

    // Default seed if second argument is not passed
    int seed = 123456789;
	if (argc == 3) {
        seed = atoi(argv[2]);
    }

    printf("Playing Boggle...\n");

    srand(seed);

    // Generate random board, and pass it to bogglePlayer to play Boggle
    char board[4][4];
    int length = 16;

    // Create random board
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            int dice_index = rand() % length;
            char* dice = boggle_dices[dice_index];
            board[i][j] = dice[rand() % 6];
            strcpy(boggle_dices[dice_index], boggle_dices[length - 1]);
            strcpy(boggle_dices[length - 1], dice);
            length--;
        }
    }

    // Calculate the time taken to find the words on the board
    int startTime = clock();
    // Play the game of Boggle and find the words
    WordList* guessedWords = getWords(board);
    int endTime = clock();

        // Calculate the used memory
    int memory = getPeakMemory();

    double totalElapsedTime = endTime - startTime;

    // Convert elapsed time into seconds, and calculate the average time
    totalElapsedTime = totalElapsedTime / CLOCKS_PER_SEC;
    if (totalElapsedTime > 180) {  // longer than 3 minutes
        printf("*** getWords() exceeded 3 minutes ***\n");
        exit(-1);
    }
    if (totalElapsedTime <= 0) // too small to measure, unlikely
       totalElapsedTime = 1.0 / CLOCKS_PER_SEC;
    if (memory <= 0) // too small to measure, highly unlikely
       memory = 1;


    // Calculate points for the words found
    int total_points = calculate_points(guessedWords, board);

    // Output performance metrics
    printf("Points: %d\n", total_points);
    printf("Time in seconds: %.4e\n", totalElapsedTime);
    printf("Used memory in bytes: %d\n", memory);
    printf("Overall performance: %.4lf\n",
               (total_points * total_points) / sqrt(totalElapsedTime * memory));
}

// call initBogglePlayer in bogglePlayer
// report time and memory for preprocessing
void preprocessingInBogglePlayer(char* wordFile) {
    printf("Preprocessing in bogglePlayer...\n");
        clock_t startTime, endTime;
        startTime = clock();
        initBogglePlayer(wordFile);
        endTime = clock();

	double initTime = ((double) (endTime - startTime)) / CLOCKS_PER_SEC;
        printf("cpu time in seconds (not part of score): %.4e\n", initTime);
        printf("memory in bytes (not part of score): %ld\n",
                getPeakMemory());
	if (initTime > 600)
	  {
	    printf("**** Preprocessing time is too long ****\n");
	    exit(-1);
	  }
}

// Calculates the points for the words found on the board
int calculate_points(WordList* words, char board[4][4]) {
  int points = 0;

  if (words == NULL)
    {
      printf("your word list is null\n");
      return 0;
    }

  int wordListLen = words->length;
  if (wordListLen < 0)
    {
      printf("Your word list has negative length: %d\n", words->length);
      return 0;  
    }
  else if (wordListLen > 20) 
    {
      wordListLen = 20;              // only the first 20 are counted
      points -= (wordListLen - 20);  // Penalty if more than 20 words were returned
    }
	  

  // Calculate points for the first 20 words or fewer
  for (int index = 0; index < wordListLen; index++) {
    Word* w = &(words->wordlist[index]);
    if (w->word) {
      // Check if the word is unique
      bool duplicate = false;
      for (int i = 0; i < index; i++) {
	if (strcmp(w->word, words->wordlist[i].word) == 0) {
	  duplicate = true;
	  break;
	}
      }

      // If the word is duplicate then give penalty, else check if word is valid
      if (duplicate) {
	points -= (w->path_length - 2) * (w->path_length - 2);
      } else {
	points += checkForWordValidity(w, board);
      }
    } else {
      points -= 1;
    }
  }

  if (points < 0)
    points = 0;

  return points;
}

// Checks if the word is valid and assigns positive points for valid word and negative points for invalid word
int checkForWordValidity(Word* word, char board[4][4]) {
	int length = strlen(word->word);

	// Check if path length is same as word length
	int numberOfQs = 0;
	for (int i = 0; i < length; i++) {
		if (word->word[i] == 'Q') {
			numberOfQs++;
		}
	}
	if (length != word->path_length + numberOfQs) {
		return -((length - 2) * (length - 2));
	}

	// Check if word has at least 3 letters
	if (length < 3) {
		return -1;
	}

	// Check if the letters of the word are adjacent on the board
	for (int i = 1; i < word->path_length; i++) {
	    if (squareDistance(word->path[i - 1], word->path[i]) > 2) {
			return -((length - 2) * (length - 2));
		}
	}

	// Check if each letter on the board is used at most once and if letters are on the board or not
	bool used[4][4];
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      used[i][j] = false;
    }
  }

	for (int i = 0, letter_index = 0; i < word->path_length; i++, letter_index++) {
		int row = word->path[i].row;
		int col = word->path[i].column;

		if (used[row][col] || board[row][col] != word->word[letter_index]) {
			return -((length - 2) * (length - 2));
		} else {
			used[row][col] = true;
		}
		if (word->word[letter_index] == 'Q') {
			letter_index++;
		}
	}

	// Check if word exists in the dictionary
	bool in_dictionary = false;
	for (int i = 0; i < 234937; i++) {
		if (strcmp(word->word, dictionary[i]) == 0) {
			in_dictionary = true;
			break;
		}
	}
	if (!in_dictionary) {
		return -((length - 2) * (length - 2));
	}

	return (length - 2) * (length - 2);
}

// Calculate squared distance between two location structs
int squareDistance(Location l1, Location l2) {
	return (l1.row - l2.row) * (l1.row - l2.row) + (l1.column - l2.column) * (l1.column - l2.column);
}


/**********************************************************************
Different versions of getPeakMemory() for different operating systems,
selected by #define at the top of this file.

LINUX
MAC_OS
WINDOWS

OTHERS: getPeakMemory() is not supported
***********************************************************************/

#ifdef LINUX
// return peak usage of virtual memmory in bytes
long getPeakMemory() {
    FILE *procFile = fopen("/proc/self/status", "r");
    if (!procFile) {
        perror("/proc/self/status: ");
        return -1;
    }

    long peakVirtualMemory=0;    // VmPeak
    char line[128];
    bool done = false;

    while ((fgets(line, 128, procFile) != NULL) && (done == false))
    {
        // "Peak virtual memory size"
        if (strncmp(line, "VmPeak:", 7) == 0)  // check the first 7 chars
        {
            sscanf(line+7, "%ld", &peakVirtualMemory);  // in kB
            done = true;
        }
    }
    fclose(procFile);
    return peakVirtualMemory * 1024;
}
#endif


#ifdef MAC_OS
#include <sys/time.h>
#include <sys/resource.h>
// return max resident set size
long getPeakMemory() {
    struct rusage usage;

    if(getrusage(RUSAGE_SELF, &usage) == 0)
        //"the maximum resident set size utilized (in kilobytes)"
        return (long)usage.ru_maxrss * 1024;
    else
        return -1;
}
#endif


#ifdef WINDOWS
#include <windows.h>
#include <psapi.h>

// return total amount of memory committed in bytes
long getPeakMemory() {
    PROCESS_MEMORY_COUNTERS info;

    if (GetProcessMemoryInfo(GetCurrentProcess(), &info, sizeof(info)) != 0) {
      //"The peak value in bytes of the Commit Charge during the lifetime of this process."
      return (long)info.PeakPagefileUsage;
    }
    else {
        fprintf(stderr, "GetProcessMemoryInfo() error\n");
        return -1;
    }
}
#endif


#ifdef OTHERS

// return total amount of memory committed in bytes
long getPeakMemory()
{
    fprintf(stderr, "getPeakMemory() not supported\n");
    return -1;
}
#endif
