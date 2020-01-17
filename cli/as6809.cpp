#include "asm_mc6809.h"
#include "asm_directive.h"
#include "asm_driver.h"
#include "bin_formatter.h"

int main(int argc, const char **argv) {
    AsmMc6809 assembler;
    AsmMotoDirective<AsmMc6809> directive(assembler);
    AsmDriver<AsmMc6809, SRecord<target::uintptr_t>> driver(directive);
    return driver.main(argc, argv);
}
