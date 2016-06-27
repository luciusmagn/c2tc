# c2tc (C2 Tiny Compiler)
A tiny compiler to compile C2 code. C2 is a language invented and its first and main compiler written by [bvdberg](https://github.com/bvdberg). The compiler can be found at [c2lang/c2compiler](https://github.com/c2lang/c2compiler/).

##Libraries in use

- thlorenz/log.h - Minimal yet colorful logging lib
- jwerle/throw.h - Create and throw errors easily
- orangeduck/mpc - Parser Combinator Library for C
- littlstar/asprintf.c - asprintf() implementation

## Waypoints
- [X] Parse recipe files
- [ ] Process recipes
- [X] Parse C2
- [ ] Parse each C2 file
- [ ] Command-line arguments
- [ ] Recipe options
- [ ] First C2 pass - throw out unneeded tokens
- [ ] Second C2 pass - overview pass
- [ ] Third C2 pass - analysis pass
- [ ] Fourth C2 pass - output pass
- [ ] .h -> .c2i
- [ ] .c2i -> .h
- [ ] tcc tie-in
- [ ] Parse manifest file and c2libs