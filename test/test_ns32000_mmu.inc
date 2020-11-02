# Copyright 2020 Tadashi G. Takaoka
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# Load Memory Management Register: LMR mmureg/short, src/gen
        lmr     bpr0, r0
        lmr     bpr1, r1
        lmr     msr, r5
        lmr     bcnt, r6
        lmr     ptb0, r7
        lmr     ptb1, r0
        lmr     eia, r1
# Store Memory Management Register: SMR mmureg/short, dest/gen
        smr     bpr0, r0
        smr     bpr1, r1
        smr     msr, r5
        smr     bcnt, r6
        smr     ptb0, r0
        smr     ptb1, r7
        smr     eia, r1
# Validate Address for Reading: RDVAL src/gen
        rdval   512(r0)
# Validate Address for Writing: WRVAL loc/gen
        wrval   512(r0)

# Local Variables:
# mode: asm
# End:
# vim: set ft=asm:
