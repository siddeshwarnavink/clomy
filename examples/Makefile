EXAMPLES = $(patsubst %.c, %.out, $(wildcard *.c))

all: $(EXAMPLES)

%.out: %.c
	$(CC) -ggdb -Wall -Wextra -o $@ $(realpath $<)

clean:
	rm -f *.out
