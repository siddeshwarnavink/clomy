SUBDIRS = examples tests

all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

clean:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done

test:
	$(MAKE) -C tests/ run

.PHONY: all clean $(SUBDIRS)
