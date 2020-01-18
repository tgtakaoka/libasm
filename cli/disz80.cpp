#include "dis_z80.h"
#include "dis_driver.h"

int main(int argc, const char **argv) {
    DisZ80 disassembler;
    DisDriver<target::uintptr_t> driver(disassembler);
    return driver.main(argc, argv);
}
