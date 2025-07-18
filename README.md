![<https://github.com/tgtakaoka/libasm/blob/main/LICENSE.md>](https://img.shields.io/badge/License-Apache%202.0-blue.svg)
![<https://github.com/tgtakaoka/libasm/releases>](https://img.shields.io/github/v/release/tgtakaoka/libasm.svg?maxAge=3600)
![badge](https://github.com/tgtakaoka/libasm/actions/workflows/ccpp.yml/badge.svg)
![<https://github.com/tgtakaoka/libasm/actions/workflows/arduino-ci.yml>](https://github.com/tgtakaoka/libasm/actions/workflows/arduino-ci.yml/badge.svg)
![<https://github.com/tgtakaoka/libasm/actions/workflows/platformio-ci.yml>](https://github.com/tgtakaoka/libasm/actions/workflows/platformio-ci.yml/badge.svg)

# libasm: cross assemble/disassemble library

The libasm allows assembling and disassembling supported retro CPUs on
a small environment (less than 9kB-17kB Flash and 350B RAM on AVR
Arduino).

## Assembler library interface

Assembling MC68HC11 instruction in string to binary can be done in a
minute. You can also add symbol table lookup via
[`SymbolTable`](https://github.com/tgtakaoka/libasm/blob/main/src/symbol_table.h)
interface.

``` C++
#include <asm_mc6800.h>
libasm::mc6800::AsmMc6800 asm6800;
asm6800.setCpu("68HC11");

const char *text = "SUBD $90";
libasm::Insn insn{0x1000};

asm6800.encode(text, insn);
assert(insn.getError() == OK);
assert(insn.address()  == 0x1000);
assert(insn.length()   == 2);    // direct addressing
assert(insn.bytes()[0] == 0x93); // SUBD
assert(insn.bytes()[1] == 0x90); // $90
```

## Disassembler library interface

To disassemble MC68000 binaries, you need to wrap memory by
[`ArrayMemory`](https://github.com/tgtakaoka/libasm/blob/main/src/array_memory.h)
or implements your own
[`DisMemory`](https://github.com/tgtakaoka/libasm/blob/main/src/dis_memory.h)
interface to feed binaries. You can also add symbol table lookup via
[`SymbolTable`](https://github.com/tgtakaoka/libasm/blob/main/src/symbol_table.h)
interface.

``` C++
#include <dis_mc68000.h>
libasm::mc68000::DisMc68000 dis68k;
dis68k.setOption("relative", "true");

const uint16_t words[] = {0047372, 0x1232};
const libasm::ArrayMemory memory{0x1000, words, sizeof(words)};
libasm::DisMemory dismem = memory.iterator();
libasm::Insn insn{dismem.address()};
char opr[80];

dis68k.decode(dismem, insn, opr, sizeof(opr));
assert(insn.getError() == OK);
assert(insn.address()  == 0x1000);
assert(std::string(insn.name()) == "jmp");
assert(std::string(opr) == "(*+$1234,pc)");
```

## Arduino sketch examples

There are assembler and disassembler example sketches you can try to
run on actual Arduino board at
[examples](https://github.com/tgtakaoka/libasm/tree/devel/examples).

## Assembler command line interface

On POSIX environment, assembler command line interface is provided.
It can generate Intel HEX or Motorola S-Record output.

    libasm assembler (version 1.6.62)
    usage: asm [-o <output>] [-l <list>] <input>
      -C <CPU>          : target CPU
         MC6800 MB8861 MC6801 HD6301 MC68HC11 MC6805 MC146805 MC68HC05
         MC68HC08 MC68HCS08 MC6809 HD6309 MC68HC12 MC68HC16 MOS6502 R65C02
         G65SC02 W65C02S W65C816S i8039 i8048 i80C39 i80C48 MSM80C39 MSM80C48
         i8051 i8080 i8085 V30EMU Z80 Z180 Z280 Z380 Z8 Z86C Z88 TLCS90
         INS8060 INS8070 CDP1802 CDP1804 CDP1804A SCN2650 F3850 IM6100 HD6120
         TMS7000 TMS370 TMS32010 TMS32015 TMS32020 TMS320C25 TMS320C26
         TMS320C20X TMS320C5X i8086 i80186 i80286 V30 i8096 i80196 MC68000
         MC68010 MC68020 MC68030 MC68040 TMS9900 TMS9980 TMS9995 TMS99105
         TMS99110 Z8001 Z8002 NS32032 MN1610 MN1613 MN1613A J11 T11
      -o <output>       : output file
      -l <list>         : list file
      -S[<bytes>]       : output Motorola S-Record format
      -H[<bytes>]       : output Intel HEX format
                        : optional <bytes> specifies data record length (max 32)
      -h                : use lower case letter for hexadecimal
      -n                : output line number to list file
      -v                : print progress verbosely
      --<name>=<vale>   : extra options (<type> [, <CPU>])
      --list-radix      : set listing radix (8, 16) (int)
      --smart-branch    : optimize branch instruction (bool)
      --extmode         : Extended mode (bool: Z380)
      --fpu             : floating point co-processor (text: 8086, 68000, 32032)
      --implicit-word   : unknown instruction defines word (bool: 6100, J11)
      --lwordmode       : Long word mode (bool: Z380)
      --optimize-index  : optimize zero index (bool: Z8)
      --optimize-segment: optimize segment override (bool: 8086)
      --pc-bits         : program counter width in bit, default 13 (int: 6805)
      --pmmu            : memory management unit (text: 68000, 32032)
      --reg-alias       : emit register alias regarding setrp value (bool: Z8)
      --short-direct    : optimize direct addressing (bool: Z8001)
      --use-register    : enable register name Rn (bool: 1802)
      --zilog-syntax    : Use Zilog syntax (bool: 8080)

## Disassembler command line interface

On POSIX environment, disassembler command line interface is provided.
It can read Intel HEX or Motorola S-Record input.

    libasm disassembler (version 1.6.62)
    usage: dis -C <CPU> [-o <output>] [-l <list>] <input>
      -C <CPU>         : target CPU
         MC6800 MB8861 MC6801 HD6301 MC68HC11 MC6805 MC146805 MC68HC05
         MC68HC08 MC68HCS08 MC6809 HD6309 MC68HC12 MC68HC16 MOS6502 R65C02
         G65SC02 W65C02S W65C816S i8039 i8048 i80C39 i80C48 MSM80C39 MSM80C48
         i8051 i8080 i8085 V30EMU Z80 Z180 Z280 Z380 Z8 Z86C Z88 TLCS90
         INS8060 INS8070 CDP1802 CDP1804 CDP1804A SCN2650 F3850 IM6100 HD6120
         TMS7000 TMS370 TMS32010 TMS32015 TMS32020 TMS320C25 TMS320C26
         TMS320C20X TMS320C5X i8086 i80186 i80286 V30 i8096 i80196 MC68000
         MC68010 MC68020 MC68030 MC68040 TMS9900 TMS9980 TMS9995 TMS99105
         TMS99110 Z8001 Z8002 NS32032 MN1610 MN1613 MN1613A J11 T11
      -o <output>      : output file
      -l <list>        : list file
      <input>          : file can be Motorola S-Record or Intel HEX format
      -A start[,end]   : disassemble start address and optional end address
      -r               : use program counter relative notation
      -h               : use lower case letter for hexadecimal
      -u               : use upper case letter for output
      -v               : print progress verbosely
      --<name>=<vale>  : extra options (<type> [, <CPU>])
      --upper-hex      : use upper case letter for hexadecimal (bool)
      --upper-case     : use upper case letter for output (bool)
      --list-radix     : set listing radix (8, 16) (int)
      --relative       : program counter relative branch target (bool)
      --c-style        : C language style number constant (bool)
      --intel-hex      : Intel style hexadecimal (bool)
      --origin-char    : letter for origin symbol (char)
      --gnu-as         : GNU assembler compatible (bool)
      --external-paren : disp2(disp(ext)) as external addressing (bool: 32032)
      --extmode        : Extended mode (bool: Z380)
      --fpu            : floating point co-processor (text: 8086, 68000, 32032)
      --indirect-long  : [] for indirect long operand (bool: 6502)
      --longa          : enable 16-bit accumulator (bool: 6502)
      --longi          : enable 16-bit index registers (bool: 6502)
      --lwordmode      : Long word mode (bool: Z380)
      --pc-bits        : program counter width in bit (default 13) (int: 6805)
      --pmmu           : memory management unit (text: 68000, 32032)
      --segment-insn   : segment override as instruction (bool: 8086)
      --segmented-addr : use <<segment>> notation (bool: Z8001)
      --short-direct   : use |addr| for short direct notation (bool: Z8001)
      --string-insn    : string instruction as repeat operand (bool: 8086)
      --use-absolute   : zero register indexing as absolute addressing (bool: 8096)
      --use-aux-name   : use aux register name ARn (bool: 32010)
      --use-port-name  : use port name PAn (bool: 32010)
      --use-register   : use register name Rn (bool: 1802)
      --use-sharp      : use # (default =) for immediate (bool: 8070)
      --work-register  : prefer work register name than alias address (bool: Z8)
      --zilog-syntax   : Use Zilog syntax (bool: 8080)

## Supported host environment

  - Arduino (avr, megaavr, samd, teensy)

  - PlatformIO (atmelavr, atmelmegaavr, atmelsam, teensy)

  - Linux, macOS (C++14)

<div class="note">

More information about this library can be found at
[GitHub](https://github.com/tgtakaoka/libasm)

</div>
