#include "asm_tms9995.h"
#include "asm_driver.h"

int main(int argc, const char **argv) {
    AsmTms9995 assembler;
    AsmIntelDirective<AsmTms9995> directive(assembler);
    AsmDriver<AsmTms9995> driver(directive);
    return driver.main(argc, argv);
}
