#!/usr/bin/env python3
# Copyright 2024 Tadashi G. Takaoka
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

# Convert a PDP-11 ASL/libasm include file (*.inc) to a GNU-assembler include
# file (*.ginc). The .inc is the source of truth.
#
# Syntax transformations (ASL -> GAS):
#   #X           -> $X       (immediate)
#   @#X          -> *$X      (absolute; GAS rejects @#X for 2-op src)
#   *+N , *-N    -> .+N , .-N
#   @*+N , @*-N  -> *.+N , *.-N
#   .radix 10/8  -> tracked in state, dropped from output, numeric literals
#                   re-emitted with a leading 0 when current radix is 8.
#
# GAS / binutils bug workarounds (encoded as raw .word so the binary matches
# the canonical ASL/libasm output):
#   TSTSET (Rn) / WRTLCK (Rn) - GAS opcode table off-by-one
#   FP-11 immediate operands  - GAS truncates to one 16-bit word
#   32-bit integer immediate  - GAS truncates LDCLD/LDCLF imm to one word
# These are flagged by inline `;# GAS bug: ...` comments in the .ginc so the
# workarounds are easy to remove once binutils is fixed.

import re
import sys
from fractions import Fraction


NUM_RE = re.compile(r'[+-]?[0-9][0-9.]*')


def reradix(token, radix):
    if '.' in token:
        return token
    sign = ''
    if token[0] in '+-':
        sign, token = token[0], token[1:]
    if token.startswith('0x') or token.startswith('0X'):
        return sign + token
    if radix == 8 and not token.startswith('0'):
        return sign + '0' + token
    return sign + token


def parse_int(literal, radix):
    """Parse a numeric literal honoring the current ASL radix."""
    s = literal.strip()
    sign = 1
    if s.startswith('+'):
        s = s[1:]
    elif s.startswith('-'):
        sign = -1; s = s[1:]
    if s.startswith(('0x', '0X')):
        return sign * int(s, 16)
    if s.startswith('0') and len(s) > 1:
        return sign * int(s, 8)
    if radix == 8:
        return sign * int(s, 8)
    return sign * int(s, 10)


def fp11_encode(value, total_bits):
    """Encode a value as FP-11 single (total_bits=32) or double (total_bits=64).

    FP-11 normalizes mantissa to [0.5, 1) with a hidden bit representing 0.5
    (i.e. mantissa = 0.5 + frac/2^(frac_bits+1)). Bias is 128. Layout is
    sign(1) | exp(8) | frac(23 or 55), stored big-endian-by-word.
    """
    f = Fraction(value)
    sign = 0
    if f < 0:
        sign = 1
        f = -f
    nwords = total_bits // 16
    if f == 0:
        return [0] * nwords
    exp = 0
    while f >= 1:
        f /= 2
        exp += 1
    while f < Fraction(1, 2):
        f *= 2
        exp -= 1
    frac_bits = total_bits - 9
    diff = f - Fraction(1, 2)
    scale = 1 << (frac_bits + 1)
    num = diff.numerator * scale
    den = diff.denominator
    q, r = divmod(num, den)
    if 2 * r > den or (2 * r == den and q & 1):
        q += 1
    if q >> frac_bits:
        q = 0
        exp += 1
    biased = exp + 128
    if not 0 < biased < 256:
        raise ValueError(f'FP-11 exponent out of range for {value!r}')
    bits = (sign << (total_bits - 1)) | (biased << frac_bits) | q
    return [(bits >> ((nwords - 1 - i) * 16)) & 0xFFFF for i in range(nwords)]


def fp11_single(value):
    return fp11_encode(value, 32)


def fp11_double(value):
    return fp11_encode(value, 64)


# PDP-11 FP-11 instruction opcode high bytes. The low byte is
# (AC << 6) | src/dst mode_reg.
FP_OPCODES = {
    'MULD': 0xF1, 'MULF': 0xF1,
    'MODD': 0xF3, 'MODF': 0xF3,
    'ADDD': 0xF4, 'ADDF': 0xF4,
    'LDD':  0xF5, 'LDF':  0xF5,
    'SUBD': 0xF6, 'SUBF': 0xF6,
    'CMPD': 0xF7, 'CMPF': 0xF7,
    'DIVD': 0xF9, 'DIVF': 0xF9,
    'LDCID': 0xFE, 'LDCLD': 0xFE, 'LDCIF': 0xFE, 'LDCLF': 0xFE,
    'LDCFD': 0xFF, 'LDCDF': 0xFF,
}

# Source mnemonics whose immediate operand is FP-11 double (4 words).
FP_DOUBLE = {'ADDD', 'SUBD', 'MULD', 'DIVD', 'CMPD', 'MODD', 'LDD', 'LDCDF'}
# Source mnemonics whose immediate operand is FP-11 single (2 words).
FP_SINGLE = {'ADDF', 'SUBF', 'MULF', 'DIVF', 'CMPF', 'MODF', 'LDF', 'LDCFD'}
# Source mnemonics whose immediate operand is 32-bit integer (2 words).
INT_LONG = {'LDCLD', 'LDCLF'}


def ac_reg(operand):
    """Return AC index 0-3 if operand is ACn, else None."""
    m = re.match(r'(?i)^AC([0-3])$', operand.strip())
    return int(m.group(1)) if m else None


