#!/bin/bash

echo '#include "intern/dec64.h"'
echo
echo "const intern_dec64_t pow10m1_dec64[20][9] = {"
for ((pos=1;pos<=20;pos++)); do
    echo "    {"
    for ((i=1;i<=9;i++)); do
        value=`echo "scale=30+$pos; \
                     x=$pos; \
                     y=10^x*(e($i*l(10)/10^x)-1); \
                     if (y > 18.446744073709551615) \
                         exp=16 \
                     else if (y > 1.8446744073709551615) \
                         exp=17 \
                     else \
                         exp=18; \
                     z=10^exp*y; \
                     scale=0; \
                     print \"{0, \"; \
                     print (z+0.5)/1; \
                     print \"ULL, -\"; \
                     print exp+x; \
                     print \", DEC_NORMAL},\""|bc -l`
        echo "        "${value}
    done
    echo "    },"
done
echo "};"
