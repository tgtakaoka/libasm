#include "asm_i8080.h"
#include "asm_driver.h"

int main(int argc, const char **argv) {
    AsmI8080 assembler;
    AsmIntelDirective<AsmI8080> directive(assembler);
    AsmDriver<AsmI8080> driver(directive);
    return driver.main(argc, argv);
}
