#!/bin/sh

docker run --rm -ti \
	--volume "${PWD}:/wideriver" \
	--workdir="/wideriver" \
	--user "`id -u`:`id -g`" \
	wideriver:latest \
	$@
