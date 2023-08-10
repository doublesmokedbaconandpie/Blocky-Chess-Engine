# Written by ChatGpt-3.5
CXX := g++
CXXFLAGS := -std=c++11 -Isrc

SRC_DIR := src
TEST_DIR := tests

SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp, $(SRC_DIR)/%.o, $(SRC_FILES))

TEST_SRC_FILES := $(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJ_FILES := $(patsubst $(TEST_DIR)/%.cpp, $(TEST_DIR)/%.o, $(TEST_SRC_FILES))

BIN := test_board

all: $(BIN)

test_board: $(OBJ_FILES) $(TEST_DIR)/test_board.o
	$(CXX) $(CXXFLAGS) -o $@ $^

$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(TEST_DIR)/%.o: $(TEST_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(BIN) $(OBJ_FILES) $(TEST_OBJ_FILES)

.PHONY: all clean
