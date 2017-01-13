ASSIGNMENT=Graph

SRC_DIR=src
TEST_DIR=test
UNITY_FOLDER=Unity
TEST_INC_DIRS=-I$(UNITY_FOLDER)

SRC_FILES=$(SRC_DIR)/$(ASSIGNMENT)_main.o \
          $(SRC_DIR)/File_io.o \
          $(SRC_DIR)/GraphPathFinding.o \
          $(SRC_DIR)/GraphSearch.o \
          $(SRC_DIR)/MakeGraph.o \
          $(SRC_DIR)/Terminal_io.o 
		  

TEST_FILES=$(UNITY_FOLDER)/unity.c \
           $(TEST_DIR)/$(ASSIGNMENT)_test.c

HEADER_FILES=$(SRC_DIR)/*.h

TEST = $(ASSIGNMENT)_test

CC=gcc

SYMBOLS=-Wall -Werror -pedantic -O2 -std=c99
TEST_SYMBOLS=$(SYMBOLS) -DTEST

.PHONY: all clean test

all: $(ASSIGNMENT) $(TEST)

$(ASSIGNMENT): $(SRC_FILES) $(HEADER_FILES) Makefile
	$(CC) $(INC_DIRS) $(SYMBOLS) $(SRC_FILES) -o $(ASSIGNMENT)

$(TEST): Makefile $(TEST_FILES)  $(HEADER_FILES)
	$(CC) $(TEST_INC_DIRS) $(TEST_SYMBOLS) $(TEST_FILES) -o $(TEST)

clean:
	rm -f $(ASSIGNMENT) $(TEST)

test: $(TEST)
	@./$(TEST)