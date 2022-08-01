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
ASL_FLAGS = +t 0x1e
P2HEX_FLAGS = -q -k -r \$$-\$$ -l 16

define __asl-opt # target
$(if $(filter $(1),$(I32_OUTS)),-F Intel32,$(if $(filter $(1),$(S19_OUTS) $(S28_OUTS)),-F Moto +5,-F Intel))
endef
define asl-opt # file
$(call __asl-opt,$(subst gen_,,$(subst test_,,$(1))))
endef

# Local Variables:
# mode: makefile-gmake
# End:
# vim: set ft=make:
