/* -*- mode: c++; -*- */
#ifndef __FILE_UTIL__
#define __FILE_UTIL__

#include <stdio.h>
#include <stdint.h>

/*
 * Simplified version of getline() of glibc.
 * Line terminate '\n' will be eliminated.
 */
int getLine(char *&lineptr, size_t &t, FILE *input);

#endif

