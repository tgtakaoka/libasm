#include "asm_r65c02.h"
#include "asm_directive.h"
#include "asm_driver.h"
#include "bin_formatter.h"

int main(int argc, const char **argv) {
    AsmR65c02 assembler;
    AsmMostekDirective<AsmR65c02> directive(assembler);
    AsmDriver<AsmR65c02, SRecord<target::uintptr_t>> driver(directive);
    return driver.main(argc, argv);
}
