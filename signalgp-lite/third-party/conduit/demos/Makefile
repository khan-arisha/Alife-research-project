SHELL := /bin/bash

TARGET_NAMES += consensus
TARGET_NAMES += diffusion
TARGET_NAMES += signalgp
TARGET_NAMES += channel_selection

default: test

test-%: %
	$(MAKE) test -C $<

opt-%: %
	$(MAKE) opt -C $<

fulldebug-%: %
	$(MAKE) fulldebug -C $<

cranky-%: %
	$(MAKE) cranky -C $<

clean-%: %
	$(MAKE) clean -C $<

cov-%: %
	$(MAKE) cov -C $<

cov: coverage_conversion $(addprefix cov-, $(TARGET_NAMES))

coverage_conversion:
	./convert_for_tests.sh

# Test in debug mode without pointer tracker
test: $(addprefix test-, $(TARGET_NAMES))

opt: $(addprefix opt-, $(TARGET_NAMES))

fulldebug: $(addprefix fulldebug-, $(TARGET_NAMES))

cranky: $(addprefix cranky-, $(TARGET_NAMES))

clean: $(addprefix clean-, $(TARGET_NAMES))
	rm -f *.out
	rm -f *.o
	rm -f *.gcda
	rm -f *.gcno
	rm -f *.info
	rm -f *.gcov
	rm -f ./Coverage*
	rm -rf ./temp
