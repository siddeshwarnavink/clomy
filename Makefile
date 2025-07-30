SUBDIRS = tests

all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

clean:
	$(MAKE) -C tests/ clean

test:
	$(MAKE) -C tests/ test

.PHONY: all clean $(SUBDIRS)
