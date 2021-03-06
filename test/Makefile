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

all:
	@echo '"make test"     run tests of libasm'
	@echo '"make gen"      build auto test source generator'
	@echo '"make generate" generate auto test sources'

-include ../src/Makefile.arch

.PHONY: all test gen generate clean clean-objs

generate: gen
	$(MAKE) -C autogen clean-gen
	$(MAKE) -C autogen generate

clean: clean-objs
	-rm -f $(BINS)
clean-objs:
	-rm -f $(OBJS) *.d *~

VPATH = ../src:../cli
INCLUDES = -I ../src -I ../cli
CXXFLAGS = -Wall -std=c++14 -O $(INCLUDES) $(DEBUG_FLAGS)
CPPFLAGS = -MMD

OBJS_test = test_asserter.o error_reporter.o
OBJS_gen  = dis_base.o value_formatter.o test_memory.o text_buffer.o \
	    cli_listing.o error_reporter.o
OBJS_asm  = asm_base.o test_asm_helper.o value_parser.o
OBJS_dis  = dis_base.o test_dis_helper.o value_formatter.o test_memory.o
OBJS_expr = test_expr_helper.o test_asserter.o error_reporter.o \
            value_parser.o value_formatter.o
$(eval $(foreach a,$(ARCHS),$(call objs-arch,$(a))))

TEST_OBJS = $(ARCHS:%=test_asm_%.o) $(ARCHS:%=test_dis_%.o)
EXPR_OBJS = test_expr_moto.o test_expr_intel.o test_expr_base.o
GEN_OBJS  = $(ARCHS:%=gen_%.o)
OBJS = 	$(TEST_OBJS) $(GEN_OBJS) $(OBJS_test) $(OBJS_gen) \
	$(OBJS_asm) $(OBJS_dis) $(OBJS_expr) \
	$(ARCHS:%=asm_%.o) $(ARCHS:%=dis_%.o) $(EXPR_OBJS) \
	$(foreach a,$(ARCHS),$(OBJS_$(a)))
BINS = $(TEST_OBJS:.o=) $(EXPR_OBJS:.o=) $(GEN_OBJS:.o=)

-include $(OBJS:.o=.d)

# make rule for testing an architecture.
define test-rule # arch
test_asm_$(1): test_asm_$(1).o asm_$(1).o $$(OBJS_$(1)) $$(OBJS_asm) $$(OBJS_test)
	$$(CXX) -o $$@ $$(CXXFLAGS) $$^
test_dis_$(1): test_dis_$(1).o dis_$(1).o $$(OBJS_$(1)) $$(OBJS_dis) $$(OBJS_test)
	$$(CXX) -o $$@ $$(CXXFLAGS) $$^
test_$(1): test_asm_$(1) test_dis_$(1)
	./test_asm_$(1)
	./test_dis_$(1)

endef
$(eval $(foreach a,$(ARCHS),$(call test-rule,$(a))))

# make rule for testing expression.
EXPRS = base moto intel
define test-expr # expr
test_expr_$(1): test_expr_$(1).o $$(OBJS_expr) $$(OBJS_test)
	$$(CXX) -o $$@ $$(CXXFLAGS) $$^

endef
$(eval $(foreach e,$(EXPRS),$(call test-expr,$(e))))

test_expr: $(EXPRS:%=test_expr_%)
	./test_expr_base
	./test_expr_moto
	./test_expr_intel
test: $(foreach a,$(ARCHS),test_$(a)) test_expr

define gen-rule
gen_$(1): gen_$(1).o dis_$(1).o $$(OBJS_$(1)) $$(OBJS_gen)
	$$(CXX) -o $$@ $$(CXXFLAGS) $$^

endef
$(eval $(foreach a,$(ARCHS),$(call gen-rule,$(a))))

gen: $(GEN_OBJS:.o=)

# Local Variables:
# mode: makefile-gmake
# End:
# vim: set ft=make:
