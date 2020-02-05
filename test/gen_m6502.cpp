#include "dis_m6502.h"
#include "gen_driver.h"

int main(int argc, const char **argv) {
    DisM6502 dis6502;
    GenDriver<target::uintptr_t> driver(dis6502);
    return driver.main(argc, argv);
}

