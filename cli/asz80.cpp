#include "asm_z80.h"
#include "asm_directive.h"
#include "asm_driver.h"
#include "bin_formatter.h"

int main(int argc, const char **argv) {
    AsmZ80 assembler;
    AsmIntelDirective<target::uintptr_t> directive(assembler);
    AsmDriver<target::uintptr_t, IntelHex<target::uintptr_t>> driver(directive);
    return driver.main(argc, argv);
}