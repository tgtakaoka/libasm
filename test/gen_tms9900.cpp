#include "dis_tms9900.h"
#include "gen_driver.h"

int main(int argc, const char **argv) {
    DisTms9900 dis9900;
    GenDriver<target::uintptr_t> driver(dis9900);
    return driver.main(argc, argv);
}

