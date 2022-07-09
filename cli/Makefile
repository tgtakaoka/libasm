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

BINS = asm dis
INSTALL_DIR ?= $(HOME)/.local/bin

help:
	@echo '"make cli"    build '"${BINS}"
	@echo '"make install [INSTALL_DIR='"${INSTALL_DIR}"']"'
	@echo '              install ${BINS} to INSTALL_DIR'
	@echo '"make clean"  remove unnecessary files'

-include ../src/Makefile.arch

.PHONY:: help install clean-objs clean tidy

cli: $(BINS)

install: $(BINS)
	install -s $(BINS) ${INSTALL_DIR}

clean-objs:
	-rm -f $(OBJS)
clean: clean-objs
	-rm -f $(BINS) *.d *~ compile_commands.json

VPATH = ../src
CXXFLAGS = -Wall -O -std=c++14 -I../src $(DEBUG_FLAGS)
CPPFLAGS = -MMD

OBJS_com = list_formatter.o bin_memory.o error_reporter.o file_reader.o file_printer.o \
	   bin_decoder.o bin_encoder.o intel_hex.o moto_srec.o str_buffer.o str_scanner.o \
	   $(foreach a,$(ARCHS),$(OBJS_$(a)))
OBJS_asm = asm.o asm_base.o asm_driver.o asm_directive.o value_parser.o function_store.o \
	   $(foreach a,$(ARCHS),$(if $(wildcard ../src/asm_$(a).cpp),asm_$(a).o))
OBJS_dis = dis.o dis_base.o dis_driver.o dis_directive.o value_formatter.o \
	   $(foreach a,$(ARCHS),$(if $(wildcard ../src/dis_$(a).cpp),dis_$(a).o))
OBJS_bin = $(BINS:=.o)
$(eval $(foreach a,$(ARCHS),$(call objs-arch,$(a))))

OBJS = $(OBJS_com) $(OBJS_asm) $(OBJS_dis) $(OBJS_bin) \
       $(foreach a,$(ARCHS),$(OBJS_$(a)))

define bin-rule # bin
$(1): $(OBJS_com) $(OBJS_$(1))
	$$(CXX) -o $$@ $$(CXXFLAGS) $$^

endef
$(eval $(foreach b,$(BINS),$(call bin-rule,$(b))))

compile_commands.json:
	bear $(MAKE) cli
SRCS=$(foreach s,$(sort $(OBJS:%.o=%.cpp)),$(if $(wildcard $(s)),$(s),../src/$(s)))
define tidy-rule # target
.PHONY:: $(1).tidy
$(1).tidy: compile_commands.json $(1).cpp
	clang-tidy $(1).cpp

endef
$(eval $(foreach t,$(SRCS:%.cpp=%),$(call tidy-rule,$(t))))
tidy: $(SRCS:%.cpp=%.tidy)

-include $(OBJS:.o=.d)

# Local Variables:
# mode: makefile-gmake
# End:
# vim: set ft=make:
