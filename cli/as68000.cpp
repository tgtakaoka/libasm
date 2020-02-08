#include "asm_mc68000.h"
#include "asm_directive.h"
#include "asm_driver.h"
#include "bin_formatter.h"

int main(int argc, const char **argv) {
    AsmMc68000 assembler;
    AsmMotoDirective<target::uintptr_t> directive(assembler);
    AsmDriver<target::uintptr_t, SRecord<target::uintptr_t>> driver(directive);
    return driver.main(argc, argv);
}
