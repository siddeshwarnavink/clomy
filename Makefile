all: $(patsubst %.c, %.out, $(wildcard examples/*.c))

examples/%.out: examples/%.c
	$(CC) -ggdb -Wall -Wextra -o $@ $<

clean:
	rm -f examples/*.out
