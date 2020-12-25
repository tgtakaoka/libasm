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

ASL_FLAGS = -gnuerrors -L +t 0x1e -q -w -relaxed
P2HEX_FLAGS = -q -k -r \$$-\$$ -l 32

%.hex: %.asl
	asl $(ASL_FLAGS) $^
	p2hex $(P2HEX_FLAGS) -F Intel $*.p $@
%.s19 %.s28 %.s37: %.asl
	asl $(ASL_FLAGS) $^
	p2hex $(P2HEX_FLAGS) -F Moto +5 $*.p $@

# Local Variables:
# mode: makefile-gmake
# End:
# vim: set ft=make:
