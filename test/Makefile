ASSIGNMENT=Graph

UNITY_FOLDER=Unity
TEST_INC_DIRS=$(INC_DIRS) -I$(UNITY_FOLDER)

TEST_FILES=$(UNITY_FOLDER)/unity.c \
           Graph_test.c \
           ../src/GraphPathFinding/GraphPathFinding.c \
           ../src/MakeGraph/MakeGraph.c \


HEADER_FILES=../src/GraphPathFinding/GraphPathFinding.h \
             ../src/MakeGraph/MakeGraph.h \

TEST = $(ASSIGNMENT)_test

CC=gcc

SYMBOLS=-Wall -Werror -pedantic -O2 -std=c99
TEST_SYMBOLS=$(SYMBOLS) -DTEST

.PHONY: clean test

all: $(TEST)

$(TEST): Makefile $(TEST_FILES)  $(HEADER_FILES)
	$(CC) $(TEST_INC_DIRS) $(TEST_SYMBOLS) $(TEST_FILES) -o $(TEST)

clean:
	rm -f $(TEST)

test: $(TEST)
	@./$(TEST)