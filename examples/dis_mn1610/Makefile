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
	@echo '"make clean"    remove unnecessary files'
	@echo '"make pio"      run PlatformIO CI'

PIO_FLAGS ?= --no-ansi
PIO_ENVS = $(shell grep -Po '^\[env:\K[^]]+' platformio.ini)
PIO_BOARDS = $(shell grep -Po '^board *= *\K[\w]+' platformio.ini)
LIB_ENVS = $(PIO_ENVS:%=.pio/libdeps/%)

install-local-lib:
	for lib in $(LIB_ENVS); do \
	    test -h $${lib}/libasm && continue; \
	    mkdir -p $${lib} && rm -rf $${lib}/libasm; \
	    ln -s $${PWD}/../.. $${lib}/libasm; \
	done

pio: install-local-lib
	pio $(PIO_FLAGS) run $(PIO_RUN_FLAGS) $(PIO_ENVS:%=-e %)

pio-envs: platformio.ini
	@echo $(PIO_ENV)
pio-boards: platformio.init
	@echo $(PIO_BOARDS)

clean:
	rm -rf $$(find . -type d -a -name .pio)

.PHONY: help clean pio pio-boards pio-envs install-local-lib

# Local Variables:
# mode: makefile-gmake
# End:
# vim: set ft=make:
