all: $(patsubst %.c, %.out, $(wildcard examples/*.c))

examples/%.out: examples/%.c
	$(CC) -Wall -Wextra -o $@ $<

clean:
	rm -f examples/*.out
