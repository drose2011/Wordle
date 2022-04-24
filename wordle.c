#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mylist.h"

#define BUF_SIZE 4069


int locked[5] = {0,0,0,0,0};
struct List ogList;

int getLen(struct List *list) 
{
	int len = 0;
	struct Node *cur = list->head;
	while(cur) {
		len++;
		cur = cur->next;
	}
	return len;
}

struct Node *removeNode(struct List *list, struct Node *cur, struct Node *last)
{
	//printf("Removing %s\n", cur->data);
	struct Node *next = cur->next;
	if(last == NULL) {
		list->head = next;
	} else {
		last->next = next;
	}
	//printf("\tRemoving Node w data: %s\n", cur->data);
	free(cur->data);
	free(cur);
	return next;
}

void alphaInverse(char *in)
{
	printf("starting string: %s\n", in);
	char temp[27];
	strcpy(temp, in);
	in[0] = '\0';
	char temp2[2];
	temp2[1] = '\0';
	for(int i='a'; i<='z'; i++) {
		if(!strchr(temp, i)) {
			temp2[0] = i;
			strncat(in, temp2, 1);
		}
	}
	printf("output string: %s\n", in);
}

void pruneList(struct List *list, int val, char chr, int pos) 
{
	struct Node *cur = list->head;
	struct Node *last = NULL;
	char *temp;
	if(val == 0) {
		// Remove all words containing this letter
		while(cur) {
			if((temp = strchr(cur->data, chr))) {
				//printf("locked = {%d,%d,%d,%d,%d}, idx: %ld\n", locked[0], locked[1],locked[2],locked[3],locked[4], temp - (char *)(cur->data));
				if(locked[temp - (char *)(cur->data)] == 0) {
				//	printf("Removed becasue %c is not in word, and %s does contain it.\n", chr, cur->data);
					cur = removeNode(list, cur, last);	
				}
			} else {
				last = cur;
				cur = cur->next;
			}
		}
	} else if(val == 1) {
		// Remove all words not containing this letter, or with the letter in this location
		while(cur) {
			if(!strchr(cur->data, chr)) {
		//		printf("Removed becasue %c is in word, and %s doesn't contain it.\n", chr, cur->data);
				cur = removeNode(list, cur, last);
			} else if(  ((char *)(cur->data))[pos] == chr) {
		//		printf("Removed becasue %c is in word, but not in the position it is in in %s.\n", chr, cur->data);
				cur = removeNode(list, cur, last);
			} else {
				last = cur;
				cur = cur->next;
			}
		}
	} else if(val == 2) {
		// Remove all words that don't have this letter in this position
		while(cur) {
			if(  ((char *)(cur->data))[pos] != chr) {
		//		printf("Removing becasue %c is not in the right position/not in %s.\n", chr, cur->data);
				cur = removeNode(list, cur, last);
			} else {
				last = cur;
				cur = cur->next;
			}
		}
		locked[pos] = 1;
	}
}

int countNewChars(char *word, char *guessed, int *locked, int elim)
{
	int sum = 0;
	char *temp;
	for(char i='a'; i<='z'; i++) {
		if((temp = strchr(word, i)) && !strchr(guessed, i)) {
			if(strchr("aeiou", i)) 
				sum++;
			sum++;
			if(locked[(int)(temp - word)] == 0 && elim) {
				char temp[2];
				temp[0] = i; temp[1] = '\0';
				strcat(guessed, temp);
			}
		}
	}
	return sum;
}

void myPrint(void *word) 
{
	printf("%s\n", (char *)word);
}

