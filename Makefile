CC = g++
CFLAGS =-g -Wall -std=c++17 `pkg-config --cflags gtk+-3.0`  # -fsanitize=address
LDFLAGS = `pkg-config --libs gtk+-3.0` 


SRC_DIR = src
OBJ_DIR = obj
BIN = main

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

all: directories $(BIN)

$(BIN): $(OBJS)
	$(CC)   -o $@ $^ $(LDFLAGS)
	./$(BIN)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: directories

directories: ${OBJ_DIR}

${OBJ_DIR}:
	mkdir -p ${OBJ_DIR}

clean:
	rm -rf $(OBJ_DIR) $(BIN)

