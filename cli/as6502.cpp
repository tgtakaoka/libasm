#include "asm_r65c02.h"
#include "asm_driver.h"

template<typename Asm>
class AsmMostekDirective : public AsmDirective<Asm> {
public:
    typedef typename Asm::addr_t Addr;
    AsmMostekDirective(Asm &assembler) : AsmDirective<Asm>(assembler) {}

protected:
    Error processDirective(
        const char *directive, const char *&label,
        CliMemory<Addr> &memory) override {
        if (strcmp(directive, ":=") == 0
            || strcmp(directive, "=") == 0) {
            return this->defineLabel(label, memory);
        }
        if (strcasecmp(directive, "fcb") == 0)
            return this->defineBytes(memory);
        if (strcasecmp(directive, "fdb") == 0)
            return this->defineWords(memory, true);
        if (strcasecmp(directive, "rmb") == 0)
            return this->defineSpaces();
        return UNKNOWN_DIRECTIVE;
    }
};

int main(int argc, const char **argv) {
    AsmR6502 assembler;
    AsmMostekDirective<AsmR6502> directive(assembler);
    AsmDriver<AsmR6502> driver(directive);
    return driver.main(argc, argv);
}
