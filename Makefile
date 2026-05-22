CC := cc
CFLAGS := -Wall -Wextra -Iinclude -g -MMD -MP
SRC_DIR := src
BIN_DIR := bin
EX_DIR := examples
SRC := $(shell find $(SRC_DIR) -name '*.c')
OBJ := $(patsubst $(SRC_DIR)/%.c,$(BIN_DIR)/%.o,$(SRC))
LIB := $(BIN_DIR)/libmeow.a
EXAMPLE := $(BIN_DIR)/demo

.PHONY: all example clean run print

all: $(LIB) example

# Build static library
$(LIB): $(OBJ)
	@mkdir -p $(BIN_DIR)
	ar rcs $@ $(OBJ)
	@echo "Library built: $@"

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

example: $(EXAMPLE)

$(EXAMPLE): $(LIB)
	$(CC) $(CFLAGS) $(EX_DIR)/demo.c -L$(BIN_DIR) -lmeow -o $(EXAMPLE)
	@echo "Example built: $(EXAMPLE)"

clean:
	rm -rf $(BIN_DIR)

run: $(EXAMPLE)
	./$(EXAMPLE)

print:
	@echo "SRC: $(SRC)"
	@echo "OBJ: $(OBJ)"

-include $(OBJ:.o=.d)
