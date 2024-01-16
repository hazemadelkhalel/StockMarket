# CMakeLists.txt for building StockMarket application

cmake_minimum_required(VERSION 3.10)

project(StockMarket)

find_package(jwt-cpp REQUIRED)

include_directories(${jwt-cpp_INCLUDE_DIRS})



# Compiler and flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -pthread")

# Source files
set(SRC
    dal/DatabaseHandler/DatabaseHandler.cpp
    Service.cpp
    controllers/AuthController.cpp
    controllers/StockController.cpp
    controllers/StockMarketController.cpp
    controllers/TransactionController.cpp
    controllers/UserController.cpp
)

# Object files
add_executable(StockMarket ${SRC})

# Libraries
target_link_libraries(StockMarket PRIVATE sqlite3 ${jwt-cpp_LIBRARIES})
target_include_directories(StockMarket PRIVATE /json-develop/single_include)

# Output directory
set(OUTPUT_DIR services)
set_target_properties(StockMarket PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR}
)

# Clean
add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/${OUTPUT_DIR}
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/dal/Database/StockMarket.db
    COMMENT "Cleaning project"
)

# Set as default target
add_custom_target(default
    ALL
    DEPENDS StockMarket
)
