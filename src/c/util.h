#ifndef inttypes_h
#define inttypes_h

#include <stdarg.h>

//numbers
#include <stdint.h>

#ifdef _WIN32
typedef __int64 int64;
typedef __int32 int32;
typedef __int16 int16;
typedef __int8 int8;
#else
typedef int64_t int64;
typedef int32_t int32;
typedef int16_t int16;
typedef int8_t int8;
#endif
typedef uint8_t  uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

//temporary evil
#define bool uint8
#define true 1
#define false 0

//strings
/*zero-indexed*/
#define get_line(src, index) strsplit(src, "\n")[index]
#define startswith(str1, str2) strncmp(str1, str2, strlen(str2))
bool endswith(char* str1,char* str2); /*[RUST DEFINED]*/
char** strsplit(char* str, const char* delim);
char *skip_spaces(const char *str);
uint32 occurences(char* str, char c); /*[RUST DEFINED]*/
char *str_replace(char *orig, char *rep, char *with);
char* bin_type_str(int32 type);

//asprintf & vasprintf
#ifndef HAVE_ASPRINTF
//copyright (c) 2014 joseph werle <joseph.werle@gmail.com>
//modified by me (Lukáš Hozda <luk.hozda@gmail.com>)
int vasprintf(char** str, const char* fmt, va_list args);
int asprintf(char** str, const char *fmt, ...);
#endif

//colors
#ifndef _WIN32
#define ANSI_RED     "\x1b[31m"
#define ANSI_GREEN   "\x1b[32m"
#define ANSI_YELLOW  "\x1b[33m"
#define ANSI_BLUE    "\x1b[34m"
#define ANSI_MAGENTA "\x1b[35m"
#define ANSI_CYAN    "\x1b[36m"
#define ANSI_RESET   "\x1b[0m"
#else
#define ANSI_RED	""
#define ANSI_GREEN	""
#define ANSI_YELLOW	""
#define ANSI_BLUE	""
#define ANSI_MAGENTA""
#define ANSI_CYAN	""
#define ANSI_RESET	""
#endif
//vector
typedef struct
{
	void** items;
	int32 capacity;
	int32 total;
} vector;

vector* vector_alloc();
void vector_init(vector* v);
int32 vector_total(vector* v);
void vector_resize(vector* v, int32 capacity);
void vector_add(vector* v, void* item);
void vector_set(vector* v, int32 index, void* item);
void* vector_get(vector* v, int32 index);
void vector_delete(vector* v, int32 index);
void vector_free(vector* v);

//llist
typedef struct _llist
{
	void* data;
	struct _llist* n;
} llist;

void llist_put(llist* l, void* data);
void* llist_get(llist* l, int32_t index, int32_t carry);
llist* llist_new(void* first);
int32_t llist_total(llist* l, int32_t carry);

//ast helpers
#define MODULE_PRE(x) x->children[1]->children[0]->children[1]

#endif