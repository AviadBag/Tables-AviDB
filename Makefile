CC       := gcc
CC_FLAGS := -ggdb

BIN     := bin
SRC     := src
INCLUDE := include

SOURCES := $(shell find $(SRCDIR) -name "*.c")

EXECUTABLE  := main


all: $(BIN)/$(EXECUTABLE)

run: clean all
	echo
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SOURCES)
	$(CC) $(CC_FLAGS) -I$(INCLUDE) $^ -o $@

clean:
	-rm -f $(BIN)/*
