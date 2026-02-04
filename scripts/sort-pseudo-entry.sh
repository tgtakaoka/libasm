#!/bin/bash
#
# Create name entries table.
# How to use:
# - Mark start and end of name entries
# - C-u M-x shell-command-on-region ../scripts/sort-pseudo-entry.sh

tr -d '{},' \
   | awk 'NF >= 2 { k=$1; sub(/TEXT_d/, "TEXT_@", k); print k,"/",$0 }' \
   | sort -t"@" -k1 -s \
   | cut -d"/" -f 2- \
   | awk '
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
              in_comment = 0;
              for (i = 1; i <= num_words[n]; i++) {
                  fmt=" %-"(max_len[i]+1)"s";
                  elem = lines[n, i];
                  if (i < num_words[n]) {
                      if (elem ~ /\/\// || elem ~ /\/\*/) {
                          printf("},  "elem);
                          in_comment = 1;
                      } else {
                          printf(fmt, elem",");
                      }
                  } else if (in_comment) {
                      printf("%s", elem);
                  } else {
                      printf(fmt"},", elem);
                  }
              }
              printf("\n");
          }
      }'
