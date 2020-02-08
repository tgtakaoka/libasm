#include "dis_mc68000.h"
#include "dis_driver.h"

int main(int argc, const char **argv) {
    DisMc68000 disassembler;
    DisDriver<target::uintptr_t> driver(disassembler);
    return driver.main(argc, argv);
}
