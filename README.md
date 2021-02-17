![C/C++ CI](https://github.com/tgtakaoka/libasm/workflows/C/C++%20CI/badge.svg)

# libasm: cross assemble/disassemble library

The libasm allows assembling and disassembling supported retro CPUs on
a small environment (less than 9kB~17kB Flash and ~350B RAM on AVR
Arduino).

- Supported target CPUs.
  - MC6800, MC6801, HD6301, MC68HC11
  - MC6809, HD6309
  - MOS6502, G65SC02, R65C02, W65C02S
  - W65C816S
  - i8051
  - i8080, i8085 (Intel or Zilog syntax)
  - Z80
  - Z8 (Z86), Z86C, Z88 (Super8)
  - TLCS90
  - INS8060 (SC/MP)
  - INS8070
  - CDP1802, CDP1804, CDP1804A
  - i8086
  - TMS9900, TMS9995, TMS99105
  - MC68000
  - Z8001, Z8002
  - NS32032

- Supported host environment.
  - Arduino (avr, megaavr, samd, teensy)
  - PlatformIO (atmelavr, atmelmegaavr, atmelsam, teensy)
  - Linux, macOS (`C++14`: `g++`, `clang++`)

More information about this library can be found at
https://github.com/tgtakaoka/libasm
