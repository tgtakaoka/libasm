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
	@echo '"make clean"  remove unnecessary files'
	@echo '"make pio"    run PlatformIO CI'

PIO_CI_BOARDS=nano_every nano_33_iot pico
_BOARDS=$(foreach b,$(PIO_CI_BOARDS),--board=$(b))

pio: examples
	@for ex in $(shell make -s -C examples examples); do \
	    echo pio ci --lib="." $(PIO_CI_FLAGS) $(_BOARDS) examples/$${ex}/$${ex}.ino; \
	    pio ci --lib="." $(PIO_CI_FLAGS) $(_BOARDS) examples/$${ex}/$${ex}.ino; \
	done

clean: 
	$(MAKE) -C cli clean
	$(MAKE) -C test clean
	$(MAKE) -C test/unittest clean
	$(MAKE) -C test/autogen clean
	$(MAKE) -C test/reference clean
	$(MAKE) -C driver/test clean
	$(MAKE) -C examples clean
	rm -f $$(find . -type f -a -name '*~')
	rm -f $$(find . -type f -a -name '.ninja_log')

.PHONY: help clean pio

# Local Variables:
# mode: makefile-gmake
# End:
# vim: set ft=make:
