CC := gcc
CFLAGS := -Wall -Wextra -Iinclude -g

SRC_DIR := src
BIN_DIR := bin
EX_DIR := examples

SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(patsubst $(SRC_DIR)/%.c,$(BIN_DIR)/%.o,$(SRC))

LIB := $(BIN_DIR)/libmeow.a
EXAMPLE := $(BIN_DIR)/demo

all: $(LIB) example

# Build static library
$(LIB): $(OBJ)
	@mkdir -p $(BIN_DIR)
	ar rcs $@ $(OBJ)
	@echo "Library built: $@"

# Compile object files
$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Build example and link library
example: $(LIB)
	$(CC) $(CFLAGS) $(EX_DIR)/demo.c -L$(BIN_DIR) -lmeow -o $(EXAMPLE)
	@echo "Example built: $(EXAMPLE)"

clean:
	rm -rf $(BIN_DIR)

run: example
	./$(EXAMPLE)
