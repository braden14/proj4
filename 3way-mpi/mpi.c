/* To run on Beocat:
		get rid of windows newlines on scripts
		make sure path of shell_script_mpi.sh is right
		compile using "mpicc mpi.c -o mpi_out"
		./mass_mpi to queue
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <mpi.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/sysinfo.h>

// Max length of a line
#define MAX_LINE_LENGTH 1000000
// Number of lines in a file
#define LINE_COUNT 1000
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

int display(char ** subStrings)
{
	int i;
	for (i = 0; i < LINE_COUNT - 1; i++)
	{
		printf("Line: %d | %s\n", i, subStrings[i]);
	}
	return 0;
}

//
// Function uses longestSubstring to find longest substring of all lines
//
char ** doLongestSub(char ** lines, int argc, char * argv[])
{
	int threadRank, numTasks, i, startIndex, endIndex, rc;
	
	MPI_Status state;
	MPI_Request *req = NULL;

	//initializes the MPI execution environment
	rc = MPI_Init(&argc, &argv);
	//to handle capturing a return/error code of MPI_SUCCESS's failure [return 0]
	if(rc != MPI_SUCCESS)
	{
		printf("MPI could not start.\n");

		//represents the # of MPI tasks available to app
		MPI_Abort(MPI_COMM_WORLD, rc);
	}
	
	//returns the total number of MPI processes in the specified communicator
	MPI_Comm_size(MPI_COMM_WORLD, &numTasks);
	//returns the rank of the calling MPI process within the specified communicator
	MPI_Comm_rank(MPI_COMM_WORLD, &threadRank);
	printf("\tThread rank: %d\n", threadRank);

	char ** subStrings = malloc((LINE_COUNT - 1) * sizeof(char *));

	startIndex = (threadRank) * (LINE_COUNT / numTasks);
	endIndex = startIndex + (LINE_COUNT / numTasks);

	if(threadRank == numTasks - 1)
	{
	  endIndex = LINE_COUNT - 1;
	}

	for(i = startIndex; i < endIndex; i++)
	{
		subStrings[i] = longestSub(lines[i], strlen(lines[i]), lines[i+1], strlen(lines[i+1]));
		printf("On thread %d. \n\tIndex %d\n\tResult: %s\n", threadRank, i, subStrings[i]);
	}
	MPI_Barrier(MPI_COMM_WORLD);
	//MPI_Finalize();

	int dumby;
	if (threadRank == 0)
	{
		dumby = display(subStrings);
		dumby = display(subStrings);
	}

	return subStrings;
}



//
// main
//
int main(int argc, char * argv[])
{
	//printf("%d/n", 1);
	int i;
	time_t current_time;
	current_time = time(NULL);
	//printf("%d/n", 2);
	char ** lines = getLines();
	//printf("%d/n", 3);
	char ** subStrings = doLongestSub(lines, argc, argv);
	//printf("%d/n", 4);
	int threadRank;
	//exit(0);
	//MPI_Comm_rank(MPI_COMM_WORLD, &threadRank);
	//for(i = 0; i < LINE_COUNT - 1; i++)
	//{
	//	printf("Line: %d | %s\n", i, subStrings[i]);
	//}

	printf("\nStart time is %s", ctime(&current_time));
	current_time = time(NULL);
	printf("End time is %s\n", ctime(&current_time));
}
