#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "microtest.h"
#include "inttypes.h"
#include "shared.h"
#include "stringutils.h"
#include "throw.h"
#include "errors.h"

int32 main(int32 argc, char** argv)
{
	init_errors();
    c2main(argc, argv);
    return 0;
}
