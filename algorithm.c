#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


char * longestSub(char * word1, int len1, char * word2, int len2)
{
	int subCount[len1][len2];
	int length = 0;

	int indexOfI;
	
	for(int i = 0; i < len1; i++)
	{
		for (int j = 0; j < len2; j++)
		{
			if(word1[i] == word2[j])
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
	strncpy(result, &word1[indexOfI - length + 1], length);
	return result;
}

char ** getWords(int * outNumberwords)
{
	*outNumberwords = 4;
	char ** temp = malloc(*outNumberwords*sizeof(char*));
	temp[0] = malloc(sizeof(char)*4);
	temp[0] = "cows";
	temp[1] = malloc(sizeof(char)*7);
	temp[1] = "plowing";
	temp[2] = malloc(sizeof(char)*4);
	temp[2] = "pigs";
	temp[3] = malloc(sizeof(char)*5);
	temp[3] = "twigs";
	temp[3] = malloc(sizeof(char)*5);
	temp[3] = "twins";
	return temp;
}

char ** doLongestSub(char ** words, int * numberWords)
{
	char ** subStrings = malloc((*numberWords-1)*sizeof(char*));
	for(int i = 0; i < *numberWords - 1; i++)
	{
		subStrings[i] = longestSub(words[i], strlen(words[i]), words[i+1], strlen(words[i+1]));
	}
	return subStrings;
}

int main()
{
	int * numberWords = malloc(sizeof(int));
	char ** words = getWords(numberWords);
	
	char ** subStrings = doLongestSub(words, numberWords);
	
	for(int i = 0; i < *numberWords-1; i++)
	{
		printf("%s\n", subStrings[i]);
	}
	
}

