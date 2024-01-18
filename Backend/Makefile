# Makefile for building StockMarket and StocksService applications

# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic  

# Source files for StockMarket
SRC_STOCKMARKET := dal/DatabaseHandler/DatabaseHandler.cpp \
                   Service.cpp \
                   controllers/AuthController.cpp \
                   controllers/StockController.cpp \
                   controllers/StockMarketController.cpp \
                   controllers/TransactionController.cpp \
                   controllers/UserController.cpp

# Source files for StocksService
SRC_STOCKSSERVICE := dal/DatabaseHandler/DatabaseHandler.cpp \
                     StocksService.cpp \
                     controllers/AuthController.cpp \
                     controllers/StockController.cpp \
                     controllers/StockMarketController.cpp \
                     controllers/TransactionController.cpp \
                     controllers/UserController.cpp

# Object files for StockMarket
OBJ_STOCKMARKET := $(SRC_STOCKMARKET:.cpp=.o)

# Object files for StocksService
OBJ_STOCKSSERVICE := $(SRC_STOCKSSERVICE:.cpp=.o)

# Libraries
LIBS := -lsqlite3 -I/json-develop/single_include -I/jwt-cpp -lssl -lcrypto -lpthread -lboost_system -lboost_thread

# Executables
EXEC_STOCKMARKET := StockMarket
EXEC_STOCKSSERVICE := StocksService
OUTPUT_DIR := services

.PHONY: all clean

all: $(OUTPUT_DIR)/$(EXEC_STOCKMARKET) $(OUTPUT_DIR)/$(EXEC_STOCKSSERVICE)

$(OUTPUT_DIR)/$(EXEC_STOCKMARKET): $(OBJ_STOCKMARKET) | $(OUTPUT_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIBS)

$(OUTPUT_DIR)/$(EXEC_STOCKSSERVICE): $(OBJ_STOCKSSERVICE) | $(OUTPUT_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIBS)

$(OUTPUT_DIR):
	mkdir -p $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_STOCKMARKET) $(OBJ_STOCKSSERVICE) $(OUTPUT_DIR) dal/Database/StockMarket.db
