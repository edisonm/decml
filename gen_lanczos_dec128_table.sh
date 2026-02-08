#!/bin/bash

echo '#include "intern/dec128.h"'
echo
echo "r=dump_lanczos_dec128_coef(18,25)"|BC_LINE_LENGTH=0 bc -l lanczos.bc


