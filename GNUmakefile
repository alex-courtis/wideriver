include config.mk

#
# dependencies
#
INC_H = $(wildcard inc/*.h) $(wildcard lib/alex-c/inc/*.h)

SRC_C = $(wildcard src/*.c)
SRC_O = $(SRC_C:.c=.o)

LIB_C = $(wildcard lib/*/src/*.c)
LIB_O = $(LIB_C:.c=.o)

PRO_X = $(wildcard pro/*.xml)
PRO_H = $(PRO_X:.xml=.h)
PRO_C = $(PRO_X:.xml=.c)
PRO_O = $(PRO_X:.xml=.o)

TST_H = $(wildcard tst/*.h) $(wildcard lib/alex-c/tst/*.h)
TST_C = $(wildcard tst/*.c) $(wildcard lib/alex-c/tst/*.c)
TST_O = $(TST_C:.c=.o)
TST_E = $(filter tst/tst%,$(TST_O:.o=))

all: wideriver

clean:
	rm -f way-displays $(SRC_O) $(PRO_O) $(LIB_O) $(PRO_H) $(PRO_C) $(TST_O) $(TST_E) actual.* expected.*
	find . -name '*.gcno' -type f -delete -print
	find . -name '*.gcda' -type f -delete -print
	rm -rf tst/tmp

$(SRC_O): CFLAGS += $(COVCFLAGS)
$(SRC_O): $(INC_H) $(PRO_H) config.mk GNUmakefile
$(LIB_O): config.mk GNUmakefile
$(PRO_O): $(PRO_H) config.mk GNUmakefile

#
# executable
#
wideriver: $(SRC_O) $(PRO_O) $(LIB_O)
	$(CC) -o $(@) $(^) $(LDFLAGS) $(LDLIBS)
	@test -x ../deploy.sh && ../deploy.sh || true
	@test -x ../../deploy.sh && ../../deploy.sh || true

#
# protocols
#
$(PRO_H): $(PRO_X)
	wayland-scanner client-header $(@:.h=.xml) $@

$(PRO_C): $(PRO_X)
	wayland-scanner private-code $(@:.c=.xml) $@

#
# deploy
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
iwyu: override CC = include-what-you-use \
	-Xiwyu --no_fwd_decls \
	-Xiwyu --error=1 \
	-Xiwyu --verbose=3 \
	-Xiwyu --mapping_file=.iwyu.imp \
	-Xiwyu --check_also="inc/*h" \
	-Xiwyu --check_also="lib/alex-c/inc/*h" \
	-Xiwyu --check_also="tst/*h" \
	-Xiwyu --check_also="lib/alex-c/tst/*h"
iwyu: INCS += -Ilib/alex-c/tst
iwyu: clean $(SRC_O) $(LIB_O) $(TST_O)

#
# cppcheck
#
cppcheck: INCS += -Ilib/alex-c/tst
cppcheck: $(SRC_C) $(LIB_C) $(INC_H) $(PRO_H) $(TST_H) $(TST_C)
	cppcheck $(^)\
		--enable=all \
		--disable=information \
		--inconclusive \
		--check-level=exhaustive \
		--inline-suppr \
		--suppressions-list=.cppcheck.supp \
		--error-exitcode=1 \
		$(CPPFLAGS)

#
# tests only included when executing test targets
#
ifneq (,$(or $(findstring test,$(MAKECMDGOALS)), $(findstring tst/tst,$(MAKECMDGOALS))))

include tst/test.mk

endif

#
# local docker dev
#
docker-build:
	docker build --tag "wide-river:latest" .

docker-stop:
	docker rm -f wide-river || true

docker-run: docker-stop
	docker run \
		--name="wide-river" \
		--volume "${PWD}:/wide-river" \
		--workdir="/wide-river" \
		--user "`id -u`:`id -g`" \
		--detach \
		"wide-river:latest"

docker-packages:
	docker exec                    wide-river .github/workflows/packages/include-what-you-use/build.sh
	docker exec --user "root:root" wide-river .github/workflows/packages/include-what-you-use/install.sh

#
# targets
#
.PHONY: all clean install uninstall doc iwyu cppcheck test test-vg $(TST_T)

.NOTPARALLEL: iwyu test test-vg
