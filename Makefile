CC := clang

# compiler flags
DEBUG     := -gdwarf-4
OPTIMISE  := -O0
WARNING   := -Wall -Wextra -Wno-variadic-macros \
             -Wno-overlength-strings -pedantic
EXTRA     := -std=c11 -fPIC
CFLAGS    := $(DEBUG) $(OPTIMISE) $(WARNING) $(EXTRA) -I./deps
DFLAGS    ?= # -DDEBUG

# linker flags

LDFLAGS := -L./deps/c-lib-stack -lstack

# commands

COMPILE:=$(CC) $(CFLAGS) $(DFLAGS)

# targets

libstringutils.a: build/src/str_to_argv.o
	ar rcs $@ $^
	ar t libstringutils.a

tests: test_string_utils

# target files

build/src/%.o: src/%.c src/%.h | build
	$(COMPILE) -c -o $@ $< $(LDFLAGS)

build/tests/%.o: tests/%.c libstringutils.a | build
	$(COMPILE) -c -o $@ $<

build/%.o: %.c | build
	$(COMPILE) -c -o $@ $<

test_string_utils: build/tests/test_string_utils.o libstringutils.a
	$(COMPILE) -o $@ $< -L. -lstringutils $(LDFLAGS)

# target directories

build:
	mkdir -p build/src
	mkdir -p build/tests

install: libstringutils.a src/stack.h
	cp libstringutils.a /usr/local/lib
	cp src/stack.h /usr/local/include

uninstall:
	rm -f /usr/local/lib/libstringutils.a
	rm -f /usr/local/include/stack.h

clean:
	rm -rf build
	rm -f libstringutils.a
	rm -f test_string_utils

.PHONY: clean tests install uninstall

