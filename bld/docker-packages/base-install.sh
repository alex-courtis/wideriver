#
# Install packages within the docker container.
# Not for running locally.
#

#
# arch 64 and 32 bit packages
#
pacman --noconfirm -Syu \
	clang \
	cmake \
	cmocka \
	cppcheck \
	git \
	llvm \
	valgrind \
	wayland \
	wayland-protocols
