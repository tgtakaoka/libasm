# i486-specific .ginc transform, applied AFTER ginc-to-nasm-i8086.sed for
# the real_i80486 and (with ginc-to-nasm-i80386.sed) i80486 nasm targets.

# GAS emits operand-size (66) BEFORE lock (F0) for these 32-bit LOCKed ops;
# nasm emits LOCK first. Encode raw to match GAS prefix order.
s/^\([[:space:]]\+\)lock[[:space:]]\+xadd[[:space:]]\+dword[[:space:]]\+\[bx+si\][[:space:]]*,[[:space:]]*ecx[[:space:]]*$/\1db 0x66,0xf0,0x0f,0xc1,0x08 ; lock xadd [bx+si],ecx/
s/^\([[:space:]]\+\)lock[[:space:]]\+cmpxchg[[:space:]]\+dword[[:space:]]\+\[bx+si\][[:space:]]*,[[:space:]]*ecx[[:space:]]*$/\1db 0x66,0xf0,0x0f,0xb1,0x08 ; lock cmpxchg [bx+si],ecx/
