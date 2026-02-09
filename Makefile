CC = gcc
CFLAGS = -Wall -Wextra -g -O3 -I$(SRC_DIR)
LDFLAGS = -lm

SRC_DIR = src
BUILD = build
TARGET = $(BUILD)/ifi

TEST_DIR = test
TEST_BIN = $(BUILD)/run_tests

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c, $(BUILD)/%.o, $(SRC))
DEPS = $(wildcard $(SRC_DIR)/*.h)

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

debug:
	@echo "C Source files: $(SRC)"
	@echo "Object files:   $(OBJ)"
	@echo "Test Objects:   $(OBJ_NO_MAIN)"
