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

# scrape-am29000-syms.sh INPUT.ginc OUT.ld_flags TEXT_ORG
#
# Scrape A_XXXXXXXX branch target symbols from INPUT.ginc and write a gas
# linker @file OUT.ld_flags containing --section-start=.text=TEXT_ORG plus one
# --defsym per referenced symbol.
#
# The absolute form of a branch names its target rather than spelling it out,
# because a29k-coff-as reads the 16-bit word address of that form as signed and
# rejects a target over 0x1FFFC, where the manual zero-extends it.  Left to the
# linker the whole range resolves.  Pass OUT.ld_flags to a29k-coff-ld with
# @OUT.ld_flags.

set -eu

input=$1
flags_out=$2
text_org=$3

{
    printf -- '--section-start=.text=%s\n' "$text_org"
    grep -oE 'A_[0-9a-fA-F]{8}' "$input" | sort -u | while read -r sym; do
        printf -- '--defsym %s=0x%s\n' "$sym" "${sym#A_}"
    done
} > "$flags_out"
