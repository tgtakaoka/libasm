#!/bin/bash
#
# Create name index table from Entry table.
# - name index table holds index to main Entry page.
# - name index table is sorted ascending order of entry->name_P().
# How to use:
# - Copy main Entry table and change element type to uint8_t
# - Mark start and end of table.
# - C-u M-x shell-command-on-region ../scripts/make-index-array.sh

# check if grep support -P (PCRE)
grep -q -P x <<<"x" 2>/dev/null && PCRE=1 || PCRE=0

if [[ ${PCRE} = 1 ]]; then
    grep -Po '(TEXT_[A-Za-z0-9]+)'
else
    perl -ne 'print "$1\n" while /(TEXT_[A-Za-z0-9]+)/g'
fi |
    sed 's/TEXT_null/@@@@_null/' |
    awk '{ printf("    %3d,  // %s\n", i++, $1); }' |
    sort -k3 -s |
    sed 's/@@@@_null/TEXT_null/'
