#include "dis_r65c02.h"
#include "dis_driver.h"

int main(int argc, const char **argv) {
    DisR65c02 disassembler;
    DisDriver<DisR65c02> driver(disassembler);
    return driver.main(argc, argv);
}
