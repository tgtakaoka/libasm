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
	@echo '"make gen" auto generate test sources'
	@echo '"make asm-test" assemble with "asm" command'
	@echo '"make asl-test" assemble with "asl" command'
	@echo '"make gas-test" assemble with "gas" command'

-include ../../src/Makefile.arch
-include ../Makefile.asm
-include ../Makefile.asl
-include ../Makefile.gas

.PHONY:: gen asm-test asl-test gas-test
.PHONY:: clean clean-gen clean-asm clean-asl clean-gas

# target which has ".inc" suffix for output.
INC_targets = w65c816 tlcs90 i8086 i80186 \
    mc68000 mc68010 mc68020 mc68030 z8001 z8002 ns32000 mn1610 mn1613 \
    tms370 tms7000 tms9980 j11 t11 mc146805 z80 z180 z380
# target which for GNU assembler (also ".ginc" targets)
GINC_targets = mc68000 mc68010 mc68020 mc68030 z8001 z8002 ns32000 \
    mc68hc11 mc68hc12 i8086 i80186 i80286
# target which also has Zilog syntax (also ".zinc" targets)
ZINC_targets = i8080 i8085 v30emu
# target which doesn't need ".asm"
NOASM_targets = i80286
# target which ASL could not assemble
ASL_exclude = gen_mb8861 gen_i80286 gen_z280

# file name of generating file
define out-inc # target
$(filter $(1),$(INC_targets) $(ZINC_targets:%=%_inc))
endef
define gen-out # target
$(if $(filter $(1),$(GINC_targets:%=%_ginc)),gen_$(1:%_ginc=%).ginc,\
$(if $(filter $(1),$(ZINC_targets:%=%_zinc)),gen_$(1:%_zinc=%).zinc,\
$(if $(filter $(1),$(NOASM_targets)),,\
$(if $(call out-inc,$(1)),gen_$(1).inc,gen_$(1).asm))))
endef

# generator options
define ginc-out
$(filter $(1),$(GINC_targets:%=%_ginc))
endef
define zinc-out
$(filter $(1),$(ZINC_targets:%=%_zinc))
endef
define zinc-out
$(filter $(1),$(ZINC_targets:%=%_zinc))
endef
define gen-opt
$(if $(call ginc-out,$(1)),-g -i,$(if $(call out-inc,$(1)),-i -u,-u)) \
$(if $(call zinc-out,$(1)),-z)
endef
define arch-target # arch
$(foreach t,$(TGT_$(1)),$(t) \
  $(if $(filter $(t),$(ZINC_targets)),$(t)_zinc) \
  $(if $(filter $(t),$(GINC_targets)),$(t)_ginc))
endef
define cpu-opt # out
$(if $(call zinc-out,$(1)),-C $(CPU_$(1:%_zinc=%)),\
$(if $(call ginc-out,$(1)),-C $(CPU_$(1:%_ginc=%)),-C $(CPU_$(1))))
endef

# make rule for auto generation file
define gen-target # arch target out opt
	../gen_$(1) $(4) $(call cpu-opt,$(2)) -o $(3) -l gen_$(2).lst
endef
define gen-target-rule # arch target out opt
$(3): #../gen_$(1)
	$(call gen-target,$(1),$(2),$(3),$(4))

endef
define gen-arch-rule # arch
clean_$(1):
	rm -f $(foreach t,$(call arch-target,$(1)),$(call gen-out,$(t)))
gen_$(1): clean_$(1) $(foreach t,$(call arch-target,$(1)),$(call gen-out,$(t)))

endef

GEN_OUTS = $(foreach a,$(ARCHS),$(foreach t,$(call arch-target,$(a)),$(call gen-out,$(t))))

# rules for auto generations
$(eval $(foreach a,$(ARCHS),$(call gen-arch-rule,$(a))))
$(eval $(foreach a,$(ARCHS),$(foreach t,$(call arch-target,$(a)),\
	$(call gen-target-rule,$(a),$(t),$(call gen-out,$(t)),$(call gen-opt,$(t))))))

gen: $(foreach a,$(ARCHS),gen_$(a))

ASM=../../cli/asm
$(ASM)-refresh:
	+$(MAKE) -C ../../cli asm

ASM_TESTS = $(patsubst %.asm,%,$(wildcard gen_*.asm))
ASM_OUTS  = $(foreach f,$(ASM_TESTS),$(f).$(call out-ext,$(f)))
define asm-rule # source stem suffix option
.PHONY:: asm-$(2).$(3)
asm-$(2).$(3):: $(ASM)-refresh
asm-$(2).$(3):: $(1)
	$$(ASM) $$(ASM_FLAGS) $(4) -o $(2).$(3) -l $(2).lst $$^

endef
$(eval $(foreach f,$(ASM_TESTS),$(call asm-rule,$(f).asm,$(f),$(call out-ext,$(f)),$(call asm-opt,$(f)))))

asm-test: $(ASM)-refresh clean-asm $(ASM_OUTS:%=asm-%)
clean-asm:
	-rm -f $(ASM_OUTS)

ASL_SRCS = $(foreach t,$(ASM_TESTS),$(if $(filter $(t),$(ASL_exclude)),,$(t)))
ASL_TESTS = $(ASL_SRCS) \
	$(foreach t,$(patsubst %.asl,%,$(wildcard gen_*.asl)),$(if $(filter $(t),$(ASL_SRCS)),,$(t)))
ASL_OUTS  = $(foreach f,$(ASL_TESTS),$(f).$(call out-ext,$(f)))
define asl-file # stem
$(if $(wildcard $(1).asl),$(1).asl,$(1).asm)
endef
define asl-rule # source stem suffix option
.PHONY:: asl-$(2).$(3)
asl-$(2).$(3): $(1)
	$$(ASL) $$(ASL_FLAGS) -L $$^ && p2hex $$(P2HEX_FLAGS) $(4) $(2).p $(2).$(3)

endef
$(eval $(foreach f,$(ASL_TESTS),$(call asl-rule,$(call asl-file,$(f)),$(f),$(call out-ext,$(f)),$(call asl-opt,$(f)))))

asl-test: clean-asl $(ASL_OUTS:%=asl-%)
clean-asl:
	-rm -f $(ASL_OUTS)


GAS_TESTS = $(wildcard gen_*.gas)
GAS_OUTS    = $(foreach f,$(GAS_TESTS),$(f:.gas=).$(call out-ext,$(f:.gas=)))
gas-test: clean-gas $(GAS_OUTS:%=gas-%)
clean-gas:
	-rm -f $(GAS_OUTS)

clean-gen:
	-rm -f $(GEN_OUTS)

clean: clean-asm clean-asl clean-gas
	-rm -f *.p *.o *.lst *.dis

# Local Variables:
# mode: makefile-gmake
# End:
# vim: set ft=make:
