#!/bin/bash
set -ex -o pipefail
CC=${CC-clang}

# generate table includes
./gen-mnemonics.sh
./gen-sizes.sh
./gen-cycles.sh

# build main program
$CC -Wall -Wextra -pedantic -O0 -g -std=c99 -o 8080 8080.c
