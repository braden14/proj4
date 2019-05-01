#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

int numberlines = 100000; // GIVE ME LARGEST LINE VALUE
int lineLength = 12; // GIVE ME THE TOTAL NUMBER OF LINES
char fileName[] = "testFile.txt"; // GIVE ME THE NAME OF THE FILE TO READ

// 
// Function finds the longest substring in two line
//
char * longestSub(char * lines1, int len1, char * lines2, int len2)
{
	int subCount[len1][len2];
	int length = 0;

	int indexOfI;
	
	for(int i = 0; i < len1; i++)
	{
		for (int j = 0; j < len2; j++)
		{
			if(lines1[i] == lines2[j])
			{
				if(i > 0 && j > 0)
				{
					subCount[i][j] = 1 + subCount[i-1][j-1];
					if (subCount[i][j] > length)
					{
						length = subCount[i][j];	
						indexOfI = i;
					}
				}
				else 
				{
					subCount[i][j] = 1;
					if (subCount[i][j] > length)
					{
						length = subCount[i][j];
						indexOfI = i;
					}
				}
			}
			else
			{
				subCount[i][j] = 0;
			}
		}
	}
	char * result = malloc(sizeof(char)*length);
	strncpy(result, &lines1[indexOfI - length + 1], length);
	return result;
}

// 
// Function gets list of all lines in txt file
//
char ** getLines(int * outNumberLines)
{
	*outNumberLines = 0;
	FILE * file = fopen(fileName, "r");
	char ** temp = malloc(numberlines*sizeof(char*));
	char lines[lineLength];
	while(fgets(lines, lineLength, file) != NULL)
	{
		int length = strlen(lines);
		if(*outNumberLines != lineLength - 1) lines[length-3] = '\0';
		else lines[length-1];
		temp[*outNumberLines] = malloc(length*sizeof(char));
		strcpy(temp[*outNumberLines], lines);
		(*outNumberLines)++;
	}
	return temp;
}

//
// Function uses longestSubstring to find longest substring of all lines
//
char ** doLongestSub(char ** lines, int * numberlines)
{
	char ** subStrings = malloc((*numberlines-1)*sizeof(char*));
	for(int i = 0; i < *numberlines - 1; i++)
	{
		subStrings[i] = longestSub(lines[i], strlen(lines[i]), lines[i+1], strlen(lines[i+1]));
	}
	return subStrings;
}

//
// main 
//
int main()
{
	time_t current_time;
    current_time = time(NULL);
	
	int * numberlines = malloc(sizeof(int));
	char ** lines = getLines(numberlines);
	
	char ** subStrings = doLongestSub(lines, numberlines);
	
	for(int i = 0; i < *numberlines-1; i++)
	{
		printf("%s\n", subStrings[i]);
	}
	
	printf("\nStart time is %s", ctime(&current_time));
	current_time = time(NULL);
	printf("End time is %s\n", ctime(&current_time));
}

