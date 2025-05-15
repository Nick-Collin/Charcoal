# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

#-Wall: Warning messages
#Wextra: Extra warning messages
#Iinclude: Adds the include directory to the compiler's include path, so your #include "lexer.h" will find headers in include/.

# Source and build directories
SRC_DIR = src
BUILD = build

#collects all src files in SRC_DIR
SRC = $(wildcard $(SRC_DIR)/*.c)


# Convert source file paths to object file paths in build dir
OBJ = $(patsubst $(SRC_DIR)/%.c, $(BUILD)/%.o, $(SRC))

# Output binary
TARGET = Charcoal
# Default rule
all: $(TARGET)

# Link all object files into the executable
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@

# Compile .c files to .o files in build directory
$(BUILD)/%.o: $(SRC_DIR)/%.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

# Create build directory if it doesn't exist
$(BUILD):
	mkdir -p $(BUILD)

clean:
	rm -rf $(BUILD) $(TARGET)

run: $(TARGET)
	./$(TARGET) test.chr