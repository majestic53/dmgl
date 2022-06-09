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

BINARY_FILE=test_$(FILE)
OBJECT_FILES=$(patsubst %.c,%.o,$(shell find ./ -maxdepth 1 -name '*.c'))

COVERAGE_FLAGS=-fprofile-arcs -ftest-coverage
INCLUDE_FLAGS=$(subst $(INCLUDE_DIRECTORY),-I$(INCLUDE_DIRECTORY),$(shell find $(INCLUDE_DIRECTORY) -maxdepth 2 -type d))
TEST_INCLUDE_FLAGS=$(subst $(TEST_INCLUDE_DIRECTORY),-I$(TEST_INCLUDE_DIRECTORY),$(shell find $(TEST_INCLUDE_DIRECTORY) -maxdepth 2 -type d))

.PHONY: all
all: build

.PHONY: build
build: $(BINARY_FILE)

.PHONY: clean
clean:
	@rm -f $(BINARY_FILE)
	@rm -f $(OBJECT_FILES)
	@rm -f $(FILE).c.gcov
	@rm -f $(SOURCE_DIRECTORY)$(FILE).gcda
	@rm -f $(SOURCE_DIRECTORY)$(FILE).gcno
	@rm -f $(SOURCE_DIRECTORY)$(FILE).o

.PHONY: coverage
coverage:
	@gcov $(SOURCE_DIRECTORY)$(FILE).c

.PHONY: run
run:
	@if ! ./$(BINARY_FILE); then \
		exit 1; \
	fi

$(SOURCE_DIRECTORY)$(FILE).o: $(SOURCE_DIRECTORY)$(FILE).c
	$(CC) $(FLAGS) $(COVERAGE_FLAGS) $(INCLUDE_FLAGS) -c -o $@ $<

$(BINARY_FILE): $(SOURCE_DIRECTORY)$(FILE).o $(OBJECT_FILES)
	$(CC) $(FLAGS) $(COVERAGE_FLAGS) $(SOURCE_DIRECTORY)$(FILE).o $(OBJECT_FILES) -o $@

%.o: %.c
	$(CC) $(FLAGS) $(INCLUDE_FLAGS) $(TEST_INCLUDE_FLAGS) -c -o $@ $<