void getGuess(struct List *list, char *guess, char *guessed) {
	int elim = 0;

	int numLocked = 0;
	for(int i=0;i<5;i++) numLocked += locked[i];
	

	char remaining[27];
	if((numLocked >= 4 && getLen(list) >= 8) || (numLocked == 3 && getLen(list) >= 15)) {
		printf("switching tacts\n");
		elim = 1;
		struct Node *cur = list->head;
		while(cur) {
			countNewChars(cur->data, remaining, locked, 1);
			cur = cur->next;
		}
		alphaInverse(remaining);

		guessed = remaining;

		list = &ogList;	


		// TODO:
		// Make guessed actually contain letters to be eliminated 
		// 	Look through current list and add each element not in a locked spot to a char
		// 	No duplicates
		// feed it new list with everything
		// 	Save the starting list
		// 	feed this into list
		// 	reset to original list at the end
		// 	Don't delete an item from total list, but do strcpy
	} 
	
	struct Node *cur = list->head;
	struct Node *last = NULL;
	struct Node *maxNode = NULL;
	struct Node *maxLastNode = NULL;
	int max = -1;

	while(cur) {
		int newChars = countNewChars(cur->data, guessed, locked, 0);
		if(newChars > max) {
			max = newChars;
			maxNode = cur;
			maxLastNode = last;
		}
		last = cur;
		cur = cur->next;
	}

	strcpy(guess, maxNode->data);
	if(!elim) removeNode(list, maxNode, maxLastNode);
}

void updateGuessed(char *guess, char *guessed) {
	for(int i=0; guess[i]; i++) {
		if(!strchr(guessed, guess[i])) {
			strncat(guessed, (guess+i), 1);
		}
	}
}



int main()
{
	char buf[BUF_SIZE];

	struct List list;
	initList(&list);
	initList(&ogList);

	FILE *dict5 = fopen("dict5.txt", "r");
	while(fgets(buf, BUF_SIZE, dict5) != NULL) {
		char *cpy = malloc(sizeof(char) * 6);
		char *cpy2 = malloc(sizeof(char) * 6);
		cpy[5] = '\0';
		cpy2[5] = '\0';
		strncpy(cpy,buf,5);
		strncpy(cpy2,buf,5);
		addFront(&ogList, cpy);
		addFront(&list, cpy2);
	}
	reverseList(&ogList);
	reverseList(&list);

	char guessed[27];
	guessed[0] = '\0';
	char guess[6];
	char singleNum[2];
	singleNum[1] = '\0';
	
	getGuess(&list, guess, guessed);

	while(1) {
		printf("Guess: \t\t%s\n", guess);
		printf("Accuracy: \t");
		fgets(buf, BUF_SIZE, stdin);

		if(buf[0] == 'q')
			break;
		if(buf[0] == 'n') {
			getGuess(&list, guess, guessed);
			continue;
		}
		if(buf[0] == 'p') {
			traverseList(&list, &myPrint);
			continue;
		}

		if(strlen(buf) != 6) {
			printf("Please input 5 numbers\n");
			continue;
		}

		updateGuessed(guess, guessed);

		//TODO: Error Checking input
		int values[6];
		
		for(int i=0; guess[i]; i++) {
			singleNum[0] = buf[i];
			values[i] = atoi(singleNum);
		}

		for(int i=2; i>=0; i--) {
			for(int j=0; guess[j]; j++) {
				if(values[j] == i) {
					printf("evaling the %d position with val = %d\n", j, i);
					pruneList(&list, values[j], guess[j], j);
				}
			}
		}
/*
		for(int i=0; guess[i]; i++) {
			singleNum[0] = buf[i];
			val  = atoi(singleNum);
			pruneList(&list, val, guess[i], i);
		}
*/
		if(getLen(&list) == 0) {
			printf("No more words left in list.\n");
			exit(0);
		} if(getLen(&list) < 8) {
			printf("\nThese are the remaining few, pick the most reasonable:\n");
			traverseList(&list, &myPrint);
			printf("\n");

			printf("Which one do you choose(starting at 0): ");
			fgets(buf, BUF_SIZE, stdin);
			int choiceNum = atoi(buf);
			struct Node *choice = list.head;
			struct Node *last = NULL;
			while(choiceNum--) {
				last = choice;
				choice = choice->next;
			}
			strcpy(guess, choice->data);
			removeNode(&list, choice, last);

		} else {
			getGuess(&list, guess, guessed);
		}
	}


	fclose(dict5);
	traverseList(&list, &free);
	removeAllNodes(&list);
}



