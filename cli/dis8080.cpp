#include "dis_i8080.h"
#include "dis_driver.h"

int main(int argc, const char **argv) {
    DisI8080 disassembler;
    DisDriver<DisI8080> driver(disassembler);
    return driver.main(argc, argv);
}
