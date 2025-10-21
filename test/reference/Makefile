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

.PHONY:: asm-test asl-test gas-test
.PHONY:: clean clean-asm clean-asl clean-gas

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

ASL_exclude = test_i80286
ASL_TESTS = $(foreach a,$(ASM_TESTS) \
   $(foreach t,$(patsubst %.asl,%,$(wildcard test_*.asl)),$(if $(filter $(t),$(ASM_TESTS)),,$(t))),\
   $(if $(filter $(a),$(ASL_exclude)),,$(a)))
ASL_OUTS  = $(foreach t,$(ASL_TESTS),$(t).$(call out-ext,$(t)))
define asl-rule # test suffix option source
.PHONY:: asl-$(1).$(2)
asl-$(1).$(2): $(4)
	$$(ASL) $$(ASL_FLAGS) -L $$^ && p2hex $$(P2HEX_FLAGS) $(3) $(1).p $(1).$(2)

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
clean-gas:
	-rm -f $(GAS_OUTS)

clean:
	-rm -f *.o *.p *.lst *.dis

# Local Variables:
# mode: makefile-gmake
# End:
# vim: set ft=make:
