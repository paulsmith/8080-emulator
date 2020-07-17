#!/bin/sh
set -e -x
CC=${CC-clang}
$CC -Wall -Wextra -pedantic -O0 -g -std=c99 -o 8080 8080.c
