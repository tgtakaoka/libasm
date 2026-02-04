#!/bin/bash
#
# Sort text constants.
# How to use:
# - Mark start and end of text constants.
# - C-u M-x shell-command-on-region ../scripts/sort-text-constant.sh

awk 'match($0, /(TEXT|PSTR)_[A-Za-z0-9_]+/) {
     sub(/^ +/, "");
     text = substr($0, RSTART, RLENGTH);
     print text,"@",$0;}' |
    sort -t"@" -k1 -s |
    cut -d"@" -f 2- |
    awk '
        {
           nw = num_words[++num_lines] = NF;
           for (i = 1; i <= nw; i++) {
               if (max_len[nw, i] < length($i))
                    max_len[nw, i] = length($i);
               lines[num_lines, i] = $i;
           }
        }
        END {
             for (n = 1; n <= num_lines; n++) {
                 nw = num_words[n]
                 for (i = 1; i <= nw; i++) {
                     if (i < nw) {
                         fmt="%-"(max_len[nw, i]+1)"s";
                         printf(fmt, lines[n, i]);
                     } else {
                         printf("%s\n", lines[n, i]);
                     }
                 }
             }
        }'
