#include "asm_i8080.h"
#include "asm_directive.h"
#include "asm_driver.h"
#include "bin_formatter.h"

int main(int argc, const char **argv) {
    AsmI8080 assembler;
    AsmIntelDirective<AsmI8080> directive(assembler);
    AsmDriver<AsmI8080, IntelHex<target::uintptr_t>> driver(directive);
    return driver.main(argc, argv);
}
