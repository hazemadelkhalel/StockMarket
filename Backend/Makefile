# Makefile for building StockMarket application
  
# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic -lpthread

# Source files
SRC := dal/DatabaseHandler/DatabaseHandler.cpp StockMarket.cpp controllers/UserController.cpp

# Object files
OBJ := $(SRC:.cpp=.o)

# Libraries
LIBS := -lsqlite3 -I /json-develop/single_include

# Executable
EXEC := StockMarket
OUTPUT_DIR := services

.PHONY: all clean

all: $(OUTPUT_DIR)/$(EXEC)

$(OUTPUT_DIR)/$(EXEC): $(OBJ) | $(OUTPUT_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIBS)

$(OUTPUT_DIR):
	mkdir -p $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ) $(OUTPUT_DIR) dal/Database/StockMarket.db

