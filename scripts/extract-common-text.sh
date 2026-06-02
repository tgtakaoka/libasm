#!/bin/bash
#
# Extract duplicate text constants
# How to use:
# - under src/ directory
# - ../scripts/extract-common-text.sh

# check if grep support -P (PCRE)
grep -q -P x <<<"x" 2>/dev/null && PCRE=1 || PCRE=0

function get_text {
    if [[ ${PCRE} = 1 ]]; then
        grep -hPo '(?<=^extern const char )TEXT_[\w_]+' "$@"
    else
        perl -ne 'print "$&\n" if /(?<=^extern const char )TEXT_[\w_]+/' "$@"
    fi
}

function list_files {
    local -a list=()
    if [[ ${PCRE} = 1 ]]; then
        list=($(grep -lPow '(?<=^extern const char )'"$1" text_*.h))
    else
        list=($(perl -ne 'if (/(?<=^extern const char )\Q'"$1"'\E\b/) {
               print "$ARGV\n";
               close ARGV;
         }' text_*.h))
    fi
    local f
    for f in "${list[@]}"; do
        printf "%-16s" $f
    done
    echo
}

function list_array {
    local t
    for t in "$@"; do
        echo "$t"
    done
}

declare -a texts=()
for f in text_*.h; do
    texts=($(sort -s <(list_array "${texts[@]}") <(get_text "$f")))
done

for t in $(list_array "${texts[@]}" | uniq -d); do
    printf "%-12s" "${t/TEXT_/}"
    list_files "$t"
done
