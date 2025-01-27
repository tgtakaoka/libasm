#!/bin/bash

function get_text {
    grep -hPo '(?<=^extern const char )TEXT_[\w_]+' "$@"
}

function list_files {
    for f in $(grep -lPow '(?<=^extern const char )'"$1" text_*.h); do
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
    printf "%-7s" "${t/TEXT_/}"
    list_files "$t"
done
