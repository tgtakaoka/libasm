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
#
# nasm round-trip check for the i8086-family autogen.  For each variant it
# assembles gen_<v>.ginc with GAS (the reference), converts it to nasm syntax
# via ginc-to-nasm.py, assembles that with nasm, and requires the two flat
# binaries to be byte-identical.  nasm is thus a third independent assembler
# (alongside libasm and GAS) for the exhaustively-generated instruction set.
#
# Run from test/autogen:  ./nasm-roundtrip.sh [variant ...]   (default: all)
set -u
REF=../reference
CONV=$REF/ginc-to-nasm.py
TMP=$(mktemp -d)
trap 'rm -rf "$TMP"' EXIT

# variant  mode  gas-mtune  sed-chain (space separated, applied left-to-right)
declare -A MODE=( [i8086]=16 [i80186]=16 [i80286]=16 [i80386]=32 [i80486]=32 )
declare -A TUNE=( [i8086]=i8086 [i80186]=i186 [i80286]=i286 [i80386]=i386 [i80486]=i486 )
declare -A SEDS=(
  [i8086]="ginc-to-nasm-i8086.sed"
  [i80186]="ginc-to-nasm-i8086.sed"
  [i80286]="ginc-to-nasm-i8086.sed"
  [i80386]="ginc-to-nasm-i8086.sed ginc-to-nasm-i80386.sed"
  [i80486]="ginc-to-nasm-i8086.sed ginc-to-nasm-i80486.sed ginc-to-nasm-i80386.sed" )

variants=${*:-i8086 i80186 i80286 i80386 i80486}
rc=0
for v in $variants; do
    mode=${MODE[$v]}; tune=${TUNE[$v]}
    # 1. GAS reference: listing (source-aligned bytes) + linked flat binary
    i386-elf-as -mtune=$tune --warn -mnaked-reg -msyntax=intel -mmnemonic=intel \
        -a=$TMP/g.lst -o $TMP/g.o gen_$v.gas 2>$TMP/gas.err
    i386-elf-ld --entry=0 -section-start=.text=0x1000 -o $TMP/g.elf $TMP/g.o 2>/dev/null
    i386-elf-objcopy -O binary $TMP/g.elf $TMP/g.bin
    # 2. sed chain
    cp gen_$v.ginc $TMP/s.txt
    for s in ${SEDS[$v]}; do sed -f $REF/$s $TMP/s.txt >$TMP/s2.txt; mv $TMP/s2.txt $TMP/s.txt; done
    # 3. convert to nasm syntax
    python3 $CONV gen_$v.ginc $TMP/s.txt $TMP/g.lst $mode $TMP/n.ninc
    # 4. nasm assemble
    { echo "[bits $mode]"; echo '[org 0x1000]'; echo 'label_abs equ 0x12345678'; cat $TMP/n.ninc; } >$TMP/n.nasm
    nasm -f bin -o $TMP/n.bin $TMP/n.nasm 2>$TMP/nasm.err
    err=$(grep -c ': error:' $TMP/nasm.err)
    if [ "$err" = 0 ] && cmp -s $TMP/n.bin $TMP/g.bin; then
        db=$(grep -c $'\tdb ' $TMP/n.ninc)
        tot=$(grep -cvE '^[[:space:]]*(;|#|$)' gen_$v.ginc)
        printf '%-8s OK  byte-identical  (nasm-verified %d/%d)\n' "$v" "$((tot-db))" "$tot"
    else
        printf '%-8s FAIL  nasm_errors=%s  byte_diffs=%s\n' "$v" "$err" \
            "$(cmp -l $TMP/n.bin $TMP/g.bin 2>/dev/null | wc -l)"
        rc=1
    fi
done
exit $rc
