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
	@echo '"make check"       run test, pio-ci, and arduino-ci'
	@echo '"make test"        run test'
	@echo '"make pio-ci"      run PlatformIO CI for an example and a board'
	@echo  "                    (default BOARD=$(BOARD)), EX=$(EX))"
	@echo '"make arduino-ci"  run Arduino CLI for an example and a board'
	@echo  "                    (default FQBN=$(FQBN)), EX=$(EX))"

EXS = $(shell $(MAKE) -s -C examples examples)
EX ?= $(firstword $(EXS))

BOARDS = $(shell $(MAKE) -s -C examples boards)
BOARD = $(firstword $(BOARDS))

FQBNS = $(shell $(MAKE) -s -C examples fqbns)
FQBN = $(firstword $(FQBNS))

check: test pio-ci arduino-ci

test:
	$(MAKE) -C test test

pio-ci: examples
	$(MAKE) -C examples pio-ci

arduino-ci: examples
	$(MAKE) -C examples arduino-ci

clean: 
	$(MAKE) -s -C cli clean
	$(MAKE) -s -C test clean
	$(MAKE) -s -C examples clean
	rm -f $$(find . -type f -a -name '*~')
	rm -f $$(find . -type f -a -name '.ninja_*')

.PHONY: help clean check test pio-ci arduino-ci

# Local Variables:
# mode: makefile-gmake
# End:
# vim: set ft=make:
