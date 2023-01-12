CC := gcc
CFLAGS=-I -Wall

BUILD_DIR ?= build

HEADERS := $(wildcard include/*.h) 
SRC := $(wildcard src/*.c)

TARGET := main.c
DEPENDENIES := null 
OUT := out

all: $(HEADERS) $(SRC)

mkbuild: # mkdir build
ifeq ("$(wildcard $(BUILD_DIR))","") 
	mkdir $(BUILD_DIR)
endif

%: $(mkbuild) # compile + run %.c
	$(CC) src/$@.c $(CFLAGS) -o $(BUILD_DIR)/$@
	./$(BUILD_DIR)/$@

clean: # rm  build dir
ifneq ("$(wildcard $(BUILD_DIR))","") 
	rm -rf $(BUILD_DIR)
endif

