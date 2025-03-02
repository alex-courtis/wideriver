include config.mk

#
# dependencies
#
SRC_H = $(wildcard inc/*.h)
SRC_C = $(wildcard src/*.c)
SRC_O = $(SRC_C:.c=.o)

PRO_X = $(wildcard pro/*.xml)
PRO_H = $(PRO_X:.xml=.h)
PRO_C = $(PRO_X:.xml=.c)
PRO_O = $(PRO_X:.xml=.o)

LIB_H = $(wildcard lib/col/inc/*.h)
LIB_C = $(wildcard lib/col/src/*.c)
LIB_O = $(LIB_C:.c=.o)

TST_H = $(wildcard tst/*.h)
TST_C = $(wildcard tst/*.c)
TST_O = $(TST_C:.c=.o)
TST_E = $(patsubst tst/%.c,%,$(wildcard tst/tst-*.c))
TST_T = $(patsubst tst%,test%,$(TST_E))

#
# build
#
all: wideriver

$(SRC_O): $(SRC_H) $(PRO_H)
$(PRO_O): $(PRO_H)
$(LIB_O): $(LIB_H)

wideriver: $(SRC_O) $(PRO_O) $(LIB_O)
	$(CC) -o $(@) $(^) $(LDFLAGS) $(LDLIBS)
	@test -x ../deploy.sh && ../deploy.sh || true

$(PRO_H): $(PRO_X)
	wayland-scanner client-header $(@:.h=.xml) $@

$(PRO_C): $(PRO_X)
	wayland-scanner private-code $(@:.c=.xml) $@

clean:
	rm -f wideriver $(SRC_O) $(PRO_O) $(PRO_H) $(PRO_C) $(LIB_O) $(TST_O) $(TST_E)

#
# install
#
install: wideriver man/wideriver.1
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f wideriver $(DESTDIR)$(PREFIX)/bin
	chmod 755 $(DESTDIR)$(PREFIX)/bin/wideriver
	mkdir -p $(DESTDIR)$(PREFIX)/share/man/man1
	cp -f man/wideriver.1 $(DESTDIR)$(PREFIX)/share/man/man1
	chmod 644 $(DESTDIR)$(PREFIX)/share/man/man1/wideriver.1

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/wideriver
	rm -f $(DESTDIR)$(PREFIX)/share/man/man1/wideriver.1

#
# doc
#
doc: wideriver
	$(MAKE) -f doc/GNUmakefile

#
# iwyu
#
iwyu: override CC = $(IWYU) -Xiwyu --check_also="inc/*h"
iwyu: clean $(SRC_O) $(TST_O)
IWYU = include-what-you-use -Xiwyu --no_fwd_decls -Xiwyu --error=1 -Xiwyu --verbose=3

#
# cppcheck
#
cppcheck: $(SRC_H) $(SRC_C) $(TST_H) $(TST_C)
	cppcheck $(^) --enable=warning,unusedFunction,performance,portability --suppressions-list=.cppcheck.supp --error-exitcode=1 $(CPPFLAGS)

#
# valgrind
#
%-vg: VALGRIND = valgrind --error-exitcode=1 --leak-check=full --show-leak-kinds=all --errors-for-leak-kinds=all --gen-suppressions=all
%-vg: % ;

#
# test
#
test: $(TST_T)
test-vg: $(TST_T)

$(TST_T): EXE = $(patsubst test%,tst%,$(@))
$(TST_T): $(SRC_O) $(PRO_O) $(LIB_O)
	$(MAKE) -f tst/GNUmakefile $(EXE)
	$(VALGRIND) ./$(EXE)

#
# targets
#
.PHONY: all clean install uninstall doc iwyu cppcheck test test-vg $(TST_T)

.NOTPARALLEL: iwyu test test-vg
