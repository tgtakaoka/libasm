#include "dis_m6502.h"
#include "dis_driver.h"

int main(int argc, const char **argv) {
    DisM6502 disassembler;
    DisDriver<target::uintptr_t> driver(disassembler);
    return driver.main(argc, argv);
}
