#include "asm_hd6309.h"
#include "asm_driver.h"

template<typename Asm>
class AsmMotoDirective : public AsmDirective<Asm> {
public:
    typedef typename Asm::addr_t Addr;
    AsmMotoDirective(Asm &assembler) : AsmDirective<Asm>(assembler) {}

protected:
    Error processDirective(
        const char *directive, const char *&label,
        CliMemory<Addr> &memory) override {
        if (strcasecmp(directive, "fcb") == 0 ||
            strcasecmp(directive, "fcc") == 0)
            return this->defineBytes(memory);
        if (strcasecmp(directive, "fdb") == 0)
            return this->defineWords(memory, true);
        if (strcasecmp(directive, "rmb") == 0)
            return this->defineSpaces();
        return UNKNOWN_DIRECTIVE;
    }
};

int main(int argc, const char **argv) {
    AsmHd6309 assembler;
    AsmMotoDirective<AsmHd6309> directive(assembler);
    AsmDriver<AsmHd6309> driver(directive);
    return driver.main(argc, argv);
}
