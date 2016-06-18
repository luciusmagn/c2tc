#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "inttypes.h"
#include "shared.h"

int32 main(int32 argc, char** argv)
{
	switch (argc)
	{
	case 1:
		puts("mpctest -mode path");
		break;
	case 2:
		puts("missing filename arg. terminating");
		break;
	case 3:
		if (strlen(argv[1]) > 1)
			switch (argv[1][1])
			{
			case 'c':
				puts("c2parser     \n"
					 "============="
				);
				c2main(2, &argv[1]);
				break;
			case 'r':
				puts("recipe parser\n"
					"=============="
					);
				recipemain(2, &argv[1]);
			default:
				puts("mpctest -mode path");
				break;
			}
		break;
	}
	//getchar();
}

