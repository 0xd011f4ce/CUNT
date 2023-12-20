CC=gcc
ECHO=echo -e

CFLAGS=-Wall -Werror -std=gnu99 -O0 -g -Isrc
LIBS=

FILES=build/test.o

TESTS=build/test_basic.o

all: $(FILES) $(TESTS)

build/test.o: src/test.c
	@$(ECHO) "CC\t\t"$<
	@$(CC) $(CFLAGS) -c $< -o $@ $(LIBS)

###############################################################################
#                                    Tests                                    #
###############################################################################
build/test_basic.o: tests/basic.c
	@$(ECHO) "Building test "$<
	@$(CC) $(CFLAGS) $(FILES) $< -o $@ $(LIBS)

###############################################################################
#                                 Useful calls                                #
###############################################################################
run_basic: build/test_basic.o
	@$<

valgrind: build/test_basic.o
	@valgrind $<

clean:
	@rm -f $(FILES) $(TESTS)
