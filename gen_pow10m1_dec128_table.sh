#!/bin/bash

echo '#include "intern/dec128.h"'
echo
echo "const intern_dec128_t pow10m1_dec128[40][9] = {"
echo "r=dump_pow10m1_dec128_table(40)"|BC_LINE_LENGTH=0 bc -l lanczos.bc
echo "};"
