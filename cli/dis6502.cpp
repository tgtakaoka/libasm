#include "dis_r65c02.h"
#include "dis_driver.h"

int main(int argc, const char **argv) {
    DisR6502 disassembler;
    DisDriver<DisR6502> driver(disassembler);
    return driver.main(argc, argv);
}
