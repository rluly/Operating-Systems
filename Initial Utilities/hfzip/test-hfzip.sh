#! /bin/bash

if ! [[ -x hfzip.out ]]; then
    echo "hfzip.out executable does not exist"
    exit 1
fi

../tester/run-tests.sh $*


