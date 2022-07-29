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

TESTS = test_array_memory test_str_buffer

.PHONY:: help test clean clean-objs

clean: clean-objs
	-rm -f $(BINS)
clean-objs:
	-rm -f *.o *.d *~

vpath %.cpp ../../src
vpath %.cpp ../../test

CXXFLAGS = -std=c++14 -Wall -O $(DEBUG_FLAGS)
CPPFLAGS = -MMD -I../../src -I../../test

OBJS_common = test_src_helper.o test_asserter.o error_reporter.o
OBJS = $(OBJS_common) $(TESTS:=.o)
BINS = $(TESTS)

-include $(OBJS:.o=.d)

define test-rule # test
test:: $(1)
	./$(1)

endef
$(eval $(foreach t,$(TESTS),$(call test-rule,$(t))))

test_array_memory: test_array_memory.o $(OBJS_common)
	$(CXX) -o $@ $^
test_str_buffer: test_str_buffer.o str_buffer.o $(OBJS_common)
	$(CXX) -o $@ $^

# Local Variables:
# mode: makefile-gmake
# End:
# vim: set ft=make:
