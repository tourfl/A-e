#!/bin/bash

if [ $# = 0 ]; then
	echo "Usage: ./test dir_name* (ou ./test all)"
	exit
elif [ $1 == "all" ]; then
	TST_SRC=`ls ./tst/*/*.emu`
else
	for arg in $*; do
		TST_SRC+=`ls ./tst/$arg/*.emu`
	done
fi

../prof/testing/simpleUnitTest.sh -e ./emul-ARM $TST_SRC