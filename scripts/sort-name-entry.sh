#!/bin/bash
#
# Create name entries table.
# How to use:
# - Mark start and end of name entries
# - C-u M-x shell-command-on-region ../scripts/sort-name-entry.sh

tr -d '{},' |
    awk 'NF == 2 { print $1,$2,$1 }' |
    tr -s ' ' '\t' |
    sort -s -k3 |
    cut -f 1-2 |
    tr -d '@' |
    awk '
        {
           num_lines++;
           for (i = 1; i <= NF; i++) {
               if (max_len[i] < length($i)) max_len[i] = length($i);
               lines[num_lines, i] = $i;
           }
           num_words[num_lines] = NF;
        }
        END {
             for (n = 1; n <= num_lines; n++) {
                 printf("    {");
                 for (i = 1; i <= num_words[n]; i++) {
                     fmt=" %-"(max_len[i]+1)"s";
                     if (i < num_words[n]) {
                         printf(fmt, lines[n, i]",");
                     } else {
                         printf(fmt"},\n", lines[n, i]);
                     }
                 }
             }
        }
    '
