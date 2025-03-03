include GNUmakefile

PKGS_TST = cmocka
CFLAGS += $(foreach p,$(PKGS_TST),$(shell pkg-config --cflags $(p)))
LDLIBS += $(foreach p,$(PKGS_TST),$(shell pkg-config --libs $(p)))

OBJS =  tst/util.o \
		$(patsubst %.c,%.o,$(wildcard tst/wrap-*.c)) \
		$(filter-out src/main.o,$(SRC_O)) \
		$(PRO_O) $(LIB_O)

WRAPS_COMMON = -Wl,$\
	--wrap=log_f,--wrap=log_e,--wrap=log_w,--wrap=log_i,--wrap=log_d

tst-args_cli: WRAPS=,$\
	--wrap=usage

tst-cmd: WRAPS=,$\
	--wrap=args_cmd

tst-output_apply_cmd: WRAPS=,$\
	--wrap=tag_all

$(TST_O): $(TST_H)

$(TST_E): $(TST_O) $(OBJS)
	$(CC) -o $(@) tst/$(@).o $(OBJS) $(LDFLAGS) $(LDLIBS) $(WRAPS_COMMON)$(WRAPS)

