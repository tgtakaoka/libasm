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

# Macro Assembler asl target

ASL = asl -q -gnuerrors -relaxed -supmode
ASL_FLAGS = +t 0x0e
P2HEX_FLAGS = -q -k -r \$$-\$$ -l 16

define __asl-opt # target
$(if $(filter $(1),$(I32_OUTS)),-F Intel32,$(if $(filter $(1),$(S19_OUTS) $(S28_OUTS) $(S37_OUTS)),-F Moto +5,-F Intel))
endef
define asl-opt # file
$(call __asl-opt,$(subst gen_,,$(subst test_,,$(1))))
endef

# p2hex stores only the low 16 bits of each target word, so on a 32-bit word
# machine half of every instruction is lost (and the record length still counts
# the dropped bytes, leaving a malformed file).  Those targets go through p2bin,
# which keeps the whole word, and objcopy turns the image into Intel HEX.  The
# binary carries no addresses, so the load address is supplied here; it must
# match the word origin of the source times the word size.
P2BIN_OUTS = tms320c30 tms320c31 tms320c32
P2BIN_FLAGS = -q -k -r \$$-\$$
ORIGIN_tms320c30 = 0x300
ORIGIN_tms320c31 = 0x300
ORIGIN_tms320c32 = 0x300

define __asl-conv # target stem ext
$(if $(filter $(1),$(P2BIN_OUTS)),p2bin $(P2BIN_FLAGS) $(2).p $(2).bin && objcopy -I binary -O ihex --change-addresses=$(ORIGIN_$(1)) $(2).bin /dev/stdout | grep -v '^:040000030' | tr -d '\015' > $(2).$(3) && rm -f $(2).bin,p2hex $(P2HEX_FLAGS) $(call __asl-opt,$(1)) $(2).p $(2).$(3))
endef
# stem is the file base name (test_xxx / gen_xxx), ext the output suffix.
define asl-conv # stem ext
$(call __asl-conv,$(subst gen_,,$(subst test_,,$(1))),$(1),$(2))
endef

# Local Variables:
# mode: makefile-gmake
# End:
# vim: set ft=make:
