#include "asm_hd6309.h"
#include "asm_driver.h"

int main(int argc, const char **argv) {
    AsmMc6809 assembler;
    Asm09Directive<AsmMc6809> directive(assembler);
    AsmDriver<AsmMc6809> driver(directive);
    return driver.main(argc, argv);
}
