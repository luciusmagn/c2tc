#ifndef _DICT_H
#define _DICT_H

#define _DICT_LIMIT 10000

//The MIT License(MIT)

//Copyright(c) 2014 Ajay Madhusudan
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files(the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions :
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.

typedef struct dict {
	char key[_DICT_LIMIT];
	void* value;
	struct dict *next;
} dict;

dict *dict_new();
void dict_free(dict *d);
char *dict_get(dict *d, char *key);
void dict_set(dict *d, char *key, void *value);
void dict_del(dict *d, char *key);
int dict_has(dict *d, char *key);
int dict_len(dict *d);
char **dict_keys(dict *d);
void **dict_values(dict *d);

#endif