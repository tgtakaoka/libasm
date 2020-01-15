#include "dis_tms9995.h"
#include "dis_driver.h"

int main(int argc, const char **argv) {
    DisTms9995 disassembler;
    DisDriver<DisTms9995, true> driver(disassembler);
    return driver.main(argc, argv);
}