def convert_operand(operand, radix, fp_kind=None):
    parts = [s.strip() for s in operand.split(',')]
    out = []
    for part in parts:
        if part.startswith('@*+'):
            part = '*.+' + reradix(part[3:], radix)
        elif part.startswith('@*-'):
            part = '*.-' + reradix(part[3:], radix)
        elif part.startswith('*+'):
            part = '.+' + reradix(part[2:], radix)
        elif part.startswith('*-'):
            part = '.-' + reradix(part[2:], radix)
        elif part.startswith('@#'):
            part = '*$' + reradix(part[2:], radix)
        elif part.startswith('#'):
            body = part[1:]
            if fp_kind:
                sign = ''
                if body and body[0] in '+-':
                    sign, body = body[0], body[1:]
                if '.' not in body:
                    body = body + '.0'
                part = f'${sign}0{fp_kind}{body}'
            else:
                part = '$' + reradix(body, radix)
        else:
            m = re.match(r'^(@?)([+-]?[0-9][0-9.]*)(.*)$', part)
            if m:
                prefix, num, rest = m.group(1), m.group(2), m.group(3)
                part = prefix + reradix(num, radix) + rest
        out.append(part)
    return ', '.join(out)


def emit_word_replacement(line, indent, mnem, operand, radix):
    """Detect GAS/binutils-bug instructions and return a .word-based
    workaround (preserving the original source text in a comment), or
    None if the instruction is fine to assemble normally.
    """
    upper = mnem.upper()
    parts = [s.strip() for s in operand.split(',')]

    # Bug #1: TSTSET / WRTLCK opcode off-by-one in tc-pdp11.c.
    if upper in ('TSTSET', 'WRTLCK') and len(parts) == 1:
        m = re.match(r'(?i)^\(R([0-7])\)$', parts[0])
        if m:
            reg = int(m.group(1))
            base = 0x0E88 if upper == 'TSTSET' else 0x0EC8  # opcode | mode=1
            word = base | reg
            return (
                f';# GAS bug: {upper} opcode encoded as 007{1 if upper == "TSTSET" else 2}NN'
                f' instead of 007{2 if upper == "TSTSET" else 3}NN. Use .word.\n'
                f';#{indent[1:]}{mnem}    {operand}\n'
                f'{indent}.word   0x{word:04X}\n')

    # Bug #2/#3: FP-11 immediate truncated to 1 word; 32-bit int LDCLD/LDCLF
    # immediate truncated to 1 word. In both cases we emit the entire
    # instruction as .word so the encoding matches ASL/libasm.
    if len(parts) == 2 and parts[0].startswith('#') and parts[1].upper().startswith('AC'):
        ac = ac_reg(parts[1])
        if ac is None or upper not in FP_OPCODES:
            return None
        # Source addressing for "#N" is mode 2 reg 7 (autoincrement PC).
        src = 0o27  # mode 2 reg 7
        opcode = (FP_OPCODES[upper] << 8) | (ac << 6) | src
        literal = parts[0][1:]
        try:
            if upper in FP_DOUBLE:
                data = fp11_double(literal)
                bug = 'FP-11 double immediate truncated to 1 word (4 expected)'
            elif upper in FP_SINGLE:
                data = fp11_single(literal)
                bug = 'FP-11 single immediate truncated to 1 word (2 expected)'
            elif upper in INT_LONG:
                v = parse_int(literal, radix) & 0xFFFFFFFF
                data = [(v >> 16) & 0xFFFF, v & 0xFFFF]
                bug = '32-bit integer immediate truncated to 1 word (2 expected)'
            else:
                return None
        except (ValueError, ZeroDivisionError):
            return None
        words = ', '.join(f'0x{w:04X}' for w in [opcode] + data)
        return (
            f';# GAS bug: {bug}.\n'
            f';#{indent[1:]}{mnem}    {operand}\n'
            f'{indent}.word   {words}\n')

    return None


def convert_line(line, radix):
    stripped = line.strip()
    if not stripped:
        return line, radix
    if stripped.startswith(';'):
        return line, radix
    m = re.match(r'^\s*\.?radix\s+(\d+)\s*(;.*)?$', line, re.IGNORECASE)
    if m:
        radix = int(m.group(1))
        return f';# .radix {radix}\n', radix

    code, comment = line, ''
    if ';' in line:
        i = line.index(';')
        code, comment = line[:i].rstrip(), line[i:]

    m = re.match(r'^(\s*)(\S+)(\s+)(.*)$', code)
    if not m:
        return line, radix
    indent, mnem, sep, operand = m.group(1), m.group(2), m.group(3), m.group(4)
    operand = operand.rstrip()

    workaround = emit_word_replacement(line, indent, mnem, operand, radix)
    if workaround:
        return workaround, radix

    upper = mnem.upper()
    fp_kind = None
    if upper in FP_DOUBLE:
        fp_kind = 'd'
    elif upper in FP_SINGLE:
        fp_kind = 'f'
    operand = convert_operand(operand, radix, fp_kind)
    new = f'{indent}{mnem}{sep}{operand}'
    if comment:
        new = f'{new}\t{comment}'
    if not new.endswith('\n'):
        new += '\n'
    return new, radix


def convert(text):
    radix = 8
    out = []
    for line in text.splitlines(keepends=True):
        new, radix = convert_line(line, radix)
        out.append(new)
    return ''.join(out)


def main():
    if len(sys.argv) != 3:
        sys.stderr.write(
            'usage: pdp11-inc-to-ginc.py <input.inc> <output.ginc>\n')
        sys.exit(2)
    with open(sys.argv[1], 'r') as f:
        text = f.read()
    out = convert(text)
    with open(sys.argv[2], 'w') as f:
        f.write(out)


if __name__ == '__main__':
    main()
