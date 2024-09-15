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
	@echo '"make test"     run unit tests of libasm::src'

TESTS = test_array_memory test_fixed64 test_float32 test_float64 \
    test_float80_hard test_float80_soft test_reg_base test_str_buffer \
    test_str_scanner test_value

.PHONY:: help test clean clean-objs

clean: clean-objs
	-rm -f $(BINS)
clean-objs:
	-rm -f *.o *.d *~

vpath %.cpp ../../src
vpath %.cpp ../../test

CXXFLAGS = -std=c++14 -Wall -O $(DEBUG_FLAGS) -DLIBASM_DEBUG_VALUE
CPPFLAGS = -I../../src -I../../test -MD -MF $@.d

OBJS_common = test_unit_helper.o test_asserter.o error_reporter.o \
   fixed64.o float32.o float64.o float80_base.o float80_hard.o float80_soft.o \
   str_buffer.o str_scanner.o text_common.o value.o
OBJS = $(OBJS_common) $(TESTS:=.o)
BINS = $(TESTS)

-include $(OBJS:%=%.d)

define test-rule # test
test:: $(1)
	./$(1)

endef
$(eval $(foreach t,$(TESTS),$(call test-rule,$(t))))

test_array_memory: test_array_memory.o $(OBJS_common)
	$(CXX) -o $@ $^
test_fixed64: test_fixed64.o $(OBJS_common)
	$(CXX) -o $@ $^
test_float32: test_float32.o $(OBJS_common)
	$(CXX) -o $@ $^
test_float64: test_float64.o $(OBJS_common)
	$(CXX) -o $@ $^
test_float80_hard: test_float80_hard.o $(OBJS_common)
	$(CXX) -o $@ $^
test_float80_soft: test_float80_soft.o $(OBJS_common)
	$(CXX) -o $@ $^
test_reg_base: test_reg_base.o reg_base.o $(OBJS_common)
	$(CXX) -o $@ $^
test_str_buffer: test_str_buffer.o $(OBJS_common)
	$(CXX) -o $@ $^
test_str_scanner: test_str_scanner.o $(OBJS_common)
	$(CXX) -o $@ $^
test_value: test_value.o $(OBJS_common)
	$(CXX) -o $@ $^

# Local Variables:
# mode: makefile-gmake
# End:
# vim: set ft=make:
