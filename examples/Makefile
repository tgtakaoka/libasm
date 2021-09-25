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
	@echo '"make update" propagate common files to sketches'

EXAMPLES = $(wildcard asm_* dis_*)

pio: update $(EXAMPLES)
	for ex in $(EXAMPLES); do \
	    $(MAKE) -C $${ex} pio PIO_RUN_FLAGS=--silent; \
	done

update: Makefile.common platformio.common
	for ex in $(EXAMPLES); do \
	    cp -f Makefile.common $${ex}/Makefile; \
	    cp -f platformio.common $${ex}/platformio.ini; \
	done

clean: $(EXAMPLES)
	@for ex in $(EXAMPLES); do \
	    $(MAKE) -C $${ex} clean; \
	done

.PHONY: all clean update

# Local Variables:
# mode: makefile-gmake
# End:
# vim: set ft=make:
