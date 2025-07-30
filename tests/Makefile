TESTS = $(patsubst %.c, %.out, $(wildcard *.c))

all: $(TESTS)

%.out: %.c
	$(CC) -ggdb -Wall -Wextra -o $@ $(realpath $<)

clean:
	rm -f *.out

test:
	for test in $(TESTS); do \
		./$$test; \
	done
