#include "asm_r65c02.h"
#include "asm_driver.h"

int main(int argc, const char **argv) {
    AsmR65c02 assembler;
    AsmMotoDirective<AsmR65c02> directive(assembler);
    AsmDriver<AsmR65c02> driver(directive);
    return driver.main(argc, argv);
}
