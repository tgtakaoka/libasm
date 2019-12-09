#include "asm_z80.h"
#include "asm_driver.h"

int main(int argc, const char **argv) {
    AsmZ80 assembler;
    AsmIntelDirective<AsmZ80> directive(assembler);
    AsmDriver<AsmZ80> driver(directive);
    return driver.main(argc, argv);
}
