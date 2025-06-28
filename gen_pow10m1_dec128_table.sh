#!/bin/bash

echo '#include "intern/dec128.h"'
echo
echo "const intern_dec128_t pow10m1_dec128[40][9] = {"
for ((pos=1;pos<=40;pos++)); do
    echo "    {"
    for ((i=1;i<=9;i++)); do
        value=`echo -e "scale=50+$pos; \
                     x=$pos; \
                     y=10^x*(e($i*l(10)/10^x)-1); \
                     if (y > 34.0282366920938463463374607431768211455) \
                         exp=36 \
                     else if (y > 3.40282366920938463463374607431768211455) \
                         exp=37 \
                     else \
                         exp=38; \
                     z=10^exp*y; \
                     scale=0; \
                     obase=16; \
                     print \"{0, \"; \
                     k=(z+0.5)/1; \
                     print \"((__uint128_t)\"; \
                     print \"0x\"; \
                     print k/2^64; \
                     print \"ULL<<64)+\n\"; \
                     print \"0x\"; \
                     print k%2^64; \
                     obase=10; \
                     print \"ULL,\n-\"; \
                     print exp+x; \
                     print \", DEC_NORMAL},\""|bc -l`
        echo "        "${value}
    done
    echo "    },"
done
echo "};"
