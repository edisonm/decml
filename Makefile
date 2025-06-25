MAKEFLAGS += --silent --no-print-directory
CC = gcc
CFLAGS = -I./include -Wall -Wextra -O2
LIBS = bin/libdec.a

SRCS =  intern_dec.c gen_pow10m1_dec64_table.c \
	dec64.c  intern_dec64_str.c  intern_dec64_arithmetic.c  intern_dec64.c \
	gen_pow10m1_dec128_table.c \
	dec128.c intern_dec128_str.c intern_dec128_arithmetic.c intern_dec128.c

OBJS = $(SRCS:.c=.o)

TESTS= \
	test_dec64_str \
	test_dec64_arithmetic \
	test_dec64_sqrt \
	test_dec64_log10 \
	test_dec64_atan \
	test_dec64_compare \
	test_dec128_str \
	test_dec128_arithmetic \
	test_dec128_sqrt \
	test_dec128_log10 \
	test_dec128_atan \
	test_dec128_compare

all: bin/libdec.a

bin/libdec.a: $(addprefix bin/,$(OBJS))
	ar rcs $@ $^

bin/%.c: %.sh
	./$< > $@

bin/gen_%.o: bin/gen_%.c include/intern/dec64.h
	$(CC) $(CFLAGS) -c $< -o $@

bin/%.o: src/%.c include/dec64.h include/intern/dec64.h
	$(CC) $(CFLAGS) -c $< -o $@

bin/intern_%.o:src/intern/*.c include/intern/dec64.h
	$(CC) $(CFLAGS) -c $< -o $@

bin/%.o: tests/%.c
	$(CC) $(CFLAGS) -c $< -o $@

bin/%: tests/%.o bin/test_dec_common.o bin/test_dec64_common.o bin/test_dec128_common.o $(LIBS)
	$(CC) $(CFLAGS) -o $@ $^

tests:
	make $(TESTS)

$(TESTS):
	$(MAKE) bin/$@
	bin/$@

.PHONY: tests clean all

clean:
	rm -f $(addprefix,bin/,$(TESTS)) bin/*.o bin/*.a bin/test*
