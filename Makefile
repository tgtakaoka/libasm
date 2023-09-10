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
	@echo '"make clean"   remove unnecessary files'
	@echo '"make test"    run test'
	@echo '"make full-pio run PlatformIO CI for all examples and boards'
	@echo '"make a-pio"   run PlatformIO CI for an example and a board'

PIO_CI_BOARDS=ATmega644P nano_every nano_33_iot pico
_BOARDS=$(foreach b,$(PIO_CI_BOARDS),--board=$(b))

test:
	$(MAKE) -C test test

a-pio:
	@echo PIO_FLAGS=$(PIO_FLAGS)

full-pio: examples
	@for ex in $(shell make -s -C examples examples); do \
	    echo pio $(PIO_FLAGS) ci -l . $(PIO_CI_FLAGS) $(_BOARDS) examples/$${ex}/$${ex}.ino; \
	    pio $(PIO_FLAGS) ci -l . $(PIO_CI_FLAGS) $(_BOARDS) examples/$${ex}/$${ex}.ino; \
	done

clean: 
	$(MAKE) -C cli clean
	$(MAKE) -C test clean
	$(MAKE) -C examples clean
	rm -f $$(find . -type f -a -name '*~')
	rm -f $$(find . -type f -a -name '.ninja_log')

.PHONY: help clean test a-pio full-pio

# Local Variables:
# mode: makefile-gmake
# End:
# vim: set ft=make:
