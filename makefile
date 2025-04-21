
SRC=src
OBJ=obj
TEST=test

BIN=bin

CC=gcc
CPP=g++

OUTPUT=$(BIN)/libhttpc.a

SRCS=$(shell find $(SRC) -name "*.c")
OBJS=$(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(SRCS))
TESTS=$(patsubst $(SRC)/%.c,$(TEST)/%_unittest.cpp,$(SRCS))
TEST_OBJS=$(patsubst $(TEST)/%.cpp,$(OBJ)/%.o,$(TESTS))

TEST_MAIN=$(TEST)/unittest_main.cpp
TEST_CPP_OPTS=-Wall -Wextra -Werror -I $(SRC)/include -std=c++17 -pthread 

dbg_print: 
	@echo sources:   $(SRCS)
	@echo objects:   $(OBJS)
	@echo test srcs: $(TESTS)
	@echo test objs: $(TEST_OBJS)

build: $(OBJS)
	ar rcs $(OUTPUT) $(OBJS)

test: build $(TEST_OBJS)
	$(CPP) $(TEST_CPP_OPTS) $(TEST_MAIN) $(TEST_OBJS) -Lbin -lhttpc -lgtest -lgtest_main -o bin/test
	bin/test

$(OBJS): $(OBJ)/%.o : $(SRC)/%.c
	mkdir -p $(dir $@)
	$(CC) -c -I $(SRC)/include -fPIC $^ -o $@

$(TEST_OBJS): $(OBJ)/%.o : $(TEST)/%.cpp
	mkdir -p $(dir $@)
	$(CPP) $(TEST_CPP_OPTS) -c $^ -o $@

clean: 
	rm $(OBJS)
	rm $(TEST_OBJS)