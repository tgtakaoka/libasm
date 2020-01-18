#include "dis_mc6809.h"
#include "dis_driver.h"

int main(int argc, const char **argv) {
    DisMc6809 disassembler;
    DisDriver<target::uintptr_t> driver(disassembler);
    return driver.main(argc, argv);
}
