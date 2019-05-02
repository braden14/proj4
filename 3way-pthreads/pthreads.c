#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <pthread.h>


int maxlineCount = 100; 					// GIVE ME LARGEST LINE VALUE
int lineCount = 12; 						// GIVE ME THE TOTAL NUMBER OF LINES
char fileName[] = "/homes/dan/625/wiki_dump.txt"; 		// GIVE ME THE NAME OF THE FILE TO READ
int threadCount = 16; 						// GIVE ME NUMBER OF THREADS

char ** subStrings;							// holds substrings
char ** lines;								// holds full lines

typedef struct
{
	int start;
	int end;
} args_t;


// 
// Function finds the longest substring in two line
//
char * longestSub(char * lines1, int len1, char * lines2, int len2)
{
	int ** subCount = (int ** )malloc(sizeof(int*)*len1);
	int length = 0;

	int indexOfI;
	int i;
	for(i = 0; i < len1; i++)
	{
		subCount[i] = (int *) malloc(len2 * sizeof(int));
		int j;
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
	char ** temp = malloc(maxlineCount * sizeof(char*));
	char lines[lineCount];
	while (fgets(lines, lineCount, file) != NULL)
	{
		int length = strlen(lines);
		if (*outNumberLines != lineCount - 1) lines[length - 3] = '\0';
		else lines[length - 1];
		temp[*outNumberLines] = malloc(length * sizeof(char));
		strcpy(temp[*outNumberLines], lines);
		(*outNumberLines)++;
	}
	return temp;
}

//
// Function uses longestSubstring to find longest substring of all lines
//
void* doLongestSub(void * param)
{ 
	args_t * args = (args_t*)param;
	int i;
	for(i = args->start; i < args->end; i++)
	{
		subStrings[i] = longestSub(lines[i], strlen(lines[i]), lines[i+1], strlen(lines[i+1]));
	}
}

//
// main 
//
int main()
{
	//printf("start");
	time_t current_time;
    current_time = time(NULL);
	//printf("after time");
	subStrings = malloc((lineCount) * sizeof(char*));
	
	// START MULTITHREADING
	//printf("1");
	pthread_t threads[threadCount];
	//printf("2");
	pthread_attr_t attr;
	//printf("3");
	pthread_attr_init(&attr);
	//printf("4");
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	//printf("5");
	lines = malloc((lineCount) * sizeof(char*));
	int * numberlines = malloc(sizeof(int));
	//printf("1");
	lines = getLines(numberlines);
	//printf("2");
	void * status;

	int linesToProcess = lineCount/threadCount;
	
	//args_t *args = (args_t*)malloc(sizeof(args_t));
	
	//printf("\nlol\n");

	int i, rc;
	for(i = 0; i < threadCount; i++)
	{
		args_t *args = (args_t*)malloc(sizeof(args_t));
		//printf("\nloop");
		args->start = linesToProcess * i;
		if(i == threadCount-1)
		{
			args->end = lineCount - 1;
		}
		else
		{
			args->end = linesToProcess * (i + 1);
		}
		rc = pthread_create(&threads[i], &attr, doLongestSub, (void *)args);
		if (rc)
		{
			printf("return code from pthread_create: %d", rc);
		}
	}

	//printf("\nafter threads start");

	pthread_attr_destroy(&attr);

	for (i = 0; i < threadCount; i++)
	{
		//printf("\nloop 2");

		rc = pthread_join(threads[i], &status);
		if (rc)
		{
			printf("return code from pthread_join: %d", rc);
		}
	}

	//printf("\nafter rejoin");

	

	// END MULTITHREADING
	for(i = 0; i < lineCount-1; i++)
	{
		printf("%s\n", subStrings[i]);
		//exit(0);
	}
	
	//printf("after print");

	//exit(0);

	printf("\nStart time is %s", ctime(&current_time));
	current_time = time(NULL);
	printf("End time is %s\n", ctime(&current_time));
}


