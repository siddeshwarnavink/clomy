all: $(patsubst %.c, %.out, $(wildcard *.c))

%.out: %.c
	$(CC) -ggdb -Wall -Wextra -o $@ $<

clean:
	rm -f *.out
