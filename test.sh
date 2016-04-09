#!/usr/bin/env bash

./test_output states/SBP-level0.txt

if [ "$?" != 0 ]; then
    echo "Test failed"
    exit 1
else
    echo "Success!"
fi


