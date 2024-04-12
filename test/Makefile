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

help:
	@echo '"make test"     run tests of libasm'
	@echo '"make gen"      build auto test source generator'
	@echo '"make generate" generate auto test sources'

-include ../src/Makefile.arch

.PHONY:: help test gen generate clean clean-objs tidy

generate: gen
	$(MAKE) -C autogen clean-gen
	$(MAKE) -C autogen generate

clean: clean-test
	$(MAKE) -C driver clean
	$(MAKE) -C unit clean
	$(MAKE) -C autogen clean
	$(MAKE) -C reference clean

clean-test:
	-rm -f $(BINS)
	-rm -f $(OBJS) *.d *~ compile_commands.json

vpath %.cpp ../src
vpath %.cpp ../driver

CXXFLAGS = -std=c++14 -Wall -O $(DEBUG_FLAGS) -DLIBASM_DEBUG_VALUE
CPPFLAGS = -I../src -I../driver -MD -MF $@.d

OBJS_test = \
    test_asserter.o error_reporter.o option_base.o str_buffer.o str_scanner.o
OBJS_asm  = test_asm_helper.o asm_base.o \
    config_base.o insn_base.o operators.o parsers.o reg_base.o value.o \
    value_parser.o text_common.o \
    fixed64.o ieee_float.o float80_base.o float80_hard.o float80_soft.o
OBJS_dis  = test_dis_helper.o dis_base.o \
    config_base.o formatters.o insn_base.o reg_base.o value.o \
    value_formatter.o text_common.o \
    fixed64.o ieee_float.o float80_base.o float80_hard.o float80_soft.o \
    dec_float.o
OBJS_expr = test_expr_helper.o \
    config_base.o formatters.o insn_base.o operators.o parsers.o \
    value.o value_formatter.o value_parser.o text_common.o \
    fixed64.o ieee_float.o float80_base.o float80_hard.o float80_soft.o
OBJS_gen  = gen_driver.o test_generator.o tokenized_text.o \
    config_base.o dis_base.o dis_formatter.o formatters.o insn_base.o \
    list_formatter.o reg_base.o value.o value_formatter.o text_common.o \
    fixed64.o ieee_float.o float80_base.o float80_hard.o float80_soft.o \
    dec_float.o

TEST_OBJS = $(ARCHS:%=test_asm_%.o) $(ARCHS:%=test_dis_%.o)
EXPR_OBJS = $(EXPRS:%=test_expr_%.o)
GEN_OBJS  = $(ARCHS:%=gen_%.o)
OBJS = $(TEST_OBJS) $(OBJS_asm) $(OBJS_dis) $(OBJS_test) \
       $(EXPR_OBJS) $(OBJS_expr) \
       $(GEN_OBJS) $(OBJS_gen) \
       $(ARCHS:%=asm_%.o) $(ARCHS:%=dis_%.o) \
       $(foreach a,$(ARCHS),$(OBJS_$(a)))
BINS = $(TEST_OBJS:.o=) $(EXPR_OBJS:.o=) $(GEN_OBJS:.o=)

-include $(OBJS:%=%.d)

# make rule for testing an architecture.
define test-rule # arch
test_asm_$(1): test_asm_$(1).o asm_$(1).o text_$(1).o $$(OBJS_$(1)) $$(OBJS_asm) $$(OBJS_test)
	$$(CXX) -o $$@ $$(CXXFLAGS) $$^
test_dis_$(1): test_dis_$(1).o dis_$(1).o text_$(1).o $$(OBJS_$(1)) $$(OBJS_dis) $$(OBJS_test)
	$$(CXX) -o $$@ $$(CXXFLAGS) $$^
test_$(1): $(if $(wildcard test_asm_$(1).cpp),test_asm_$(1)) \
           $(if $(wildcard test_dis_$(1).cpp),test_dis_$(1))
	$(if $(wildcard test_asm_$(1).cpp),./test_asm_$(1))
	$(if $(wildcard test_dis_$(1).cpp),./test_dis_$(1))

endef
$(eval $(foreach a,$(ARCHS),$(call test-rule,$(a))))

# make rule for testing expression.
EXPRS = base fairchild intel motorola national zilog
define test-expr # expr
test_expr_$(1): test_expr_$(1).o $$(OBJS_expr) $$(OBJS_test)
	$$(CXX) -o $$@ $$(CXXFLAGS) $$^

endef
$(eval $(foreach e,$(EXPRS),$(call test-expr,$(e))))

define exec-test # test
test_expr:: test_expr_$(1)
	./test_expr_$(1)

endef
$(eval $(foreach e,$(EXPRS),$(call exec-test,$(e))))

test: $(foreach a,$(ARCHS),test_$(a)) test_expr driver-test unit-test

driver-test:
	$(MAKE) -C driver test

unit-test:
	$(MAKE) -C unit test

define gen-rule
gen_$(1): $(if $(wildcard gen_$(1).cpp),gen_$(1).o) \
          dis_$(1).o text_$(1).o $$(OBJS_$(1)) $$(OBJS_gen) $$(OBJS_test)
	$(if $(wildcard gen_$(1).cpp),$$(CXX) -o $$@ $$(CXXFLAGS) $$^)

endef
$(eval $(foreach a,$(ARCHS),$(call gen-rule,$(a))))

gen: $(GEN_OBJS:.o=)

compile_commands.json:
	bear -- $(MAKE) $(BINS:%=%.o)
define tidy-rule # target
.PHONY:: $(1).tidy
$(1).tidy: compile_commands.json $(1).cpp
	clang-tidy $(1).cpp

endef
SRCS=\
$(foreach s,$(sort $(OBJS:%.o=%.cpp)),\
    $(if $(wildcard $(s)),$(s),\
    $(if $(wildcard ../src/$(s)),../src/$(s),)))
$(eval $(foreach t,$(SRCS:%.cpp=%),$(call tidy-rule,$(t))))
tidy: $(SRCS:%.cpp=%.tidy)

# Local Variables:
# mode: makefile-gmake
# End:
# vim: set ft=make:
