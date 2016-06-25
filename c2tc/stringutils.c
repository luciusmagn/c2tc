#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "stringutils.h"
#include "inttypes.h"

int8 endswith(char* str1, char* str2)
{
	if (strlen(str1) < strlen(str2)) return 0;
	int8 flag = 1;
	int32 sindex = 0;
	for (int32 i = strlen(str1) - strlen(str2) - 2; i < strlen(str1); i++)
	{
		printf("%c and %c\n", str1[i], str2[sindex]);
		if (str1[i] == str2[sindex])
		{
			sindex++;
			continue;
		}
		else
		{
			flag = 0;
			break;
		}
	}
	return flag;
}

int8 startswith(char* str1, char* str2)
{

}