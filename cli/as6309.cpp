#include "asm_hd6309.h"
#include "asm_directive.h"
#include "asm_driver.h"
#include "bin_formatter.h"

int main(int argc, const char **argv) {
    AsmHd6309 assembler;
    AsmMotoDirective<AsmHd6309> directive(assembler);
    AsmDriver<AsmHd6309, SRecord<target::uintptr_t>> driver(directive);
    return driver.main(argc, argv);
}
