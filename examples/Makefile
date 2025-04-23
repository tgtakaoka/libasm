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
	@echo '"make pio-ci"      run PlatformIO CI for an example and a board'
	@echo  "                    (default BOARD=$(BOARD), EX=$(EX))"
	@echo '"make pio-run"     build an example for an environment'
	@echo  "                    (default ENV=$(ENV), EX=$(EX))"
	@echo '"make arduino-ci"  run Arduino CLI for an example and a board'
	@echo  "                    (default FQBN=$(FQBN), EX=$(EX))"
	@echo '"make update"      propagate common files to sketches'

EXS = \
	cdp1802  \
	f3850    \
	i8048    \
	i8051    \
	i8080    \
	i8086    \
	i8096    \
	ins8060  \
	ins8070  \
	mc6800   \
	mc68000  \
	mc6809   \
	mn1610   \
	mos6502  \
	ns32000  \
	pdp11    \
	pdp8     \
	scn2650  \
	tlcs90   \
	tms32010 \
	tms370   \
	tms7000  \
	tms9900  \
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

BOARDS = $(shell $(MAKE) -s -C $(EX) boards)
BOARD ?= $(firstword $(BOARDS))

FQBNS = $(shell $(MAKE) -s -C $(EX) fqbns)
FQBN ?= $(firstword $(FQBNS))

boards:
	@echo $(BOARDS)

envs:
	@echo $(ENVS)

fqbns:
	@echo $(FQBNS)

define update # example
	cp -f Makefile.common $(1)/Makefile
	cp -f platformio.common $(1)/platformio.ini

endef

update: Makefile.common platformio.common
	$(foreach ex,$(EXS),$(call update,$(ex)))

define pio-ci # example, board
	$(MAKE) -C $(1) pio-ci BOARD=$(2)

endef

pio-ci:
	$(call pio-ci,$(EX),$(BOARD))

pio-ci-all:
	$(foreach ex,$(EXS),$(foreach board,$(BOARDS),$(call pio-ci,$(ex),$(board))))

define pio-run # example, env
	$(MAKE) -C $(1) pio-run ENV=$(2)

endef

pio-run:
	$(call pio-run,$(EX),$(ENV))

pio-run-all:
	$(foreach ex,$(EXS),$(foreach env,$(ENVS),$(call pio-run,$(ex),$(env))))

define arduino-ci # example, fqbn
	$(MAKE) -C $(1) arduino-ci FQBN=$(2)

endef

arduino-ci:
	$(call arduino-ci,$(EX),$(FQBN))

arduino-ci-all:
	$(foreach ex,$(EXS),$(foreach fqbn,$(FQBNS),$(call arduino-ci,$(ex),$(fqbn))))

define clean-ex # ex
	$(MAKE) -s -C $(1) clean

endef

clean: $(EXS)
	$(foreach ex,$(EXS),$(call clean-ex,$(ex)))

.PHONY: all clean update pio-ci pio-ci-all pio-run pio-run-all
.PHONY: arduino-ci arduino-ci-all

# Local Variables:
# mode: makefile-gmake
# End:
# vim: set ft=make:
