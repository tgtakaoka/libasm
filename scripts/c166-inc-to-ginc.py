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

# Convert a C166 reference include file (*.inc) to a GNU-assembler include
# file (*.ginc). The .inc is the source of truth.
#
# Each line is assembled by libasm through the harness that includes it, and
# its bytes are disassembled on their own with the gnu-as option. A line GNU
# as cannot write -- a MAC instruction, an EXTR/EXTPR/EXTSR sequence, an
# unnamed SFR, a bit of the bit-addressable RAM -- becomes .byte, with the
# source line as a comment. A branch to a label keeps the label. The prefix
# of an ESFR sequence is .byte too, so that no assembler opens the sequence.
#
# usage: c166-inc-to-ginc.py <harness.asm> <cpu> <input.inc> <output.ginc>

import os
import re
import subprocess
import sys
import tempfile

CLI = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'cli')
LISTED = re.compile(r'^\(1\)\s+([0-9A-F]+) :(?: |$)((?:[0-9A-F]{2} )*)')
DECODED = re.compile(r'^\s*[0-9A-F]+ : ((?:[0-9A-F]{2} )+)\s*(\S+)\s*(.*)$')
OPERANDS = re.compile(r',(?![^\[]*\])')


def assemble(harness, inc, work):
    """Pair each line of |inc| with its address and bytes."""
    lst = os.path.join(work, 'harness.lst')
    subprocess.run([os.path.join(CLI, 'asm'), '-l', lst, '-o', os.path.join(work, 'harness.hex'),
                    os.path.basename(harness)],
                   cwd=os.path.dirname(os.path.abspath(harness)), check=True, capture_output=True)
    src = open(inc).read().rstrip('\n').split('\n')
    listing = open(lst).read().split('\n')
    name = re.escape(os.path.basename(inc))
    j = [i for i, l in enumerate(listing) if re.search(r'INCLUDE\s+"%s"' % name, l, re.I)][0] + 1
    col = len(listing[j]) - len(src[0])
    rows = []
    for s in src:
        while not (listing[j].startswith('(1)') and listing[j][col:].rstrip() == s.rstrip()):
            j += 1
        m = LISTED.match(listing[j])
        rows.append((s, int(m.group(1), 16), bytes.fromhex(m.group(2).replace(' ', ''))))
        j += 1
    return rows


def ihex(addr, data):
    def record(type, offset, payload):
        r = bytes([len(payload), (offset >> 8) & 0xFF, offset & 0xFF, type]) + payload
        return ':' + r.hex().upper() + '%02X' % ((-sum(r)) & 0xFF)
    return '\n'.join([record(4, 0, bytes([(addr >> 24) & 0xFF, (addr >> 16) & 0xFF])),
                      record(0, addr & 0xFFFF, data), ':00000001FF']) + '\n'


def disassemble(cpu, addr, data, work):
    """The gnu-as spelling of |data|, or None where GNU as cannot write it."""
    hex = os.path.join(work, 'line.hex')
    lst = os.path.join(work, 'line.lst')
    open(hex, 'w').write(ihex(addr, data))
    r = subprocess.run([os.path.join(CLI, 'dis'), '-C', cpu, '--gnu-as=on', '--relative=on',
                        '--origin-char=.', '-l', lst, '-o', os.path.join(work, 'line.dis'), hex],
                       capture_output=True, text=True)
    lines = [l for l in open(lst).read().split('\n') if re.match(r'^\s*[0-9A-F]+ : [0-9A-F]{2} ', l)]
    if r.returncode != 0 or 'error' in r.stdout + r.stderr or len(lines) != 1:
        return None
    m = DECODED.match(lines[0])
    if bytes.fromhex(m.group(1).replace(' ', '')) != data:
        return None
    return m.group(2), m.group(3).strip()


def convert(harness, cpu, inc, work):
    rows = assemble(harness, inc, work)
    labels = {}
    for s, _, _ in rows:
        m = re.match(r'^([A-Za-z_][A-Za-z0-9_]*):', s)
        if m:
            labels[m.group(1).upper()] = m.group(1)
    out = []
    header = True
    esfr = 0
    for s, addr, data in rows:
        if header and not s.startswith(';;;'):
            out.append(';;; Converted from %s by scripts/c166-inc-to-ginc.py; do not edit.'
                       % os.path.basename(inc))
            header = False
        if not data:
            out.append(s)
            continue
        i = s.find(';')
        code, comment = (s, '') if i < 0 else (s[:i].rstrip(), s[i:])
        m = re.match(r'^([A-Za-z_][A-Za-z0-9_]*:)?\s*(.*)$', code)
        label, insn = m.group(1) or '', m.group(2)
        mnemonic = insn.split()[0].upper()
        if mnemonic in ('EXTR', 'EXTPR', 'EXTSR'):
            decoded = None
            esfr = int(re.search(r'#\s*(\d+)\s*$', insn).group(1))
        elif esfr:
            decoded = None
            esfr -= 1
        else:
            decoded = disassemble(cpu, addr, data, work)
        if decoded is None:
            body = '.byte   ' + ', '.join('0x%02X' % b for b in data)
            comment = ('; ' + insn + ('  ' + comment if comment else '')).rstrip()
        else:
            name, operands = decoded
            ops = [o.strip() for o in OPERANDS.split(operands)] if operands else []
            srcops = [o.strip() for o in OPERANDS.split(insn[len(insn.split()[0]):])]
            if ops and srcops[-1].upper() in labels:
                ops[-1] = labels[srcops[-1].upper()]
            body = '%-8s%s' % (name, ', '.join(ops))
        line = ('%-8s%s' % (label, body)).rstrip()
        if comment:
            line = '%-40s%s' % (line, comment)
        out.append(line)
    return out


def main(argv):
    if len(argv) != 5:
        sys.stderr.write('usage: c166-inc-to-ginc.py <harness.asm> <cpu> <input.inc> <output.ginc>\n')
        return 2
    harness, cpu, inc, ginc = argv[1:]
    with tempfile.TemporaryDirectory(prefix='c166-inc-to-ginc-') as work:
        out = convert(harness, cpu, inc, work)
    open(ginc, 'w').write('\n'.join(out) + '\n')
    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv))
