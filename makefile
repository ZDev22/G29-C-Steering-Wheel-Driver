CC = gcc
CFLAGS = -O2 -std=c99
LDFLAGS = -lm -lpthread -lhidapi-libusb -levdev

BIN := bin/main

SRC := $(shell find . -name '*.c')

all: $(BIN)

$(BIN): $(SRC)
	@mkdir -p bin
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
