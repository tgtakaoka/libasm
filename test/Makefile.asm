# Copyright 2021 Tadashi G. Takaoka
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

# Macro Assembler asm target

ASM_FLAGS = -n

define __asm-opt # target
$(if $(filter $(1),$(S19_OUTS) $(S28_OUTS)),-S16,-H16)
endef
define asm-opt # file
$(call __asm-opt,$(subst gen_,,$(subst test_,,$(1))))
endef

# Local Variables:
# mode: makefile-gmake
# End:
# vim: set ft=make:
