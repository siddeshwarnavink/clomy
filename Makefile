SUBDIRS = tests examples

all: clomy.h $(SUBDIRS)

clomy.h: clomy.h.tmpl
	python gen.py

$(SUBDIRS): clomy.h
	$(MAKE) -C $@

clean:
	$(MAKE) -C tests/ clean

test:
	$(MAKE) -C tests/ test

.PHONY: all clean $(SUBDIRS)
