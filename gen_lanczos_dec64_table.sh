#!/bin/bash

echo '#include "intern/dec64.h"'
echo
echo "r=dump_lanczos_dec64_coef(8,12)"|BC_LINE_LENGTH=0 bc -l lanczos.bc
