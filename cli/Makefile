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

all: $(BINS)

-include ../src/Makefile.arch

.PHONY: all install clean-objs clean

install: $(BINS)
	install -s $(BINS) ${INSTALL_DIR}

clean-objs:
	-rm -f $(OBJS)
clean: clean-objs
	-rm -f $(BINS) *.d *~

VPATH = ../src
CXXFLAGS = -Wall -O -std=c++14 -I../src
CPPFLAGS = -MMD

OBJS_com = cli_listing.o cli_memory.o error_reporter.o file_util.o \
	   bin_formatter.o intel_hex.o moto_srec.o str_buffer.o \
	   $(foreach a,$(ARCHS),$(OBJS_$(a)))
OBJS_asm = asm.o asm_base.o asm_driver.o asm_directive.o value_parser.o \
	   $(foreach a,$(ARCHS),asm_$(a).o)
OBJS_dis = dis.o dis_base.o dis_driver.o dis_directive.o value_formatter.o \
	   $(foreach a,$(ARCHS),dis_$(a).o)
OBJS_bin = $(BINS:=.o)
$(eval $(foreach a,$(ARCHS),$(call objs-arch,$(a))))

OBJS = $(OBJS_com) $(OBJS_asm) $(OBJS_dis) $(OBJS_bin) \
       $(foreach a,$(ARCHS),$(OBJS_$(a)))

define bin-rule # bin
$(1): $(OBJS_com) $(OBJS_$(1))
	$$(CXX) -o $$@ $$(CXXFLAGS) $$^

endef
$(eval $(foreach b,$(BINS),$(call bin-rule,$(b))))

-include $(OBJS:.o=.d)

# Local Variables:
# mode: makefile-gmake
# End:
# vim: set ft=make:
