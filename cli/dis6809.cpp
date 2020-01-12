#include "dis_hd6309.h"
#include "dis_driver.h"

int main(int argc, const char **argv) {
    DisMc6809 disassembler;
    DisDriver<DisMc6809> driver(disassembler);
    return driver.main(argc, argv);
}
