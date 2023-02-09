CC := gcc
CFLAGS=-I -Wall -lcurses -v

BUILD_DIR ?= build

HEADERS := $(wildcard include/*.h) 
SRC := $(wildcard src/*.c)

TARGET := main.c
ARGS := hello.ml
DEPENDENIES := null 
LOG ?=  _.log

all: $(HEADERS) $(SRC)

%: # compile + run %.c
ifeq ("$(wildcard $(BUILD_DIR))","")  # mkdir build
	mkdir $(BUILD_DIR)
endif
	$(CC) src/$@.c $(CFLAGS) -o $(BUILD_DIR)/$@
	@echo "\n\033[92mExecuting...\033[0m"
	./$(BUILD_DIR)/$@ $(ARGS)
	@echo "\n\033[92mDone\033[0m"
	make clean

clean: 
ifneq ("$(wildcard $(BUILD_DIR))","") # rm  build dir
	rm -rf $(BUILD_DIR)
endif
ifneq ("$(wildcard $(wildcard *.log))","") # rm  build dir
	rm $(wildcard *.log) # rm logs
endif
