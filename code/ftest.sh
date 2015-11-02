#!/bin/bash

for arg in $*; do
	TST_SRC+=$arg
done

../prof/testing/simpleUnitTest.sh -e ./emul-arm $TST_SRC
