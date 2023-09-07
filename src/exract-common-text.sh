#!/bin/bash

function get_text {
    grep -Ev 'TEXT_[A-Z]+_' "$@" | grep -Po '(?<!_)TEXT_[A-Z0-9]+'
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
    texts=($(sort <(list_array "${texts[@]}") <(get_text "$f")))
done

declare -ar duplicated=$(list_array "${texts[@]}" | uniq -d)
declare -ar common=($(get_text text_common.h | sort))

diff -u <(list_array "${common[@]}") <(list_array "${duplicated}") | grep '^+'
