#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#include "stringutils.h"
#include "inttypes.h"
#include "microtest.h"

int8 endswith(char *string, char* suffix)
{
	char* loc = strstr(string, suffix);

	if (string != NULL)
		return !strcmp(loc, suffix);

	return(0);
}
int8 startswith(char* str1, char* str2)
{
	return !strncmp(str1, str2, strlen(str2));
}
//Credits to Xenon from StackOverflow for the original code of this function, which was then changed and formatted by me
//Usage:
//char *arrayWhereTheExplodedStringsWillGo[];
//char *string = "Hello, world!";
//split(arrayWhereTheExplodedStringsWillGo, ',', string);
int8 split(char *dest[], const char *delimiter, const char *str)
{
	int32 numsplits = 1;
	const uint64 delimiterlength = strlen(delimiter);
	const uint64 strlength = strlen(str);

	if (delimiterlength != 1)
	{
		int32 delimiterencountered = 0;

		for (uint64 str_iterator = 0; str_iterator < strlength; str_iterator++)
		{
			if (delimiterencountered == delimiterlength)
			{
				numsplits++;
				realloc((char *)dest, numsplits + sizeof(char *));
			}
			else if (*str == *(delimiter + delimiterencountered))
			{
				delimiterencountered++;
			}
			else
			{
				if (delimiterencountered > 0) delimiterencountered = 0;
				*dest[numsplits] = *str;
				dest[numsplits]++;
			}
		}
	}
	else
	{
		for (uint64 str_iterator = 0; str_iterator < (strlength - 1); str_iterator++)
		{
			if (*str == *delimiter)
			{
				numsplits++;
				realloc((char *)dest, numsplits * sizeof(char *));
			}
			else
			{
				*dest[numsplits] = *str;
				dest[numsplits]++;
			}
			str++;
		}
	}
	return numsplits;
}

char** strsplit(char* str, const char* delim)
{
	char** res = NULL;
	char*  part;
	int i = 0;

	char* aux = strdup(str);

	part = strdup(strtok(aux, delim));

	while (part) {
		res = (char**)realloc(res, (i + 1) * sizeof(char*));
		*(res + i) = strdup(part);

		part = strdup(strtok(NULL, delim));
		i++;
	}

	res = (char**)realloc(res, i * sizeof(char*));
	*(res + i) = NULL;

	return res;
}
char *skip_spaces(const char *str)
{
	while (isspace(*str))
		++str;
	return str;
}

void teststrings()
{
	tiny_file();
	tiny_assert("test if endswith() works properly", endswith("test", "est"));
	tiny_assert("test if startswith() works properly", startswith("test", "tes"));
	tiny_assert("test if split() works properly", (strcmp(strsplit("This is a test", " ")[0], "This") == 0));
	char* str = "       test";
	tiny_assert("test if skip_spaces() works properly", (strcmp(skip_spaces(str), "test") == 0));
}