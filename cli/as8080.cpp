#include "asm_i8080.h"
#include "asm_driver.h"

template<typename Asm>
class AsmIntelDirective : public AsmDirective<Asm> {
public:
    typedef typename Asm::addr_t Addr;
    AsmIntelDirective(Asm &assembler) : AsmDirective<Asm>(assembler) {}

protected:
    Error processDirective(
        const char *directive, const char *&label,
        CliMemory<Addr> &memory) override {
        this->_parser.isSymbolLetter(0);
        if (strcasecmp(directive, "db") == 0)
            return this->defineBytes(memory);
        if (strcasecmp(directive, "dw") == 0)
            return this->defineWords(memory, false);
        if (strcasecmp(directive, "ds") == 0)
            return this->defineSpaces();
        return UNKNOWN_DIRECTIVE;
    }
};

int main(int argc, const char **argv) {
    AsmI8080 assembler;
    AsmIntelDirective<AsmI8080> directive(assembler);
    AsmDriver<AsmI8080> driver(directive);
    return driver.main(argc, argv);
}
