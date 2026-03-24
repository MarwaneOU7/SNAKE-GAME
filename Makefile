CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -fsanitize=address
LDFLAGS = -lraylib -lm

# Directories
SRC_DIR = src
OBJ_DIR = build
BIN = build/snake

# Source files
SOURCES = $(SRC_DIR)/main.c $(SRC_DIR)/game.c
OBJECTS = $(OBJ_DIR)/main.o $(OBJ_DIR)/game.o

# Default target
all: $(BIN)

# Link executable
$(BIN): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Run the game
run: $(BIN)
	./$(BIN)

# Clean build artifacts
clean:
	rm -f $(OBJ_DIR)/*.o $(BIN)

# Phony targets
.PHONY: all run clean
