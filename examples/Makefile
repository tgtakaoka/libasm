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

help:
	@echo '"make clean"       remove unnecessary files'
	@echo '"make pio-run"     build an example for an environment'
	@echo  "                    (default ENV=$(ENV), EX=$(EX))"
	@echo '"make update"      propagate common files to examples'

EXS = \
	am29000  \
	c166     \
	cdp1802  \
	cp1600   \
	f3850    \
	h16      \
	h8300    \
	h8500    \
	i8048    \
	i8051    \
	i8080    \
	i8086    \
	i8096    \
	i960     \
	ins8060  \
	ins8070  \
	mc6800   \
	mc68000  \
	mc6805   \
	mc6809   \
	mc68hc12 \
	mc68hc16 \
	mn1610   \
	mos6502  \
	ns32000  \
	pdp11    \
	pdp8     \
	scn2650  \
	superh   \
	tlcs90   \
	tlcs900  \
	tms320   \
	tms320f  \
	tms370   \
	tms7000  \
	tms9900  \
	z280     \
	z380     \
	z8       \
	z80      \
	z8000

EX ?= $(firstword $(EXS))

examples:
	@echo $(EXS)

ENVS = $(shell $(MAKE) -s -C $(EX) envs)
ENV ?= $(firstword $(ENVS))

environments:
	@echo $(ENVS)

envs:
	@echo $(ENVS)

define update # example
	cp -f Makefile.common $(1)/Makefile
	cp -f platformio.common $(1)/platformio.ini

endef

update: Makefile.common platformio.common
	$(foreach ex,$(EXS),$(call update,$(ex)))

define pio-run # example, env
	$(MAKE) -C $(1) pio-run ENV=$(2)

endef

pio-run:
	$(call pio-run,$(EX),$(ENV))

pio-run-all:
	$(foreach ex,$(EXS),$(call pio-run,$(ex),$(ENV)))

define clean-ex # ex
	$(MAKE) -s -C $(1) clean

endef

clean: $(EXS)
	$(foreach ex,$(EXS),$(call clean-ex,$(ex)))

.PHONY: all clean update pio-run pio-run-all

# Local Variables:
# mode: makefile-gmake
# End:
# vim: set ft=make:
