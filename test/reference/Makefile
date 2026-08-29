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

help:
	@echo '"make asm-test" assemble with "asm" command'
	@echo '"make asl-test" assemble with "asl" command'
	@echo '"make gas-test" assemble with "gas" command'

-include ../../src/Makefile.arch
-include ../Makefile.asm
-include ../Makefile.asl
-include ../Makefile.gas
-include ../Makefile.nasm

.PHONY:: asm-test asl-test gas-test
.PHONY:: clean clean-asm clean-asl clean-gas clean-nasm

ASM=../../cli/asm
$(ASM)-refresh:
	+$(MAKE) -C ../../cli asm

ASM_TESTS = $(patsubst %.asm,%,$(wildcard test_*.asm))
ASM_OUTS  = $(foreach t,$(ASM_TESTS),$(t).$(call out-ext,$(t)))
define asm-rule # test suffix option
.PHONY:: asm-$(1).$(2)
asm-$(1).$(2):: $(ASM)-refresh
asm-$(1).$(2):: $(1).asm
	$$(ASM) $$(ASM_FLAGS) $(3) -o $(1).$(2) -l $(1).lst $$^

endef
$(eval $(foreach t,$(ASM_TESTS),$(call asm-rule,$(t),$(call out-ext,$(t)),$(call asm-opt,$(t)))))

asm-test: $(ASM)-refresh clean-asm $(ASM_OUTS:%=asm-%)
clean-asm:
	-rm -f $(ASM_OUTS)

ASL_exclude = test_i80286 test_i80386 test_real_i80386 test_i80486 test_real_i80486 \
   test_tms320c31 test_tms320c32 \
   test_h8s2000 test_h8s2000n test_h8s2600 test_h8s2600n test_shdsp test_sh2e test_sh2a \
   test_am29050
ASL_TESTS = $(foreach a,$(ASM_TESTS) \
   $(foreach t,$(patsubst %.asl,%,$(wildcard test_*.asl)),$(if $(filter $(t),$(ASM_TESTS)),,$(t))),\
   $(if $(filter $(a),$(ASL_exclude)),,$(a)))
ASL_OUTS  = $(foreach t,$(ASL_TESTS),$(t).$(call out-ext,$(t)))
define asl-rule # test suffix option source
.PHONY:: asl-$(1).$(2)
asl-$(1).$(2): $(4)
	$$(ASL) $$(ASL_FLAGS) -L $$^ && $$(call asl-conv,$(1),$(2))

endef
define asl-source # test
$(if $(wildcard $(1).asl),$(1).asl,$(1).asm)
endef
$(eval $(foreach t,$(ASL_TESTS),$(call asl-rule,$(t),$(call out-ext,$(t)),$(call asl-opt,$(t)),$(call asl-source,$(t)))))

asl-test: clean-asl $(ASL_OUTS:%=asl-%)
clean-asl:
	-rm -f $(ASL_OUTS)

GAS_TESTS = $(patsubst %.gas,%,$(wildcard test_*.gas))
GAS_OUTS  = $(foreach t,$(GAS_TESTS),$(t).$(call out-ext,$(t)))
gas-test: clean-gas $(GAS_OUTS:%=gas-%)

# Linker @files are generated on demand and kept out of git, so clean and
# clean-gas can wipe them.
GAS_LD_FLAGS = $(patsubst %.gas,%.ld_flags,$(wildcard test_am29*.gas)) \
    test_z8001.ld_flags test_z8002.ld_flags

# Am29000: these sources name no undefined symbol, so the @file holds just the
# text origin, which must match the org in test_am29XXX.asm; the --defsym lines
# the autogen sources need have no counterpart here.
AM29000_TEXT_ORG = 0x1000
test_am29%.ld_flags:
	printf -- '--section-start=.text=%s\n' '$(AM29000_TEXT_ORG)' > $@

# Z8001: the text origin plus one --section-start per per-segment helper section
# the source declares, each placed at seg<<16 -- the same mapping
# scrape-z8001-syms.sh applies to the autogen sources.  The origin must match
# the org in test_z8001.asm, and is held at or above 0x100000 on purpose: below
# 1MB binutils writes the ihex base as a type 02 (extended segment, base<<4)
# record, which does not describe Z8001 segmentation and does not match the
# type 04 (extended linear, base<<16) record the other assemblers emit.
Z8001_TEXT_ORG = 0x100000
test_z8001.ld_flags: test_z8001.gas
	printf -- '--section-start=.text=%s\n' '$(Z8001_TEXT_ORG)' > $@
	sed -n 's,^[ \t]*\.section[ \t]*seg_\([0-9a-f][0-9a-f]\)$$,--section-start=seg_\1=0x\10000,p' \
	    $< >> $@

# Z8002: the text origin plus the two helper sections the source declares, at
# the addresses its operands name.
Z8002_TEXT_ORG = 0x1000
Z8002_BSS1_ORG = 0x1234
Z8002_BSS2_ORG = 0x5678
test_z8002.ld_flags:
	{ printf -- '--section-start=.text=%s\n' '$(Z8002_TEXT_ORG)'; \
	  printf -- '--section-start=bss1=%s\n'  '$(Z8002_BSS1_ORG)'; \
	  printf -- '--section-start=bss2=%s\n'  '$(Z8002_BSS2_ORG)'; } > $@

# Keep the generated @files: they are prerequisites of pattern rules, so make
# would otherwise treat them as intermediate and delete them after linking.
.SECONDARY: $(GAS_LD_FLAGS)

clean-gas:
	-rm -f $(GAS_OUTS) *.ld_flags

clean: clean-nasm
	-rm -f *.o *.p *.lst *.dis *.bin *.ld_flags

# Local Variables:
# mode: makefile-gmake
# End:
# vim: set ft=make:
