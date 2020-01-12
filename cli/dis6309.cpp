#include "dis_hd6309.h"
#include "dis_driver.h"

int main(int argc, const char **argv) {
    DisHd6309 disassembler;
    DisDriver<DisHd6309> driver(disassembler);
    return driver.main(argc, argv);
}
