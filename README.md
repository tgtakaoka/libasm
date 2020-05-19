![C/C++ CI](https://github.com/tgtakaoka/libasm/workflows/C/C++%20CI/badge.svg)

# libasm: cross assemble/disassemble library

The libasm allows for assembling and disassembling supported CPUs on a
small environment (less than 15kB on Arduino).

* Supported target CPUs.
  - MC6809, HD6309
  - MC6800, MC6801, HD6301, MC68HC11
  - MOS6502, W65SC02, R65C02, W65C02S
  - W65816 (Support 24bit address and LONGA/LONGI)
  - i8080, i8085
  - Z80
  - SC/MP
  - INS8070
  - CDP1802
  - TMS9900, TMS9995
  - MC68000

* Supported host environment.
  - Arduino (AVR, ARM)
  - Linux, macOS (`g++`, `clang++`)

More information about this library can be found at
https://github.com/tgtakaoka/libasm
