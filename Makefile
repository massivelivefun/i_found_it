CC = gcc
CFLAGS = -Wall -Wextra -g -O3 -I$(INCLUDE)
LDFLAGS = -lm

INCLUDE = include
SRC_DIR = src
BUILD = build
TARGET = ifi

TEST_DIR = test
TEST_BIN = $(BUILD)/run_tests

SRC = $(shell find src -name '*.c')
OBJ = $(patsubst $(SRC_DIR)/%.c, $(BUILD)/%.o, $(SRC))
DEPS = $(shell find $(INCLUDE) -name '*.h')

OBJ_NO_MAIN = $(filter-out $(BUILD)/main.o, $(OBJ))

.PHONY: all clean debug test

all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p $(BUILD)
	@echo "Linking Main Target: $@"
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LDFLAGS)

$(BUILD)/%.o: $(SRC_DIR)/%.c $(DEPS)
	@mkdir -p $(dir $@)
	@echo "Compiling: $<"
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TEST_BIN)
	@echo "Running Tests..."
	./$(TEST_BIN)

$(TEST_BIN): $(OBJ_NO_MAIN) $(TEST_DIR)/test_path.c
	@mkdir -p $(BUILD)
	@echo "Building Test Runner: $@"
	$(CC) $(CFLAGS) -o $@ $(TEST_DIR)/test_path.c $(OBJ_NO_MAIN) $(LDFLAGS)

clean:
	@echo "Cleaning build directory..."
	rm -rf $(BUILD)

leaks: $(TARGET)
	@echo "Running macOS Leaks check..."
	# --atExit checks for leaks when the program finishes
	leaks --atExit -- ./$(TARGET) $(ARGS)

debug:
	@echo "C Source files: $(SRC)"
	@echo "Object files:   $(OBJ)"
	@echo "Header files found: $(DEPS)"                                                                                                                                        
	@echo "Targets to build:   $(TARGET)"
	@echo "Test Objects:   $(OBJ_NO_MAIN)"
