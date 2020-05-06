#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>





void addString(int beforeLen, int currentLen, char* text, char* buffer);
int createBadMatchTable(char find[50] , char found[50], int *badMatchTableValues);
int findShiftValue(char textChar, char badMatchTable[50], int badMatchTableValues[50], int badMatchTableLen);
void rePlace(char* text, char replace[50], int replaceLen, int index);
void shiftCharsRigth(char* text, int replaceLen, int findLen, int index, int currentLen);
void shiftCharsLeft(char* text, int replaceLen, int findLen, int index, int currentLen);



int main() {

	int i,currentLen = 0,beforeLen, caseOption,foundCounter = 0;
	char buffer[100] = { 0 }, find[50] = { 0 }, replace[50] = { 0 };
	char* text = (char*)calloc(1,sizeof(char));
	int badMatchTableValues[50] = { 0 }, badMatchTableLen;
	char badMatchTable[50] = { 0 };

	double time_spent = 0.0;

	FILE* fp;

	fp = fopen("input.txt", "r");

	if (fp == NULL) {
		printf("input.txt does not exist.");
		return 0;
	}

	fscanf(fp, "%s", buffer);
	currentLen = strlen(buffer);
	currentLen++;
	text = (char*)realloc(text, currentLen * sizeof(char));
	strcat(text, buffer);
	
	while (!feof(fp)) {
		text[currentLen - 1] = ' ';
		fscanf(fp, "%s", buffer);
		beforeLen = currentLen;
		currentLen += strlen(buffer);
		currentLen++;
		text = (char*)realloc(text, currentLen*sizeof(char));
		addString(beforeLen, currentLen, text, buffer);
	}

	printf("\nWhich word do you want to find : ");
	gets(find);
	printf("\nWhat would you like to replace this word with ? : ");
	gets(replace);
	printf("For Case Sensitive press 0.\nFor not case Sensitive press 1.\n");
	scanf("%d", &caseOption);
	
	printf("\nTEXT : %s\n", text);

	badMatchTableLen = createBadMatchTable(find, badMatchTable, badMatchTableValues);
	badMatchTableLen++;

	printf("Bad Match Table\n");
	for (i = 0; i < badMatchTableLen; i++) {
		printf("%c : %d |",badMatchTable[i], badMatchTableValues[i]);
	}
	printf("\n");

	//HORSEPOOL ALGORITHM
	i = strlen(find) - 1;
	int j = i;
	int textPoint = i;
	int findLen = i;
	int caseChanger = 0;
	int replaceLen = strlen(replace);

	clock_t begin = clock();
	

	while (text[textPoint] != NULL) {
		i = textPoint;

		if (caseOption == 1) {
			caseChanger = 0;
			if (text[i] >= 65 && text[i] <= 90) {
				if (find[j] >= 97 && find[j] <= 122) {
					caseChanger =  32;
				}
			}
			else if (text[i] >= 97 && text[i] <= 122) {
				if (find[j] >= 65 && find[j] <= 90) {
					caseChanger = - 32;
				}
			}
		}

		while (j >= 0 && (text[i] + caseChanger) == find[j]) {
			j--;
			i--;

			if (caseOption == 1) {
				caseChanger = 0;
				if (text[i] >= 65 && text[i] <= 90) {
					if (find[j] >= 97 && find[j] <= 122) {
						caseChanger = 32;
					}
				}
				else if (text[i] >= 97 && text[i] <= 122) {
					if (find[j] >= 65 && find[j] <= 90) {
						caseChanger = -32;
					}
				}
				
			}
		}
		
		if (j < 0) {
			
			if (replaceLen > findLen + 1) {
				currentLen = currentLen + replaceLen - findLen - 1;
				text = (char*)realloc(text, currentLen * sizeof(char));
				shiftCharsRigth(text, replaceLen, findLen + 1, i + 1, currentLen);
				rePlace(text, replace, replaceLen, i + 1);;
			}

			else if (findLen + 1 > replaceLen) {
				shiftCharsLeft(text, replaceLen, findLen + 1, i + 1, currentLen);
				currentLen = currentLen + replaceLen - findLen - 1;
				text = (char*)realloc(text, currentLen * sizeof(char));
				rePlace(text, replace, replaceLen, i + 1);

			}
			
			else {
				rePlace(text, replace, replaceLen, i + 1);
			}
			
			foundCounter++;
			textPoint = textPoint + replaceLen;
			j = findLen;
		}
		else {
			textPoint = textPoint + 
				findShiftValue(text[i], badMatchTable, badMatchTableValues, badMatchTableLen);
			j = findLen;
		}


	}

	clock_t end = clock();
	time_spent += difftime(end,begin) / CLOCKS_PER_SEC;

	fp = fopen("input.txt", "w");
	fprintf(fp, "%s", text);

	printf("\nNEW TEXT : %s\n", text);
	printf("Found and Replaced : %d\n", foundCounter);
	printf("Running Time : %f\n", time_spent);
	
	




	return 0;
}

void rePlace(char* text,char replace[50], int replaceLen, int index) {

	int i;

	for (i = 0; i < replaceLen; i++) {
		text[i + index] = replace[i];
	}

}

void shiftCharsLeft(char* text, int replaceLen, int findLen, int index, int currentLen) {

	int shift = findLen - replaceLen;
	int i;

	for (i = index + findLen; i < currentLen; i++) {
		text[i - shift] = text[i];
	}


}

void shiftCharsRigth(char* text, int replaceLen, int findLen , int index, int currentLen) {
	int shift = replaceLen - findLen;
	int i;

	for (i = currentLen - shift - 1; i >= findLen + index; i--) {
		text[i + shift] = text[i];
	}

}

int findShiftValue(char textChar, char badMatchTable[50], int badMatchTableValues[50], int badMatchTableLen) {

	int i = 0;

	while (i < badMatchTableLen - 1 && (textChar != badMatchTable[i] && textChar + 32 != badMatchTable[i] && textChar - 32 != badMatchTable[i])) {
		i++;
	}
	if (textChar == badMatchTable[i] || textChar + 32 || badMatchTable[i] || textChar - 32 == badMatchTable[i]) {
		return badMatchTableValues[i];
	}
	else {
		return badMatchTableValues[badMatchTableLen - 1];
	}


}

void addString(int beforeLen, int currentLen,char *text, char *buffer) {

	int i;

	for (i = 0; i < currentLen - beforeLen; i++) {
		text[i + beforeLen] = buffer[i];
	}

}
int createBadMatchTable(char find[50], char found[50], int values[50]) {
	int i = 0,j = 0,lenght = 0,findLenght;
	findLenght = strlen(find);

	while (find[i] != NULL) {

		while (found[j] != NULL && found[j] != find[i]) {
			j++;
		}
		if (found[j] != find[i]) {
			found[j] = find[i];
			lenght++;
		}
		if (findLenght - i - 1 == 0) {
			values[j] = findLenght;
		}
		else {
			values[j] = findLenght - i - 1;
		}
		i++;
		j = 0;
	}
	found[lenght] = '*';
	values[lenght] = findLenght;
	return lenght;
}



