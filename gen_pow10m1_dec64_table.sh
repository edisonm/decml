#!/bin/bash

echo '#include "intern/dec64.h"'
echo
echo "const intern_dec64_t pow10m1_dec64[20][9] = {"
echo "r=dump_pow10m1_dec64_table(20)"|BC_LINE_LENGTH=0 bc -l lanczos.bc
echo "};"
