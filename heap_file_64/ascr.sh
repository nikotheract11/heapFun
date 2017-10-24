#!/bin/bash

#rm data*

make $1
./build/runner
rm data*
