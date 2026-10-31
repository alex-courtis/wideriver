VERSION ?= "1.4.1-SNAPSHOT"
ifneq (,$(findstring -SNAPSHOT,$(VERSION)))
	COMMIT := $(shell git rev-parse --short HEAD 2>/dev/null)
endif

RIVER_LAYOUT_V3_VERSION = 2

PREFIX ?= /usr/local

# TODO alex-c-lib cleanup
INCS = -Iinc -Ipro -Ilib/alex-c/inc -Ilib/col/inc

# TODO add COMMIT
CPPFLAGS += $(INCS) -D_GNU_SOURCE -DVERSION=\"$(VERSION)\" -DCOMMIT=\"$(COMMIT)\" -DRIVER_LAYOUT_V3_VERSION=$(RIVER_LAYOUT_V3_VERSION)

OFLAGS = -O3
WFLAGS = -pedantic \
		 -Wall \
		 -Wextra \
		 -Werror \
		 -Wimplicit-fallthrough \
		 -Wold-style-definition \
		 -Wstrict-prototypes \
		 -Wno-unused-parameter
DFLAGS = -g
COMPFLAGS = $(WFLAGS) $(OFLAGS) $(DFLAGS)

CFLAGS += $(COMPFLAGS) -std=gnu17

LDFLAGS +=

ifeq (,$(filter-out DragonFly FreeBSD NetBSD OpenBSD,$(shell uname -s)))
PKGS += epoll-shim
endif

PKGS += wayland-client
PKG_CONFIG ?= pkg-config
CFLAGS += $(foreach p,$(PKGS),$(shell $(PKG_CONFIG) --cflags $(p)))
LDLIBS += $(foreach p,$(PKGS),$(shell $(PKG_CONFIG) --libs $(p)))

CC = gcc
