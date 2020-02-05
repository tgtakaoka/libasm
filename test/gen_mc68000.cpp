#include "dis_mc68000.h"
#include "gen_driver.h"

int main(int argc, const char **argv) {
    DisMc68000 dis68000;
    GenDriver<target::uintptr_t> driver(dis68000);
    return driver.main(argc, argv);
}

