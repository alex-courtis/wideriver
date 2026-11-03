PKGS_TST = cmocka

CFLAGS += $(foreach p,$(PKGS_TST),$(shell pkg-config --cflags $(p)))
LDLIBS += $(foreach p,$(PKGS_TST),$(shell pkg-config --libs $(p)))

INCS += -Ilib/alex-c/tst

CFLAGS += -Wno-unused-function

$(TST_O): $(INC_H) $(TST_H) $(PRO_H) config.mk GNUmakefile tst/test.mk

# test executables exclude:
#   main
#   other tst-x.o
$(TST_E): $(filter-out src/main.o,$(SRC_O)) $(PRO_O) $(LIB_O) $(filter-out tst/tst%,$(TST_O))

# test-x builds tst/tst-x and executes it
test: $(patsubst tst/tst%,test%,$(TST_E))
test-%: tst/tst-%
	@rm -f actual.* expected.* unexpected.*
	./$(^)

# test-x-vg builds tst/tst-x and executes it with valgrind
test-vg: $(patsubst tst/tst%,test%-vg,$(TST_E))
test-%-vg: tst/tst-%
	$(VALGRIND) ./$(^)

#
# common mocks
#
#

# log
LDFLAGS = -Wl,$\
		  --wrap=log_f,--wrap=log_e,--wrap=log_w,--wrap=log_i,--wrap=log_d,--wrap=log_t

#
# test specific mocks
#
tst/tst-args_cli: LDFLAGS += -Wl,$\
	--wrap=usage

tst/tst-cmd: LDFLAGS += -Wl,$\
	--wrap=args_cmd

tst/tst-output_apply_cmd: LDFLAGS += -Wl,$\
	--wrap=tag_all
