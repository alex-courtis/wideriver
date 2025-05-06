#!/bin/sh

pacman --noconfirm -Syu \
	clang \
	cmake \
	cmocka \
	cppcheck \
	git \
	llvm \
	llvm-libs \
	ninja \
	valgrind \
	wayland \
	wayland-protocols
