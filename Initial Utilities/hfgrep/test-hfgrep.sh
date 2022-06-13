#! /bin/bash

if ! [[ -x hfgrep.out ]]; then
    echo "hfgrep.out executable does not exist"
    exit 1
fi

../tester/run-tests.sh $*



