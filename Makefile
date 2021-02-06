CC       := gcc
CC_FLAGS := -ggdb

BIN     := bin
SRC     := src
INCLUDE := include

EXECUTABLE  := main


all: $(BIN)/$(EXECUTABLE)

run: clean all
	echo
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.c
	$(CC) $(CC_FLAGS) -I$(INCLUDE) $^ -o $@

clean:
	-rm -f $(BIN)/*
