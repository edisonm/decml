MAKEFLAGS += --silent --no-print-directory
CC = gcc
CFLAGS = -I./include -Wall -Wextra -O2 -fPIC
LIBS = bin/libdec.a

AUTOGEN=gen_pow10m1_dec64_table.c \
	gen_lanczos_dec64_table.c \
	gen_pow10m1_dec128_table.c \
	gen_lanczos_dec128_table.c

SRCS =  $(AUTOGEN) intern_dec.c dec64_dec128.c \
	dec64.c  intern_dec64_str.c intern_dec64.c \
	dec128.c intern_dec128_str.c intern_dec128.c

.PRECIOUS: $(addprefix bin/,$(AUTOGEN))

OBJS = $(SRCS:.c=.o)

TESTS= \
	test_dec_func_1 \
	test_dec_func_2 \
	test_dec_compare \
	test_dec64_str \
	test_dec128_str

bin:
	mkdir -p bin

all: bin bin/libdec.a

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

bin/%: tests/%.o bin/test_dec_common.o $(LIBS)
	$(CC) $(CFLAGS) -o $@ $^

tests:
	make $(TESTS)

$(TESTS):
	$(MAKE) bin/$@
	bin/$@

.PHONY: tests clean all

clean:
	rm -f $(addprefix,bin/,$(TESTS)) bin/*.o bin/*.c bin/*.a bin/test*
