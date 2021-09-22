[![C/C++ CI](https://github.com/tgtakaoka/libasm/actions/workflows/ccpp.yml/badge.svg)](https://github.com/tgtakaoka/libasm/actions/workflows/ccpp.yml)
[![Compile Examples](https://github.com/tgtakaoka/libasm/actions/workflows/arduino-ci.yml/badge.svg)](https://github.com/tgtakaoka/libasm/actions/workflows/arduino-ci.yml)
[![PlatformIO CI](https://github.com/tgtakaoka/libasm/actions/workflows/platformio-ci.yml/badge.svg)](https://github.com/tgtakaoka/libasm/actions/workflows/platformio-ci.yml)

# libasm: cross assemble/disassemble library

The libasm allows assembling and disassembling supported retro CPUs on
a small environment (less than 9kB~17kB Flash and ~350B RAM on AVR
Arduino).

## Supported target CPUs
- MC6800, MC6801, HD6301, MC6805, MC146805, MC68HC05
- MC6809, HD6309, MC68HC11
- MOS6502, G65SC02, R65C02, W65C02S W65C816S
- i8051
- i8080, i8085 (Intel or Zilog syntax)
- Z80
- Z8 (Z86), Z86C, Z88 (Super8)
- TLCS90
- INS8060 (SC/MP), INS8070
- CDP1802, CDP1804, CDP1804A
- TMS32010, TMS32015
- TMS9900, TMS9995, TMS99105
- i8086
- MC68000
- Z8001, Z8002
- NS32032

## Assembler library interface

Assembling MC68HC11 instruction in string to binary can be done in a
minutes. You can also add symbols table lookup via `SymbolTable`
interface.

```
    #include <asm_mc6800.h>
    libasm::mc6800::AsmMc6800 asm6800;
    asm6800.setCpu("68HC11");
    const char *text = "SUBD $90";
    libasm::Insn insn;
    asm6800.encode(text, insn, 0x1000, /*SymbolTable*/nullptr);
    assert(insn.address()  == 0x1000);
    assert(insn.length()   == 2);    // direct addressing
    assert(insn.bytes()[0] == 0x93); // SUBD
    assert(insn.bytes()[1] == 0x90); // $90
```

## Disassembler library interface

To disassemble MC68000 binaries, you need to wrap memory by
`ArrayMemory` or implements your own `DisMemory` interface to feed
binaries. You can also add symbol table lookup via `SymbolTable`
interface.

```
    #include <dis_mc68000.h>
    const uint16_t words[0047372, 0x1232]
    libasm::ArrayMemory memory(0x100, words, sizeof(words));
    libasm::mc68000::DisMc68000 dis68k;
    dis68k.setRelativeTarget(true);
    libasm::Insn insn;
    char opr[80];
    dis68k.decode(memory, insn, opr, sizeof(opr), /*SymbolTable*/nullptr);
    assert(std::string(insn.name()) == "jmp");
    assert(std::string(opr) == "(*+$1234,pc)");
    ```

## Assembler command line interface

```
libasm assembler (version 1.6.12)
usage: asm [-o <output>] [-l <list>] <input>
  -C <CPU>    : target CPU
                MC6800, MC6801, HD6301, MC6805, MC146805, MC68HC05, MC68HC11
                MC6809, HD6309, MOS6502, R65C02, G65SC02, W65C02S, W65C816S
                i8051, i8080, i8085, Z80, Z8, Z86C, Z88, TLCS90, INS8060
                INS8070, CDP1802, CDP1804, CDP1804A, TMS32010, TMS32015
                i8086, MC68000, TMS9900, TMS9995, TMS99105, Z8001, Z8002
                NS32032
  -o <output> : output file
  -l <list>   : list file
  -S[<bytes>] : output Motorola S-Record format
  -H[<bytes>] : output Intel HEX format
              : optional <bytes> specifies data record length (max 32)
  -u          : use uppercase letter for listing
  -n          : output line number to list file
  -v          : print progress verbosely
```

## Disassembler command line interface

```
libasm disassembler (version 1.6.12)
usage: dis -C <CPU> [-o <output>] [-l <list>] <input>
  -C <CPU>    : target CPU
                MC6800, MC6801, HD6301, MC6805, MC146805, MC68HC05, MC68HC11
                MC6809, HD6309, MOS6502, R65C02, G65SC02, W65C02S, W65C816S
                i8051, i8080, i8085, Z80, Z8, Z86C, Z88, TLCS90, INS8060
                INS8070, CDP1802, CDP1804, CDP1804A, TMS32010, TMS32015
                i8086, MC68000, TMS9900, TMS9995, TMS99105, Z8001, Z8002
                NS32032
  -o <output> : output file
  -l <list>   : list file
  <input>     : file can be Motorola S-Record or Intel HEX format
  -A start[,end]
              : disasseble start address and optional end address
  -r          : use program counter relative notation
  -u          : use uppercase letter for output
  -v          : print progress verbosely
```

## Supported host environment
- Arduino (avr, megaavr, samd, teensy)
- PlatformIO (atmelavr, atmelmegaavr, atmelsam, teensy)
- Linux, macOS (C++14)

More information about this library can be found at
https://github.com/tgtakaoka/libasm
