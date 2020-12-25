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

-include ../Makefile.vars
-include ../Makefile.asl
-include ../Makefile.gas

# target which has ".inc" suffix for output.
INC_targets  = w65c816 tlcs90 z8k1 z8k2 ns32k
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

ASM_TESTS = $(wildcard gen_*.asm)
asm-test: $(ASM_TESTS)
	@for s in $^; do \
	    x=$${s%.asm}; \
	    echo asm $$x.asm -o $$x.hex -l $$x.lst; \
	    asm $$x.asm -H32 -o $$x.hex -l $$x.lst; \
	done

ASL_exclude = gen_i8086.asm gen_ns32032.asm
ASL_TESTS = $(foreach t,$(ASM_TESTS),$(if $(filter $(t),$(ASL_exclude)),,$(t)))
asl-test: $(ASL_TESTS)
	@for s in $^; do \
	    x=$${s%.asm}; \
	    a=$$x.asm; [ -f $$x.asl ] && a=$$x.asl; \
	    echo "asl $(ASL_FLAGS) $$a && p2hex $(P2HEX_FLAGS) -F Intel $$x.p $$x.hex"; \
	    asl $(ASL_FLAGS) $$a && p2hex $(P2HEX_FLAGS) -F Intel $$x.p $$x.hex; \
	done

GAS_TESTS = $(wildcard gen_*.gas)
gas-test: $(GAS_TESTS:.gas=.hex)

clean-gen:
	-rm -f $(GEN_OUTS)
clean:
	-rm -f *.o *.lst *.hex

# Local Variables:
# mode: makefile-gmake
# End:
# vim: set ft=make:
