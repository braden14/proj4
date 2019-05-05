#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <omp.h>

// Number of threads to run
#define NUM_THREADS 16
// Max length of a line
#define MAX_LINE_LENGTH 10000
// Number of lines in a file
#define LINE_COUNT 1000000
// Name of the file to read from
char fileName[] = "/homes/dan/625/wiki_dump.txt";

//
// Function finds the longest substring in two line
//
char * longestSub(char * lines1, int len1, char * lines2, int len2)
{
	int i, j, indexOfI, length = 0;
	int ** subCount = (int **)malloc(len1 * sizeof(int *));

	for(i = 0; i < len1; i++)
	{
		subCount[i] = (int *)malloc(len2 * sizeof(int));
		for (j = 0; j < len2; j++)
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
	for(i = 0; i < len1; i++) 
		free(subCount[i]);
	free(subCount);
	char * result = malloc(sizeof(char)*(length + 1));
	strncpy(result, &lines1[indexOfI - length + 1], length);
	result[length] = '\0';
	return result;
}

//
// Function gets list of all lines in txt file
//
char ** getLines()
{
	int i = 0;
	FILE * file = fopen(fileName, "r");
	char ** temp = malloc(MAX_LINE_LENGTH*sizeof(char*));
	char * line = malloc(MAX_LINE_LENGTH * sizeof(char));
	while(fgets(line, MAX_LINE_LENGTH, file) != NULL)
	{
		int length = strlen(line);
		line[length - 1] = '\0';
		temp[i] = malloc(length*sizeof(char));
		strcpy(temp[i], line);
		i++;
	}
	free(line);
	return temp;
}

//
// Function uses longestSubstring to find longest substring of all lines
//
char ** doLongestSub(char ** lines)
{
	int threadID, i, startIndex, endIndex;
	char ** subStrings = malloc((LINE_COUNT - 1)*sizeof(char*));
	omp_set_num_threads(NUM_THREADS);
	#pragma omp parallel private(threadID, i, startIndex, endIndex)
	{
		threadID = omp_get_thread_num();

		startIndex = (threadID) * (LINE_COUNT / NUM_THREADS);
		endIndex = startIndex + (LINE_COUNT / NUM_THREADS);

		if(threadID == NUM_THREADS-1)
		{
		  endIndex = LINE_COUNT - 1;
		}

		for(i = startIndex; i < endIndex; i++)
		{
			subStrings[i] = longestSub(lines[i], strlen(lines[i]), lines[i+1], strlen(lines[i+1]));
		}
	}
	return subStrings;
}

//
// main
//
int main()
{
	int i;
	time_t current_time;
  current_time = time(NULL);

	char ** lines = getLines();

	char ** subStrings = doLongestSub(lines);

	for(i = 0; i < LINE_COUNT - 1; i++)
	{
		printf("%s\n", subStrings[i]);
	}

	printf("\nStart time is %s", ctime(&current_time));
	current_time = time(NULL);
	printf("End time is %s\n", ctime(&current_time));
}
