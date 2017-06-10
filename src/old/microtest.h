#ifndef microtest_h
#define microtest_h
#include "util.h"
/*
char* finalmessage;
int32 tests_run;
int32 tests_passed;

//Use to mark a new file
#define tiny_file() do{asprintf(&finalmessage, "%s\x1b[33mFile - %s\x1b[0m\n\n", (finalmessage ? finalmessage : ""), __FILE__);} while(0)

//assert, appends test results to finalmessage.
#define tiny_assert(message, test) do { tests_run++; if (!(test)) {char* error; asprintf(&error, "[%s:%d]\x1b[31mTEST FAILED\x1b[0m: %s\n",__FILE__, __LINE__, message); asprintf(&finalmessage, "%s%s", finalmessage, error);}\
								else {char* success; tests_passed++; asprintf(&success, "[%s:%d]\x1b[32mTEST PASSED\x1b[0m: %s\n", __FILE__, __LINE__, message); asprintf(&finalmessage, "%s%s", finalmessage, success);} } while (0)

//runs a test, a function which should return a char pointer in case a test fails.
//result is appended to finalmessage
#define tiny_test(test) do { char *message = test(); tests_run++; \
								if (message){char* error; asprintf(&error, "[%s:%d]\x1b[31mTEST FAILED\x1b[0m: %s\n",__FILE__,__LINE__, message); asprintf(&finalmessage, "%s%s", finalmessage, error);}\
								else {char* success; tests_passed++; asprintf(&success, "[%s:%d]\x1b[32mTEST PASSED\x1b[0m: %s\n", __FILE__,__LINE__, message); asprintf(&finalmessage, "%s%s", finalmessage, success);} } while (0)

//runs all tests
static __inline void tiny_verdict()
{
	printf("\x1b[36mmicrotest framework - results [Tests passed: %d/%d]\n\x1b[36m", tests_passed, tests_run);
	  puts("==============================================");
	puts(finalmessage);
}
*/
#endif
