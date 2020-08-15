#!/bin/bash
FILENAME=cycles-table.inc
echo "// generated with $(basename $0) script - do not edit manually" > $FILENAME
echo "static uint8_t inst_cycles_8080[] = {" >> $FILENAME
cat 8080-isa.txt | sed '1d' | awk -F $'\t' '{print $4}' | sed -e 's/.*/\0/' | \
    paste -sd',,,,,,,,,,,,,,,\n' | sed -e 's/^/    /' -e 's/$/,/' >> $FILENAME
echo "};" >> $FILENAME
