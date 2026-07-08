#!/usr/bin/env python3
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
# Convert an autogen .ginc (GAS/libasm Intel syntax) into a nasm-assemblable
# .ninc that round-trips byte-identically to the GAS reference. Real nasm
# syntax is used wherever possible so that nasm INDEPENDENTLY reproduces the
# bytes. A small, fixed set of forms where nasm's encoder or prefix ordering
# inherently differs from GAS/libasm are emitted as `db` of the GAS bytes
# (still GAS-verified); these are the documented divergence categories:
#
#   * LOCK + operand-size prefix: nasm emits F0 before 66; GAS/libasm emit
#     66 before F0.  (~6% of the 32-bit variants; nasm has no prefix-order
#     control.)  Detected by the "66F0" signature in the GAS bytes.
#   * FPU st,st(i) two-register add/sub/mul/div: nasm picks the DC encoding,
#     GAS/libasm the D8.
#   * FSETPM: nasm uses the no-wait DB E4; GAS/libasm the wait 9B DB E4.
#   * MOVSX ax,al / eax,ax: nasm folds to CBW / CWDE.
#   * string ops with an explicit non-default segment override, and rep-
#     prefixed string ops with operands: nasm strips/rejects the operands.
#   * data16/addr16 prefixed relative branches, xlat with an operand.
#
# Anything NOT in these categories must match nasm byte-for-byte, so a new
# libasm encoding regression surfaces as a byte mismatch instead of being
# silently masked.
#
# usage: ginc-to-nasm.py <in.ginc> <sed-transformed> <gas-listing> <16|32> <out.ninc>
import re, sys

def build_bytemap(listing):
    """.ginc line number -> uppercase hex bytes, from a GAS -a listing."""
    m = {}
    cur = None
    for ln in open(listing):
        ln = ln.rstrip('\n')
        if '\t' in ln:                      # main: <line#> [<addr> <bytes>] \t <source>
            head = ln.split('\t', 1)[0]
            mt = re.match(r'\s*(\d+)\s+[0-9a-f]+\s+([0-9A-F]+)\s*$', head)
            if mt:
                cur = int(mt.group(1)); m[cur] = m.get(cur, '') + mt.group(2)
            else:
                mt2 = re.match(r'\s*(\d+)\s', head)
                cur = int(mt2.group(1)) if mt2 else cur
        else:                               # continuation: <line#> <bytes>
            mt = re.match(r'\s*(\d+)\s+([0-9A-F]+)\s*$', ln)
            if mt and cur is not None and int(mt.group(1)) == cur:
                m[cur] += mt.group(2)
    return m

RE_FORM1  = re.compile(r'^(\s*)l(call|jmp)\s+(\w+):\[([^\]]+)\]\s*$')
RE_PREFIX = re.compile(r'^\s*(data16|data32|addr16|addr32)\s')
RE_REPSTR = re.compile(r'^\s*(rep|repe|repz|repne|repnz)\s+(movs|cmps|scas|lods|stos|ins|outs)[bwd]\b')
RE_XLAT   = re.compile(r'^\s*xlat\s+\[')
RE_STRSEG = re.compile(r'^\s*(movs|cmps|scas|lods|stos|ins|outs)[bwd]\s+.*\b(es|cs|ss|ds|fs|gs):\[')
RE_FPUDIR = re.compile(r'^\s*f(add|sub|subr|mul|div|divr)\s+st\b.*\bst\b')
RE_MOVSXF = re.compile(r'^\s*movsx\s+(ax\s*,\s*al|eax\s*,\s*ax)\s*$')
RE_FSETPM = re.compile(r'^\s*fsetpm\b')
RE_SEGIN  = re.compile(r'\b(es|cs|ss|ds|fs|gs):(\[[^\]]*\])')
RE_NOSPLIT = re.compile(r'\[((?:\w\w:)?\w+\*2\b)')
R32 = ('eax', 'ecx', 'edx', 'ebx', 'esp', 'ebp', 'esi', 'edi')
RE_XCHG32 = re.compile(r'^(\s*)xchg(\s+)(%s)\s*,\s*(%s)\s*$' % ('|'.join(R32), '|'.join(R32)))

def needs_db(st, gb):
    if RE_PREFIX.match(st) or RE_REPSTR.match(st) or RE_XLAT.match(st): return True
    if RE_STRSEG.match(st) or RE_FPUDIR.match(st) or RE_MOVSXF.match(st) or RE_FSETPM.match(st): return True
    if st.startswith('lock') and '66F0' in gb: return True  # nasm orders F0 before 66
    return False

def main():
    ginc, sed_out, listing, mode, out_path = sys.argv[1:6]
    mode = int(mode)
    gm = build_bytemap(listing)
    gl = open(ginc).read().splitlines()
    sl = open(sed_out).read().splitlines()
    assert len(gl) == len(sl), (len(gl), len(sl))

    def nosplit(s):
        return RE_NOSPLIT.sub(r'[nosplit \1', s) if mode == 32 else s

    def db_line(n):
        b = gm[n]
        return "\tdb " + ",".join("0x" + b[i:i+2] for i in range(0, len(b), 2))

    out = []
    for i in range(len(gl)):
        n = i + 1
        g = gl[i].strip()
        s = sl[i]
        if not g or g[0] in ';#':
            out.append(s); continue
        if needs_db(g, gm.get(n, '')):
            out.append(db_line(n)); continue
        m = RE_FORM1.match(gl[i])
        if m:
            out.append(nosplit("%s%s far [%s:%s]" % (m.group(1), m.group(2), m.group(3), m.group(4))))
            continue
        mx = RE_XCHG32.match(s)
        if mx:
            out.append("%sxchg%s%s, %s" % (mx.group(1), mx.group(2), mx.group(4), mx.group(3)))
            continue
        s = RE_SEGIN.sub(lambda mm: "[%s:%s" % (mm.group(1), mm.group(2)[1:]), s)
        out.append(nosplit(s))
    open(out_path, 'w').write("\n".join(out) + "\n")

if __name__ == '__main__':
    main()
