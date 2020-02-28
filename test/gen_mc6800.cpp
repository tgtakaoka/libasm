#include "dis_mc6800.h"
#include "gen_driver.h"

int main(int argc, const char **argv) {
    DisMc6800 dis6800;
    GenDriver<target::uintptr_t> driver(dis6800);
    return driver.main(argc, argv);
}

