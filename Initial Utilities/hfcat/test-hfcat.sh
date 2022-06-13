#! /bin/bash

if ! [[ -x hfcat.out ]]; then
    echo "hfcat.out executable does not exist"
    exit 1
fi

../tester/run-tests.sh $*


