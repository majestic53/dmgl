# DMGL
# Copyright (C) 2022 David Jolly
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
# associated documentation files (the "Software"), to deal in the Software without restriction,
# including without limitation the rights to use, copy, modify, merge, publish, distribute,
# sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all copies or
# substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
# PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
# COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
# AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
# WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

TEST_DIRECTORIES=$(shell find . -maxdepth 2 -type d -not -path '.' -not -path '*include*')

MAKE_FLAGS=--no-print-directory -C

.PHONY: all
all: build

.PHONY: build
build:
	@for test in $(TEST_DIRECTORIES); do \
		make $(MAKE_FLAGS) $$test build; \
	done

.PHONY: clean
clean:
	@for test in $(TEST_DIRECTORIES); do \
		make $(MAKE_FLAGS) $$test clean; \
	done

.PHONY: run
run:
	@result=0; \
	for test in $(TEST_DIRECTORIES); do \
		if ! make $(MAKE_FLAGS) $$test run; then \
			result=1; \
		else \
			make $(MAKE_FLAGS) $$test coverage; \
		fi \
	done; \
	if [ $$result -ne 0 ]; then \
		exit 1; \
	fi
