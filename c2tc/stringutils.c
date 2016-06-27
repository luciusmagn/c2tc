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