#!/bin/bash
#
# Create name index table from Entry table.
# - name index table holds index to main Entry page.
# - name index table is sorted ascending order of entry->name_P().
# How to use:
# - Copy main Entry table and change element type to uint8_t
# - Mark start and end of table.
# - C-u M-x shell-command-on-region ./make-index-array.sh

grep -Po '(TEXT_[A-Za-z0-9]+)' | \
    awk '{ printf("    %3d,  // %s\n", i++, $1); }' |
    sort -k3 -s
