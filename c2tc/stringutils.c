#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "stringutils.h"
#include "inttypes.h"

int8 endswith(char* str1, char* str2)
{
	int32 max = strlen(str1);
	if (strlen(str2) > max) return 0;
	for (int32 i = max; i >= 0; i--) if (str1[i] != str2[i]) return 0;
	return 1;
}
int8 startswith(char* str1, char* str2)
{
	int32 max = strlen(str1);
	if (strlen(str2) > max) return 0;
	for (int32 i = 0; i < max; i++) if (str1[i] != str2[i]) return 0;
	return 1;
}
//Credits to Xenon from StackOverflow for the original code of this function, which was then changed and formatted by me
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
char *skip_spaces(const char *str)
{
	char* temp = malloc(strlen(str) + 1); temp = strdup(str);
	int32 i = 0;
	while (isspace(*str))
		++i;
	return &temp[i];
}