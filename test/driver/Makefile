# Copyright 2022 Tadashi G. Takaoka
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
	@echo '"make test"     run tests of libasm::driver'

.PHONY:: help test clean clean-objs

clean: clean-objs
	-rm -f $(BINS)
clean-objs:
	-rm -f *.o *.d *~ compile_commands.json

include ../../src/Makefile.arch

SRCS_TEST_FORMATTER = $(wildcard test_formatter_*.cpp)
TESTS = test_helpers test_bin_memory test_intel_hex test_moto_srec \
	test_asm_formatter $(SRCS_TEST_FORMATTER:%.cpp=%)

vpath %.cpp ../../driver
vpath %.cpp ../../src
vpath %.cpp ../../test

CXXFLAGS = -std=c++14 -Wall -O $(DEBUG_FLAGS)
CPPFLAGS = -I../../driver -I../../src -I../../test -MD -MF $@.d

OBJS_common = \
    test_driver_helper.o test_asserter.o error_reporter.o str_scanner.o \
    text_common.o
OBJS_encdec = \
    bin_encoder.o bin_decoder.o intel_hex.o moto_srec.o bin_memory.o
OBJS_formatter = \
    list_formatter.o value_formatter.o config_base.o formatters.o reg_base.o \
    option_base.o str_buffer.o bin_memory.o dis_base.o dis_formatter.o \
    dis_driver.o
OBJS_asm = \
    asm_base.o asm_formatter.o value.o value_parser.o parsers.o operators.o \
    asm_driver.o asm_directive.o function_store.o symbol_store.o bin_encoder.o \
    bin_decoder.o moto_srec.o intel_hex.o insn_base.o
ARCHS_test_asm_formatter = i8080 ins8060 mc6809 mos6502 z80
OBJS_test_asm_formatter = $(OBJS_formatter) $(OBJS_common) $(OBJS_asm) \
        $(foreach a,$(ARCHS_test_asm_formatter),asm_$(a).o $(OBJS_$(a)))

BINS = $(TESTS)
OBJS = $(OBJS_common) $(OBJS_encdec) $(OBJS_formatter) $(OBJS_asm) \
       $(SRCS_TEST_FORMATTER:%.cpp=%.o)

-include $(OBJS:%=%.d)

define test-rule # test
test:: $(1)
	./$(1)

endef
$(eval $(foreach t,$(TESTS),$(call test-rule,$(t))))

define test-formatter # arch
test_formatter_$(1): test_formatter_$(1).o $(OBJS_asm) $(OBJS_dis) $(OBJS_common) \
                     $(OBJS_formatter) asm_$(1).o dis_$(1).o $(OBJS_$(1))
	$$(CXX) -o $$@ $$^

endef
$(eval $(foreach a,$(ARCHS),$(call test-formatter,$(a))))

test_helpers: test_helpers.o bin_memory.o $(OBJS_common)
	$(CXX) -o $@ $^
test_bin_memory: test_bin_memory.o bin_memory.o $(OBJS_common)
	$(CXX) -o $@ $^
test_intel_hex: test_intel_hex.o $(OBJS_encdec) $(OBJS_common)
	$(CXX) -o $@ $^
test_moto_srec: test_moto_srec.o $(OBJS_encdec) $(OBJS_common)
	$(CXX) -o $@ $^
test_asm_formatter: test_asm_formatter.o $(OBJS_test_asm_formatter)
	$(CXX) -o $@ $^

compile_commands.json:
	bear -- $(MAKE) test
SRCS=\
$(foreach s,$(sort $(OBJS:%.o=%.cpp)),\
    $(if $(wildcard $(s)),$(s),\
    $(if $(wildcard ../$(s)),../$(s),)))
define tidy-rule # target
.PHONY:: $(1).tidy
$(1).tidy: compile_commands.json $(1).cpp
	clang-tidy $(1).cpp

endef
$(eval $(foreach t,$(SRCS:%.cpp=%),$(call tidy-rule,$(t))))
tidy: $(SRCS:%.cpp=%.tidy)

# Local Variables:
# mode: makefile-gmake
# End:
# vim: set ft=make:
