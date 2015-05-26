TEST_DIR = tests
TEST_OBJS = ./tests/level_test.h
# Get all sources except main.cpp
TEST_SRC_OBJS = $(patsubst src/main.cpp,,$(wildcard src/*.cpp))

TEST_CFLAGS = -DTEST_MODE -I./include/cxxtest-4.3 -I./src -I./src

TESTGEN = ./include/cxxtest-4.3/bin/cxxtestgen

TEST_RUNNER = ./$(TEST_DIR)/runner
TEST_RUNNER_SRC = $(TEST_RUNNER).cpp


test: test_compile
	$(TEST_RUNNER)

test_compile: test_parse
	$(CC) $(TEST_CFLAGS) $(TEST_SRC_OBJS) $(TEST_RUNNER_SRC) \
	-o $(TEST_RUNNER) $(LDLIBS)

test_parse: test_clean
	$(TESTGEN) --runner=ParenPrinter \
	-o $(TEST_RUNNER_SRC) $(TEST_OBJS)

test_clean:
	rm -f $(TEST_RUNNER) $(TEST_RUNNER_SRC)
