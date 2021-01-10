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

PATH+=:..:../../cli

all:
	@echo '"make generate" auto generate test sources'
	@echo '"make asm-test" assemble with "asm" command'
	@echo '"make asl-test" assemble with "asl" command'
	@echo '"make gas-test" assemble with "gas" command'

-include ../../src/Makefile.arch
-include ../Makefile.asl
-include ../Makefile.gas

# target which has ".inc" suffix for output.
INC_targets  = cdp1806 w65c816 tlcs90 z8001 z8002 z8k1 z8k2 ns32k
# target which for GNU assembler
GAS_targets  = z8k1 z8k2 ns32k

# suffix of generating file
define out-inc # target
$(filter $(1),$(INC_targets))
endef
define gen-out # target
$(if $(call out-inc,$(1)),gen_$(1).inc,gen_$(1).asm)
endef
# generator options
define gas-out
$(filter $(1),$(GAS_targets))
endef
define gen-opt
$(if $(call gas-out,$(1)),-g,$(if $(call out-inc,$(1)),-c -u,-u))
endef
# make rule for auto generation file
define gen-rule # arch target out opt
$(3):
	gen_$(1) $(4) -C $(CPU_$(2)) -o $$@ -l gen_$(2).lst

endef

GEN_OUTS = $(foreach a,$(ARCHS),$(foreach t,$(TGT_$(a)),$(call gen-out,$(t))))

# rules for auto generations
$(eval $(foreach a,$(ARCHS),$(foreach t,$(TGT_$(a)),\
	$(call gen-rule,$(a),$(t),$(call gen-out,$(t)),$(call gen-opt,$(t))))))

generate: $(GEN_OUTS)

ASM_TESTS = $(patsubst %.asm,%,$(wildcard gen_*.asm))
ASM_OUTS  = $(foreach f,$(ASM_TESTS),$(f).$(call out-ext,$(f)))
define asm-rule # source stem fmt
asm-$(2).$(3): $(1)
	asm -o $(2).$(3) -l $(2).lst $$^

endef
$(eval $(foreach f,$(ASM_TESTS),$(call asm-rule,$(f).asm,$(f),$(call out-ext,$(f)))))

asm-test: clean-asm $(ASM_OUTS:%=asm-%)
clean-asm:
	-rm -f $(ASM_OUTS)

ASL_exclude = gen_i8086 gen_ns32032
ASL_TESTS = $(foreach t,$(ASM_TESTS),$(if $(filter $(t),$(ASL_exclude)),,$(t)))
ASL_OUTS  = $(foreach f,$(ASL_TESTS),$(f).$(call out-ext,$(f)))
define asl-file # stem
$(if $(wildcard $(1).asl),$(1).asl,$(1).asm)
endef
define asl-rule # source stem suffix format
asl-$(2).$(3): $(1)
	asl $$(ASL_FLAGS) -L $$^
	p2hex $$(P2HEX_FLAGS) -F $(4) $(2).p $(2).$(3)

endef
$(eval $(foreach f,$(ASL_TESTS),$(call asl-rule,$(call asl-file,$(f)),$(f),$(call out-ext,$(f)),$(call out-fmt,$(f)))))

asl-test: clean-asl $(ASL_OUTS:%=asl-%)
clean-asl:
	-rm -f $(ASL_OUTS)


GAS_TESTS = $(wildcard gen_*.gas)
GAS_OUTS  = $(foreach f,$(GAS_TESTS),$(f:.gas=).$(call out-ext,$(f:.gas=)))
gas-test: clean-gas $(GAS_OUTS)
clean-gas:
	-rm -f $(GAS_OUTS)

clean-gen:
	-rm -f $(GEN_OUTS)

clean: clean-asm clean-asl clean-gas
	-rm -f *.p *.o *.lst

# Local Variables:
# mode: makefile-gmake
# End:
# vim: set ft=make:
