#CC=gcc
#CFLAGS=-I.

#SYMBOLS=-std=c99
#TEST_SYMBOLS=$(SYMBOLS) -DTEST

#SRC_DIR=./src
#TEST_DIR=./test
#UNITY_DIR=./Unity
#INC_DIRS		= -I$(SRC_DIR) -I$(UNITY_DIR) 

#DEP_FILES=$(SRC_DIR)/*.h
#OBJ_FILES=$(SRC_DIR)/*.c
#MAIN_FILES=Graph_main.c
#$(filter-out $(MAIN_FILES),$(MAIN_FILES))


#TEST_DEP_FILES=$(UNITY_DIR)/*.h
#TEST_FILES=$(TEST_DIR)/*.c\
           $(OBJ_FILES) \
           $(UNITY_DIR)/unity.c

#%.o: %.c $(DEP_FILES) $(TEST_DEP_FILES)
#	$(CC) -c -o $@ $< $(CFLAGS)

#TARGETS=Graph Graph_test

#.PHONY: all clean

#all: $(TARGETS)

#Graph: $(SRC_DIR)/Graph_main.o $(OBJ_FILES)
#	$(CC) $(SYMBOLS) -o $@ $^ $(CFLAGS)

#Graph_test: $(OBJ_FILES)$(TEST_FILES)
#	$(CC) $(TEST_SYMBOLS) -o $@ $^ $(CFLAGS)

#clean:
#	rm -f $(TARGETS) $(SRC_DIR)/*.o $(TEST_DIR)/*.o *.tmp

LIBS=src/File_io src/GraphPathFinding src/GraphSearch src/MakeGraph src/Terminal_io
INCLUDE_PATH=src/
SYMBOLS=-Wall -Werror -pedantic -O2 -std=c99

boardcomputer_exec: libraries
	gcc $(SYMBOLS) src/Graph_main.c $(wildcard libraries/*) -I$(INCLUDE_PATH) -o Graph

libraries:
	mkdir libraries/
	for dir in $(LIBS); do \
		cd $$dir; \
		gcc -c *.c -I../; \
		mv *.o ../../libraries; \
		cd -; \
	done

clean:
	rm -rf libraries/

remove:
	rm Graph