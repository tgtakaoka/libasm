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

#include "dis_i960.h"
#include "gen_driver.h"
#include "tokenizer.h"

using namespace libasm::i960;
using namespace libasm::gen;

int main(int argc, const char **argv) {
    DisI960 disi960;
    GenDriver driver(disi960);
    if (driver.main(argc, argv))
        return 1;

    // GNU as resolves a branch written against the location counter at
    // assembly time, so writing those relatively keeps the generated source
    // free of relocations and needs no linker; an absolute target would
    // instead be measured from the start of the section, which is not where
    // this source is generated to run.  Its location counter is ".".
    // A MEMB operand whose displacement would also fit a MEMA one assembles
    // back to the shorter encoding, so the generated source could not
    // reproduce it; leave those out.
    disi960.setOption("canonical-mem", "enable");

    if (driver.generateGas()) {
        disi960.setOption("relative", "enable");
        disi960.setOption("origin-char", ".");
    }

    // Numbers are C style for both this assembler and ASL.  The number matcher
    // already collapses the digits of a register name and of an index scale,
    // so no register or index helper earns its place, and every displacement
    // is rendered as an unsigned 32-bit value, leaving no sign for an
    // IndexDisp tokenizer to consolidate; a no-op one would only perturb the
    // generator's drop heuristic.
    //
    // A CTRL branch reaches 0x800000 bytes back, so generate from there: from
    // a lower origin the most negative displacements would wrap past zero and
    // read as very large addresses.
    TestGenerator generator(
            driver, disi960, 0x00800000, standardTokenizers<CstyleNumber>(disi960.curSym()));
    // An instruction is one 32-bit word, so scan it a byte at a time to give
    // the generator the child chain its drop heuristic needs, see scanByByte.
    generator.scanByByte().generate();

    return driver.close();
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
