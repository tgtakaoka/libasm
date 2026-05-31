# Copyright 2026 Tadashi G. Takaoka
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

# nasm assembler rules
#
# nasm is a third-party reference (alongside GAS and libasm asm).  Each
# nasm-compatible target has a small .nasm wrapper that %includes
# sed-transformed copies of the shared .ginc files.  The transformation
# (ginc-to-nasm.sed) converts GAS/libasm Intel syntax into the more
# restrictive nasm dialect.
#
NASM = nasm

# Transform a .ginc into nasm-compatible form via ginc-to-nasm.sed
%.ninc: %.ginc ginc-to-nasm.sed
	sed -f ginc-to-nasm.sed $< > $@

# Per-target dependency declarations: each .nasm wrapper %includes a fixed
# set of .ninc files. Listing them here lets make build them on demand.
nasm-test_i8086.bin: test_i8086.ninc test_i8086_far.ninc test_i8087.ninc
nasm-test_i80186.bin: test_i8086.ninc test_i8086_far.ninc test_i80186.ninc test_i8087.ninc test_i80c187.ninc
nasm-test_i80286.bin: test_i8086.ninc test_i80186.ninc test_i80286.ninc test_i8087.ninc test_i80287.ninc
nasm-test_real_i80386.bin: test_i8086.ninc test_i80186.ninc test_i80286.ninc test_i80386.ninc test_i8087.ninc test_i80287.ninc test_i80387.ninc

# i80386 (32-bit / .code32) uses 32-bit-mode transformed copies (named %.b32.ninc).
nasm-test_i80386.bin: test_i8086.b32.ninc test_i80186.b32.ninc test_i80286.b32.ninc test_i80386.b32.ninc test_i8087.b32.ninc test_i80287.b32.ninc test_i80387.b32.ninc

%.b32.ninc: %.ginc ginc-to-nasm.sed ginc-to-nasm-32.sed
	sed -f ginc-to-nasm.sed $< | sed -f ginc-to-nasm-32.sed > $@


# Assemble a .nasm wrapper to a flat binary; the wrapper %includes the
# corresponding .ninc files (which the build generates above).
nasm-%.bin: %.nasm
	$(NASM) -f bin -o $@ $<

# Convert flat bin to Intel HEX, OVERWRITING the committed test_*.hex.
# Strip objcopy's Start-Segment-Address record (type 03) and CR bytes
# (mirrors the GAS pipeline's `tr -d '\015'`) so the output matches the
# committed format and `git diff` is empty when bytes match.
test_%.hex: nasm-test_%.bin
	objcopy -I binary -O ihex --change-addresses=0x1000 $< /dev/stdout \
	| grep -v '^:040000030' | tr -d '\015' > $@

# Convenience phony -- depend on every committed .hex so `make nasm-test`
# regenerates them all and `git diff -- test/reference/*.hex` confirms parity.
# Per-arch hex prerequisites are appended to nasm-test in the commits that
# add the matching reference sources.
.PHONY:: nasm-test
nasm-test: test_i8086.hex test_i80186.hex test_i80286.hex
nasm-test: test_i80386.hex test_real_i80386.hex

clean-nasm:
	-rm -f *.ninc nasm-*.bin nasm-*.hex

# Local Variables:
# mode: makefile-gmake
# End:
# vim: set ft=make:
