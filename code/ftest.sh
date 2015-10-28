#!/bin/bash

for arg in $*; do
	TST_SRC+=$arg
done

~/ARM-emulator/prof/testing/simpleUnitTest.sh -e ./emul-ARM $TST_SRC
