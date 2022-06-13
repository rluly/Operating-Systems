#! /bin/bash

if ! [[ -x hfunzip.out ]]; then
    echo "hfunzip.out executable does not exist"
    exit 1
fi

../tester/run-tests.sh $*


