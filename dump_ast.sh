#!/bin/bash

set -x

../bin/clang++ -include pre_def.h -Xclang -ast-dump add_custom.cpp
