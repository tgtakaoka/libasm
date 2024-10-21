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
	@echo '"make pio-ci"      run PlatformIO CI for a board'
	@echo  "                      (default BOARD=$(BOARD))"
	@echo '"make pio-ci-all"  run PlatformIO CI for all boards'
	@echo  "                      ($(BOARDS))"
	@echo '"make pio-run"     build sketch for an environment'
	@echo  "                      (default ENV=$(ENV))"
	@echo '"make pio-run-all" build sketch for all environments'
	@echo  "                      ($(ENVS))"

ifdef INSIDE_EMACS
ifeq ($(filter $(PIO_FLAGS),"--no-ansi"),)
PIO_FLAGS += --no-ansi
endif
ifeq ($(filter $(ARDUINO_FLAGS),"--no-color"),)
ARDUINO_FLAGS += --no-color
endif
endif

PIO_CI_FLAGS =
ARDUINO_CI_FLAGS = --warnings all --library $(CURDIR)/../../

ENVS = $(shell grep -Po '^\[env:\K[^]]+' platformio.ini)
ENV ?= $(firstword $(ENVS))

BOARDS = $(shell grep -Po '^board *= *\K[\w]+' platformio.ini)
BOARD ?= $(firstword $(BOARDS))

EXAMPLE = $(shell basename $(CURDIR))

define pio-libdep-path # env libdep
-l .pio/libdeps/$(1)/$(2)
endef

define pio-ci # board
	ENV=$(shell awk 'BEGIN{RS="\n\n"} /$(1)/' platformio.ini | grep -Po '(?<=env:)[^]]+')
	pio $(PIO_FLAGS) pkg install -l libcli
	pio $(PIO_FLAGS) ci -l $(CURDIR)/../.. $(call pio-libdep-path,$(ENV),libcli) -b $(1) $(EXAMPLE).ino

endef

pio-ci:
	$(call pio-ci,$(BOARD))

pio-ci-all:
	$(foreach board,$(BOARDS),$(call pio-ci,$(board)))

boards: platformio.ini
	@echo $(BOARDS)

define pio-run # env
	pio $(PIO_FLAGS) run $(PIO_RUN_FLAGS) -e $(1)

endef

pio-run:
	$(call pio-run,$(ENV))

pio-run-all:
	$(foreach env,$(ENVS),$(call pio-run,$(env)))

envs: platformio.ini
	@echo $(ENVS)

define arduino-ci # fqbn
	arduino-cli $(ARDUINO_FLAGS) compile -b $(1) $(ARDUINO_CI_FLAGS)

endef

FQBNS = \
	MightyCore:avr:1284  \
	DxCore:megaavr:avrda \
	teensy:avr:teensy41
FQBN ?= $(firstword $(FQBNS))

fqbns:
	@echo $(FQBNS)

arduino-ci:
	$(call arduino-ci,$(FQBN))

arduino-ci-all:
	$(foreach fqbn,$(FQBNS),$(call arduino-ci,$(fqbn)))

clean:
	rm -rf $$(find . -type d -a -name .pio)

.PHONY: help clean pio-run pio-run-all pio-ci pio-ci-all pio-boards pio-envs \
	arduino-ci arduino-ci-all

# Local Variables:
# mode: makefile-gmake
# End:
# vim: set ft=make:
