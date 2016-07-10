#ifndef microtest_h
#define microtest_h
#include "vasprintf.h"
#include "inttypes.h"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

char* finalmessage;
int32 tests_run;
int32 tests_passed;
/// <summary>
///  Use this to signify a start of a file
/// </summary>
#define tiny_file() do{asprintf(&finalmessage, "%s\x1b[33mFile - %s\x1b[0m\n\n", (finalmessage ? finalmessage : ""), __FILE__);} while(0)
/// <summary>
///  Basic assertion, appends test results to finalmessage.
/// </summary>
#define tiny_assert(message, test) do { tests_run++; if (!(test)) {char* error; asprintf(&error, "[%s:%d]\x1b[31mTEST FAILED\x1b[0m: %s\n",__FILE__, __LINE__, message); asprintf(&finalmessage, "%s%s", finalmessage, error);}\
								else {char* success; tests_passed++; asprintf(&success, "[%s:%d]\x1b[32mTEST PASSED\x1b[0m: %s\n", __FILE__, __LINE__, message); asprintf(&finalmessage, "%s%s", finalmessage, success);} } while (0)
/// <summary>
///  This macro runs a test, a function which should return a char pointer in case a test fails. Result is appended to finalmessage
/// </summary>
#define tiny_test(test) do { char *message = test(); tests_run++; \
                                if (message){char* error; asprintf(&error, "[%s:%d]\x1b[31mTEST FAILED\x1b[0m: %s\n",__FILE__,__LINE__, message); asprintf(&finalmessage, "%s%s", finalmessage, error);}\
								else {char* success; tests_passed++; asprintf(&success, "[%s:%d]\x1b[32mTEST PASSED\x1b[0m: %s\n", __FILE__,__LINE__, message); asprintf(&finalmessage, "%s%s", finalmessage, success);} } while (0)

/// <summary>
///  This function runs all tests and prints verdict to stdout
/// </summary>
static __inline void tiny_verdict()
{
	printf("\x1b[36mmicrotest framework - results [Tests passed: %d/%d]\n\x1b[36m", tests_passed, tests_run);
	  puts("==============================================");
	puts(finalmessage);
}

#endif
