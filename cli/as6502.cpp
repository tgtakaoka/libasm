#include "asm_r65c02.h"
#include "asm_directive.h"
#include "asm_driver.h"
#include "bin_formatter.h"

int main(int argc, const char **argv) {
    AsmR6502 assembler;
    AsmMostekDirective<AsmR6502> directive(assembler);
    AsmDriver<AsmR6502, SRecord<target::uintptr_t>> driver(directive);
    return driver.main(argc, argv);
}
