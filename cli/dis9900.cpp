#include "dis_tms9900.h"
#include "dis_driver.h"

int main(int argc, const char **argv) {
    DisTms9900 disassembler;
    DisDriver<target::uintptr_t> driver(disassembler);
    return driver.main(argc, argv);
}
