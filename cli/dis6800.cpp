#include "dis_mc6800.h"
#include "dis_driver.h"

int main(int argc, const char **argv) {
    DisMc6800 disassembler;
    DisDriver<target::uintptr_t> driver(disassembler);
    return driver.main(argc, argv);
}
