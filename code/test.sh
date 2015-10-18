#!/bin/bash

mode=

if [ $# -eq 0 ]; then
	echo "Usage: ./test dir_name* (ou ./test all)"
	exit
else
	for arg in $*; do
		if [ $arg == "-b" ]; then
			mode="-b"
		elif [ $arg == "all" ]; then
			TST_SRC=`ls ./tst/*/*.emu`
			break
		else
			TST_SRC+=`ls ./tst/$arg/*.emu`
		fi
	done
fi

../prof/testing/simpleUnitTest.sh $mode -e ./emul-ARM $TST_SRC