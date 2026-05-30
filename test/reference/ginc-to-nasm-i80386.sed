# 32-bit-mode .ginc transform, applied AFTER ginc-to-nasm-i8086.sed only
# for the test_i80386 / test_i80486 (32-bit / .code32) nasm targets.
#
# In [bits 32] mode nasm picks 16-bit operand size for memory-only operands
# whose addressing register is 16-bit (e.g. `pop [si]` → `pop word`); GAS
# in .code32 uses dword as the default. Add an explicit `dword` to keep the
# bytes in sync with GAS.
s/^\([[:space:]]\+\)push[[:space:]]\+\(\[\)/\1push dword \2/
s/^\([[:space:]]\+\)pop[[:space:]]\+\(\[\)/\1pop dword \2/

# String operations: the GAS source treats them as 16-bit-addressed in
# .code32; nasm uses 32-bit addressing by default. Prepend the `a16`
# address-size override so nasm produces matching bytes.
s/^\([[:space:]]\+\)\(movsb\|movsw\|movsd\)$/\1a16 \2/
s/^\([[:space:]]\+\)\(cmpsb\|cmpsw\|cmpsd\)$/\1a16 \2/
s/^\([[:space:]]\+\)\(scasb\|scasw\|scasd\)$/\1a16 \2/
s/^\([[:space:]]\+\)\(lodsb\|lodsw\|lodsd\)$/\1a16 \2/
s/^\([[:space:]]\+\)\(stosb\|stosw\|stosd\)$/\1a16 \2/
s/^\([[:space:]]\+\)\(insb\|insw\|insd\)$/\1a16 \2/
s/^\([[:space:]]\+\)\(outsb\|outsw\|outsd\)$/\1a16 \2/

# In 32-bit mode nasm folds `movsx ax,al` to `66 98` (DATA32 + cbw); GAS
# in .code32 keeps the full 4-byte form 66 0F BE C0. The main sed already
# inserted a 3-byte `db` override (assuming 16-bit mode); prepend the 66
# operand-size prefix here so the bytes match in 32-bit mode.
s/db 0x0f,0xbe,0xc0/db 0x66,0x0f,0xbe,0xc0/

# The i486-specific sed rewrites two LOCK ops to a raw byte sequence
# assuming 16-bit mode (66 F0 ...). In 32-bit mode the 66 prefix is
# wrong (ECX is 32-bit native) and a 67 addr-override is needed for
# the 16-bit [bx+si] form. GAS emits the size prefix BEFORE LOCK.
s/db 0x66,0xf0,0x0f,0xc1,0x08 ; lock xadd \[bx+si\],ecx/db 0x67,0xf0,0x0f,0xc1,0x08 ; lock xadd [bx+si],ecx/
s/db 0x66,0xf0,0x0f,0xb1,0x08 ; lock cmpxchg \[bx+si\],ecx/db 0x67,0xf0,0x0f,0xb1,0x08 ; lock cmpxchg [bx+si],ecx/

# 32-bit mode: GAS emits 66 BEFORE F0 (lock) for `lock xadd/cmpxchg
# word [bx+si],cx`; nasm emits F0 then 66. Force GAS prefix order via db.
s/^\([[:space:]]\+\)lock[[:space:]]\+xadd[[:space:]]\+word[[:space:]]\+\[bx+si\][[:space:]]*,[[:space:]]*cx[[:space:]]*$/\1db 0x67,0x66,0xf0,0x0f,0xc1,0x08 ; lock xadd word [bx+si],cx/
s/^\([[:space:]]\+\)lock[[:space:]]\+cmpxchg[[:space:]]\+word[[:space:]]\+\[bx+si\][[:space:]]*,[[:space:]]*cx[[:space:]]*$/\1db 0x67,0x66,0xf0,0x0f,0xb1,0x08 ; lock cmpxchg word [bx+si],cx/

# SMSW r16 in 32-bit mode: nasm doesn't add the 66 operand-size prefix
# (treats `smsw cx` the same as `smsw ecx`); GAS does. Force the prefix
# via the explicit `o16` keyword.
s/^\([[:space:]]\+\)smsw[[:space:]]\+\(ax\|bx\|cx\|dx\|si\|di\|bp\|sp\)[[:space:]]*$/\1o16 smsw \2/
