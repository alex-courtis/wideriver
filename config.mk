VERSION ?= "1.2.0"

RIVER_LAYOUT_V3_VERSION = 2

PREFIX ?= /usr/local

INCS = -Iinc -Ipro -Ilib/col/inc

CPPFLAGS += $(INCS) -D_GNU_SOURCE -DVERSION=\"$(VERSION)\" -DRIVER_LAYOUT_V3_VERSION=$(RIVER_LAYOUT_V3_VERSION)

OFLAGS = -O3
WFLAGS = -Wall -Wextra -Werror -Wno-unused-parameter
DFLAGS = -g
COMPFLAGS = $(WFLAGS) $(OFLAGS) $(DFLAGS)

CFLAGS += $(COMPFLAGS) -std=gnu17 -Wold-style-definition -Wstrict-prototypes

LDFLAGS +=

ifeq (,$(filter-out DragonFly FreeBSD NetBSD OpenBSD,$(shell uname -s)))
PKGS += epoll-shim
endif

PKGS += wayland-client
PKG_CONFIG ?= pkg-config
CFLAGS += $(foreach p,$(PKGS),$(shell $(PKG_CONFIG) --cflags $(p)))
LDLIBS += $(foreach p,$(PKGS),$(shell $(PKG_CONFIG) --libs $(p)))

CC = gcc
