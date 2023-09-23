#!/bin/bash

function get_text {
    grep -Po '(?<!__)TEXT_[\w_]+' "$@"
}

function list_array {
    local t
    for t in "$@"; do
        echo "$t"
    done
}

declare -ar text_files=($(list_array text_*.h | grep -v text_common.h))

declare -a texts=()
for f in "${text_files[@]}"; do
    texts=($(sort -s <(list_array "${texts[@]}") <(get_text "$f")))
done

declare -ar duplicated=$(list_array "${texts[@]}" | uniq -d)
declare -ar common=($(get_text text_common.h | sort -s))

diff -u <(list_array "${common[@]}") <(list_array "${duplicated[@]}") | grep '^+'
