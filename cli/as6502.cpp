#include "asm_r65c02.h"
#include "asm_driver.h"

int main(int argc, const char **argv) {
    AsmR6502 assembler;
    AsmMotoDirective<AsmR6502> directive(assembler);
    AsmDriver<AsmR6502> driver(directive);
    return driver.main(argc, argv);
}
