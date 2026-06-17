#!/bin/bash
# Copyright 2026 Tadashi G. Takaoka
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# scrape-z8001-syms.sh INPUT.ginc OUT.syms OUT.ld_flags TEXT_ORG
#
# Scrape Z8001 seg_SS_off_XXXX symbol references from INPUT.ginc, write a
# gas helper OUT.syms containing one per-segment section with each label
# anchored at its offset via .org, and write OUT.ld_flags containing
# --section-start=text=TEXT_ORG plus one --section-start=seg_SS=0xSS0000
# per referenced segment.  The linker resolves each reference via the
# section symbol, which is what triggers z8k-coff-ld's Z8001 long-direct
# (bit-15) encoding of r_imm32.  Pass OUT.ld_flags to z8k-coff-ld with
# @OUT.ld_flags.

set -eu

input=$1
syms_out=$2
flags_out=$3
text_org=$4

awk '
    {
        line = $0
        while (match(line, /seg_[0-9a-f][0-9a-f]_off_[0-9a-f][0-9a-f][0-9a-f][0-9a-f]/)) {
            s = substr(line, RSTART, RLENGTH)
            seg = substr(s, 5, 2)
            off = substr(s, 12, 4)
            key = seg "_" off
            if (!(key in seen)) {
                seen[key] = 1
                if (seg in offs) offs[seg] = offs[seg] " " off
                else offs[seg] = off
            }
            line = substr(line, RSTART + RLENGTH)
        }
    }
    END {
        for (seg in offs) print seg "\t" offs[seg]
    }
' "$input" | sort > "$syms_out.tmp"

: > "$syms_out"
printf -- '--section-start=.text=%s\n' "$text_org" > "$flags_out"

while IFS=$'\t' read -r seg offs_list; do
    sorted_offs=$(printf '%s\n' $offs_list | tr ' ' '\n' | sort -u)
    {
        printf '\t.section\tseg_%s\n' "$seg"
        for off in $sorted_offs; do
            printf '\t.org\t0x%s\n' "$off"
            printf '\t.global\tseg_%s_off_%s\n' "$seg" "$off"
            printf 'seg_%s_off_%s:\n' "$seg" "$off"
        done
    } >> "$syms_out"
    printf -- '--section-start=seg_%s=0x%s0000\n' "$seg" "$seg" >> "$flags_out"
done < "$syms_out.tmp"

rm -f "$syms_out.tmp"
