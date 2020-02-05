#include "dis_z80.h"
#include "gen_driver.h"

int main(int argc, const char **argv) {
    DisZ80 disz80;
    GenDriver<target::uintptr_t> driver(disz80);
    return driver.main(argc, argv);
}

