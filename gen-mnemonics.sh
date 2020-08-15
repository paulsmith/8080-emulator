#!/bin/bash
FILENAME=mnemonics-table.inc
echo "// generated with $(basename $0) script - do not edit manually" > $FILENAME
echo "static char *mnemonics_8080[] = {" >> $FILENAME
cat 8080-isa.txt | sed '1d' | awk -F $'\t' '{print $2}' | sed -e 's/.*/"\0"/' | \
    paste -sd',,,,,,,,,,,,,,,\n' | sed -e 's/^/    /' -e 's/$/,/' >> $FILENAME
echo "};" >> $FILENAME
