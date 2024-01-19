# Stock Market
This is a C++-based project that aims to implement a Stock Market application that allows multiple users to view/trade stocks through a GUI and provide per-user information and stock market data.

# Build the project

## Linux
```command
// To run the backend
// Navigate to /Backend

make
cd services

//Run these on two bashes independently to initialize the backend services

./StockMaret
./StockService

// To run the frontend
// Navigate to /Frontend

// For web-view
npm run dev

// For desktop view using electron
npm run electron
```

**Note:** I implemented a logger and it is enabled by default, to disable it you can set the macro (DISABLE_LOGGING) in Backend/Logger/Logger.h to 1
  

# UML Design
![Stock Market](Design%20System/Stock%20Market.jpg)

# Database Schema
![Database Schema](Design%20System/Database%20Schema.png)

# Architecture Diagram
![Architecture Diagram](Design%20System/Architecture%20Diagram.png)

# Design Patterns Used
1. Chain of Responsibilities -> Logger, ConsoleLogger, FileLogger
2. Singleton -> AuthController, StockController, StockMarketController, TransactionController, UserController, DatabaseHandler
3. MVC pattern

# Contributors
1. Hazem Adel



