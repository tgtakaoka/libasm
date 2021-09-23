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

all:
	@echo '"make clean"  remove unnecessary files'
	@echo '"make update" propagate platformio.common to sketches'

update: platformio.common
	@for dir in asm_* dis_*; do \
	    echo cp -f platformio.common $${dir}/platformio.ini; \
	    cp -f platformio.common $${dir}/platformio.ini; \
	done

clean: 
	rm -rf $$(find . -type d -a -name .pio)

.PHONY: all clean update

# Local Variables:
# mode: makefile-gmake
# End:
# vim: set ft=make:
