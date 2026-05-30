# Base i8086-family .ginc → nasm transform: converts GAS/libasm Intel
# syntax into nasm's stricter dialect. Read by test/Makefile.nasm to
# produce .ninc build artifacts. CPU- and mode-specific overrides live in
# ginc-to-nasm-i80386.sed (32-bit / .code32) and ginc-to-nasm-i80486.sed.

# nasm uses 'tword' for 10-byte FPU extended precision (libasm/GAS use 'tbyte')
s/\btbyte\b/tword/g

# Memory size keyword: drop 'ptr' (byte ptr -> byte, etc.)
s/\b\(byte\|word\|dword\|qword\|tword\) ptr\b/\1/g

# String operations with explicit operands - nasm doesn't accept them.
# Covers movs/cmps/scas/lods/stos with [seg:reg] / [reg] forms,
# and ins/outs with mixed register + memory forms.
s/^\([[:space:]]\+\)\(movs\|cmps\|scas\|lods\|stos\)\(b\|w\|d\)[[:space:]]\+[a-z]*:\?\[[^]]*\]\(,[[:space:]]*[a-z]*:\?\[[^]]*\]\)\?[[:space:]]*$/\1\2\3/
s/^\([[:space:]]\+\)\(ins\|outs\)\(b\|w\|d\)[[:space:]]\+.*$/\1\2\3/

# Standalone prefix mnemonics: libasm treats them as instructions emitting
# the prefix byte; nasm requires them to attach to a real instruction, so
# emit the raw byte via 'db'.
s/^\([[:space:]]\+\)lock[[:space:]]*$/\1db 0xf0/
s/^\([[:space:]]\+\)rep[[:space:]]*$/\1db 0xf3/
s/^\([[:space:]]\+\)repe[[:space:]]*$/\1db 0xf3/
s/^\([[:space:]]\+\)repz[[:space:]]*$/\1db 0xf3/
s/^\([[:space:]]\+\)repne[[:space:]]*$/\1db 0xf2/
s/^\([[:space:]]\+\)repnz[[:space:]]*$/\1db 0xf2/

# jmp/call to memory through segment override -> nasm needs explicit 'near' +
# segment moved inside the brackets.
s/^\([[:space:]]\+\)\(jmp\|call\)[[:space:]]\+\([a-z]\+\):\[\([^]]\+\)\][[:space:]]*$/\1\2 near [\3:\4]/

# Far call/jmp immediate seg:off (lcall/ljmp use comma, nasm uses 'far' + colon).
s/^\([[:space:]]\+\)l\(call\|jmp\)[[:space:]]\+\(0x[0-9a-fA-F]\+\|[0-9]\+\),[[:space:]]*\(0x[0-9a-fA-F]\+\|[0-9]\+\)[[:space:]]*$/\1\2 far \3:\4/

# Far call/jmp through memory pointer (lcall [mem] / ljmp [mem]).
s/^\([[:space:]]\+\)l\(call\|jmp\)[[:space:]]\+\(\[.*\]\)[[:space:]]*$/\1\2 far \3/

# Far return: lret -> retf
s/^\([[:space:]]\+\)lret[[:space:]]*$/\1retf/
s/^\([[:space:]]\+\)lret[[:space:]]\+\(.*\)$/\1retf \2/

# FPU stack register: st(N) -> stN ; standalone 'st' -> 'st0'
s/st(\([0-9]\))/st\1/g
s/\([[:space:],]\)st\([[:space:],]\|$\)/\1st0\2/g
s/\([[:space:],]\)st\([[:space:],]\|$\)/\1st0\2/g

# INT 3 -> int3 (nasm encodes 'int 3' as CD 03 but has 'int3' for the 1-byte CC form)
s/^\([[:space:]]\+\)int[[:space:]]\+3[[:space:]]*$/\1int3/

# FPU mnemonic name differences (libasm/GAS use fnsetpm, nasm uses fsetpm)
s/^\([[:space:]]\+\)fnsetpm[[:space:]]*$/\1fsetpm/

# Strip redundant segment overrides matching the addressing-mode default
# (DS for [bx]/[si]/[di]/[disp], SS for [bp]/[sp]/[ebp]/[esp]).
s/\bds:\[\(bx\)\b/[\1/g
s/\bds:\[\(si\)\b/[\1/g
s/\bds:\[\(di\)\b/[\1/g
s/\bds:\[0x/[0x/g
s/\bss:\[\(bp\)\b/[\1/g
s/\bss:\[\(sp\)\b/[\1/g
s/\bss:\[\(ebp\)\b/[\1/g
s/\bss:\[\(esp\)\b/[\1/g
s/\[ds:bx\b/[bx/g
s/\[ds:si\b/[si/g
s/\[ds:di\b/[di/g
s/\[ss:bp\b/[bp/g
s/\[ss:sp\b/[sp/g

# Swap commutative xchg reg,reg operand order so nasm picks the same
# ModRM encoding as GAS (operand 1 in the reg field).
s/^\([[:space:]]\+\)xchg\([[:space:]]\+\)\(ax\|bx\|cx\|dx\|si\|di\|bp\|sp\|al\|bl\|cl\|dl\|ah\|bh\|ch\|dh\),[[:space:]]*\(ax\|bx\|cx\|dx\|si\|di\|bp\|sp\|al\|bl\|cl\|dl\|ah\|bh\|ch\|dh\)[[:space:]]*$/\1xchg\2\4, \3/

# Force long-form movsx ax,al (nasm optimizes the 0F BE C0 form to CBW)
s/^\([[:space:]]\+\)movsx[[:space:]]\+ax[[:space:]]*,[[:space:]]*al[[:space:]]*$/\1db 0x0f,0xbe,0xc0 ; movsx ax,al/

# i80386 D-suffix imm push: pushd N -> push dword N
s/^\([[:space:]]\+\)pushd[[:space:]]\+/\1push dword /

# Section header `### ...` becomes `;### ...` (GAS treats # as comment)
s/^###/;###/
