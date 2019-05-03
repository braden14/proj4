#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <mpi.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/sysinfo.h>

// Number of threads to run
#define NUM_THREADS 16
// Max length of a line
#define MAX_LINE_LENGTH 1000000
// Number of lines in a file
#define LINE_COUNT 200
// Name of the file to read from
char fileName[] = "../wiki_dump_small.txt";

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
	for(i = 0; i < len1; i++) free(subCount[i]);
	free(subCount);
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
	char ** temp = malloc(MAX_LINE_LENGTH*sizeof(char*));
	char lines[MAX_LINE_LENGTH];
	while(fgets(lines, MAX_LINE_LENGTH, file) != NULL)
	{
		int length = strlen(lines);
		if(*outNumberLines != LINE_COUNT - 1) lines[length-3] = '\0';
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
	int threadID, i, startIndex, endIndex;

	char ** subStrings = malloc((*numberlines-1)*sizeof(char*));
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
	MPI_Status state;
	MPI_Request *req = NULL;

	//variables for MPI calls
	int rc, taskID, numTasks;
	//initializes the MPI execution environment
	rc = MPI_Init(&argc, &argv);

	//to handle capturing a return/error code of MPI_SUCCESS's failure [return 0]
	if(rc != MPI_SUCCESS)
	{
		printf("MPI could not start.\n")

		//represents the # of MPI tasks available to app
		MPI_Abort(MPI_COMM_WORLD, rc);
	}

	//returns the total number of MPI processes in the specified communicator
	MPI_Comm_size(MPI_COMM_WORLD, &numTasks);
	//returns the rank of the calling MPI process within the specified communicator
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int i;
	time_t current_time;
	current_time = time(NULL);
	//idk why fflush(stdout)


	int * numberlines = malloc(sizeof(int));
	char ** lines = getLines(numberlines);

	char ** subStrings = doLongestSub(lines, numberlines);

	for(i = 0; i < *numberlines-1; i++)
	{
		printf("%s\n", subStrings[i]);
	}

	printf("\nStart time is %s", ctime(&current_time));
	current_time = time(NULL);
	printf("End time is %s\n", ctime(&current_time));


	//done with MPI_Init
	MPI_Finalize();
}
