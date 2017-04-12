/*
MIT/X Consortium License

© 2013-2014 Anselm R Garbe <anselm@garbe.us>

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

/*
 * Copy me if you can.
 * by 20h
 */

#ifndef ARG_H__
#define ARG_H__

static char *argv0;

/* use main(int argc, char *argv[]) */
#define ARGBEGIN    for (argv0 = *argv, argv++, argc--;\
                    argv[0] && option(&wanted, argv, &argc);\
                    argc--, argv++) {\
                if(argv[0][1] == '-') continue;\
                if(!(argv[0][0] == '-' && argv[0][1])) continue; \
                char argc_;\
                char **argv_;\
                int brk_;\
                if (argv[0][1] == '-' && argv[0][2] == '\0') {\
                    argv++;\
                    argc--;\
                    break;\
                }\
                for (brk_ = 0, argv[0]++, argv_ = argv;\
                        argv[0][0] && !brk_;\
                        argv[0]++) {\
                    if (argv_ != argv)\
                        break;\
                    argc_ = argv[0][0];\
                    switch (argc_)

#define ARGEND            }\
            }

#define ARGC()        argc_

#define EARGF(x)    ((argv[0][1] == '\0' && argv[1] == NULL)?\
                ((x), abort(), (char *)0) :\
                (brk_ = 1, (argv[0][1] != '\0')?\
                    (&argv[0][1]) :\
                    (argc--, argv++, argv[0])))

#define ARGF()        ((argv[0][1] == '\0' && argv[1] == NULL)?\
                (char *)0 :\
                (brk_ = 1, (argv[0][1] != '\0')?\
                    (&argv[0][1]) :\
                    (argc--, argv++, argv[0])))
#define FLAG(c, action) case c: action; break;
#define START_OPTION(name, action) if(strcmp(argv[0], name) == 0) { action; }
#define OPTION(name, action) else if(strcmp(argv[0], name) == 0) { action; }
#define PARAMETER(name, action) else if(strcmp(argv[0], name) == 0 && argv[1]) { action; }

#endif
