#include "asm_hd6309.h"
#include "asm_driver.h"

int main(int argc, const char **argv) {
    AsmMc6809 assembler;
    AsmMotoDirective<AsmMc6809> directive(assembler);
    AsmDriver<AsmMc6809> driver(directive);
    return driver.main(argc, argv);
}
