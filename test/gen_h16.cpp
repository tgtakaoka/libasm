/*
 * Copyright 2026 Tadashi G. Takaoka
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "dis_h16.h"
#include "gen_driver.h"
#include "tokenized_text.h"

using namespace libasm::h16;
using namespace libasm::gen;

int main(int argc, const char **argv) {
    DisH16 dish16;
    GenDriver driver(dish16);
    if (driver.main(argc, argv))
        return 1;

    // C-style numbers + relative branch targets so the test generator can
    // tokenize operand values and collapse the address/branch/EA variants
    // (Motorola $xxxx and absolute targets are not recognized by the
    // tokenizer, so without this every EA variant is kept and the output
    // explodes -- e.g. ADD:G alone fills the file).
    dish16.setOption("c-style", "enable");
    dish16.setOption("relative", "enable");
    dish16.setOption("origin-char", "*");

    // Collapse a forced ":N" displacement/immediate/absolute width into its
    // natural-width form during dedup.  H16's rich EA already explodes the
    // src x dst cross product; the explicit :8/:16/:32 sizes would double it
    // again with no new coverage (the same addressing mode at a wider field).
    TokenizedText::reduceSizeSuffix = true;

    // The H16 exception vector table occupies 0x0000-0x03FC at reset (Manual
    // Table 4-13).  Place test code at 0x1000 so PC-relative deltas produce
    // targets in a sensible code-area range rather than the wrap-around
    // negative range that ORG=0 would yield.  ignoreSizeVariation() collapses
    // the disp8/disp16/disp32 length variants of one addressing mode (the
    // widths are size variations of the same mode, cf. gen_ns32000).
    TestGenerator generator(driver, dish16, 0x1000);
    generator.ignoreSizeVariation().generate();

    return driver.close();
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
