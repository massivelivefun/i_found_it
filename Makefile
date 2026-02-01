CC = gcc
CFLAGS = -Wall -Wextra -g -O3 -I$(SRC_DIR)
LDFLAGS = -lm

SRC_DIR = src
BUILD = build
TARGET = ifi

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c, $(BUILD)/%.o, $(SRC))
DEPS = $(wildcard $(SRC_DIR)/*.h)

.PHONY: all clean debug

all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p $(BUILD)
	@echo "Linking C Target: $@"
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LDFLAGS)

$(BUILD)/%.o: $(SRC_DIR)/%.c $(DEPS)
	@mkdir -p $(dir $@)
	@echo "Compiling: $<"
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD)
	rm -f $(TARGET)

debug:
	@echo "C Source files found: $(SRC)"
	@echo "Object files found: $(OBJ)"
	@echo "Header files found: $(DEPS)"
	@echo "Targets to build:   $(TARGET)"
