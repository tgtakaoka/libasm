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

PATH+=:../cli

all:
	@echo '"make asm-test" assemble with "asm" command'
	@echo '"make asl-test" assemble with "asl" command'
	@echo '"make gas-test" assemble with "gas" command'

-include ../../src/Makefile.arch
-include ../Makefile.asl
-include ../Makefile.gas

ASM_TESTS = $(patsubst %.asm,%,$(wildcard test_*.asm))
ASM_OUTS  = $(foreach f,$(ASM_TESTS),$(f).$(call out-ext,$(f)))
define asm-rule # file fmt
asm-$(1).$(2): $(1).asm
	asm -o $(1).$(2) -l $(1).lst $$^

endef
$(eval $(foreach f,$(ASM_TESTS),$(call asm-rule,$(f),$(call out-ext,$(f)))))

asm-test: clean-asm $(ASM_OUTS:%=asm-%)
clean-asm:
	-rm -f $(ASM_OUTS)

ASL_TESTS = $(patsubst %.asl,%,$(wildcard test_*.asl))
ASL_OUTS  = $(foreach f,$(ASL_TESTS),$(f).$(call out-ext,$(f)))
define asl-rule # file suffix format
asl-$(1).$(2): $(1).asl
	asl $$(ASL_FLAGS) -L $$^
	p2hex $$(P2HEX_FLAGS) -F $(3) $(1).p $(1).$(2)

endef
$(eval $(foreach f,$(ASL_TESTS),$(call asl-rule,$(f),$(call out-ext,$(f)),$(call out-fmt,$(f)))))

asl-test: clean-asl $(ASL_OUTS:%=asl-%)
clean-asl:
	-rm -f $(ASL_OUTS)

GAS_TESTS = $(wildcard test_*.gas)
GAS_OUTS  = $(foreach f,$(GAS_TESTS),$(f:.gas=).$(call out-ext,$(f:.gas=)))
gas-test: clean-gas $(GAS_OUTS)
clean-gas:
	-rm -f $(GAS_OUTS)

clean:
	-rm -f *.o *.p *.lst

# Local Variables:
# mode: makefile-gmake
# End:
# vim: set ft=make:
