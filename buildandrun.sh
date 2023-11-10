#!/bin/bash 

cmake --build cmake-build-debug
cmake-build-debug/fluid/fluid 1 small.fld out/test.fld
