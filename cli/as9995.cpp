#include "asm_tms9995.h"
#include "asm_directive.h"
#include "asm_driver.h"
#include "bin_formatter.h"

int main(int argc, const char **argv) {
    AsmTms9995 assembler;
    AsmIntelDirective<target::uintptr_t> directive(assembler);
    AsmDriver<target::uintptr_t, IntelHex<target::uintptr_t>, true> driver(directive);
    return driver.main(argc, argv);
}
