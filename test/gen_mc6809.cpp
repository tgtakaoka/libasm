#include "dis_mc6809.h"
#include "gen_driver.h"

int main(int argc, const char **argv) {
    DisMc6809 dis6809;
    GenDriver<target::uintptr_t> driver(dis6809);
    return driver.main(argc, argv);
}

