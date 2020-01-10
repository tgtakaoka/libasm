#include "asm_hd6309.h"
#include "asm_driver.h"

int main(int argc, const char **argv) {
    AsmHd6309 assembler;
    AsmMotoDirective<AsmHd6309> directive(assembler);
    AsmDriver<AsmHd6309> driver(directive);
    return driver.main(argc, argv);
}
