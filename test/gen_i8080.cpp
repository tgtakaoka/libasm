#include "dis_i8080.h"
#include "gen_driver.h"

int main(int argc, const char **argv) {
    DisI8080 dis8080;
    GenDriver<target::uintptr_t> driver(dis8080);
    return driver.main(argc, argv);
}

