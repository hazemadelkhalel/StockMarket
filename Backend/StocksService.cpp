#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "json-develop/single_include/nlohmann/json.hpp"
#include "cpp-httplib-master/httplib.h"
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>

using json = nlohmann::json;

using json = nlohmann::json;
namespace beast = boost::beast;
namespace http = boost::beast::http;
namespace websocket = boost::beast::websocket;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

struct Stock
{
    std::string company;
    int available_stocks;
    float initial_price;
    float current_price;
    float change;
    float profit;

    // Constructor to initialize stock
    Stock(const std::string &name, int stocks, float price, float change = 0.00, float profit = 0.00)
        : company(name), available_stocks(stocks), initial_price(price), current_price(price), change(change), profit(profit) {}
};

class StocksService
{
private:
    std::vector<Stock> stocks;
    std::mutex mutex_;
    std::condition_variable cv_;
    bool data_ready = false;
    int maxAvailableStocks;

public:
    // Load stocks from JSON file
    void loadStocks(const std::string &jsonFile)
    {
        std::ifstream file(jsonFile);
        if (!file.is_open())
        {
            std::cerr << "Error opening JSON file" << std::endl;
            return;
        }

        json root;
        file >> root;
        file.close();

        for (const auto &stock : root["stocks"])
        {
            std::string company_name = stock["company"];
            int available_stocks = std::stoi(stock["available_stocks"].dump());
            float initial_price = std::stof(stock["initial_price"].dump());

            stocks.emplace_back(company_name, available_stocks, initial_price);
            if (available_stocks > maxAvailableStocks)
                maxAvailableStocks = available_stocks;
        }

        // TODO: Add stocks to database
    }
    double calculatePriceChange(int availableStocks)
    {
        // Calculate a factor based on the available stocks
        double factor = 1.0 - static_cast<double>(availableStocks) / maxAvailableStocks;

        // Generate random value between -0.1 and 0.1
        double randomChange = ((rand() % 200) - 100) / 1000.0;

        // Adjust the randomChange based on the factor
        return randomChange * factor;
    }

    void updateStockPricesAndNotify(websocket::stream<tcp::socket> &ws)
    {
        while (true)
        {
            // Lock the mutex to update stock prices
            std::unique_lock<std::mutex> lock(mutex_);

            for (auto &stock : stocks)
            {
            }

            for (auto &stock : stocks)
            {
                // Generate random value between -0.1 and 0.1
                double randomChange = calculatePriceChange(stock.available_stocks);

                double tempCurrentPrice = stock.current_price;
                stock.current_price *= (1 + randomChange);
                stock.change = (stock.current_price - tempCurrentPrice) / tempCurrentPrice * 100;
                stock.profit = (stock.current_price - stock.initial_price) * stock.available_stocks;
            }

            json result;
            for (const auto &stock : stocks)
            {
                result[stock.company] = {
                    {"available_stocks", stock.available_stocks},
                    {"initial_price", stock.initial_price},
                    {"current_price", stock.current_price},
                    {"change", stock.change},
                    {"profit", stock.profit}};
            }

            // Send the JSON message to the client
            try
            {
                ws.write(boost::asio::buffer(result.dump()));
            }
            catch (const std::exception &e)
            {
                std::cerr << "Error writing to WebSocket: " << e.what() << std::endl;
                break;
            }

            // Release the lock and wait for the next iteration
            lock.unlock();
            std::this_thread::sleep_for(std::chrono::seconds(10)); // Adjust as needed
        }
    }
};

int main()
{
    // Seed for random number generation
    std::srand(std::time(0));

    // Create and initialize StocksService
    StocksService stocksService;
    stocksService.loadStocks("stocks.json");

    auto const address = net::ip::make_address("127.0.0.1");
    auto const port = static_cast<unsigned short>(std::atoi("8083"));

    net::io_context ioc;

    tcp::acceptor acceptor{
        ioc, {address, port}};

    for (;;)
    {

        // Boost.Asio setup
        boost::asio::io_context ioc;

        // WebSocket setup
        websocket::stream<tcp::socket> ws(ioc);

        // Define the WebSocket server
        acceptor.accept(ws.next_layer());

        // Perform WebSocket handshake
        ws.accept();

        // Start the WebSocket thread
        std::thread websocket_thread([&ws, &stocksService]()
                                     {
        try
        {
            while (true)
            {
                // Handle WebSocket messages if needed
                boost::beast::flat_buffer buffer;
                boost::system::error_code ec;

                // Receive WebSocket message
                ws.read(buffer, ec);

                if (ec == websocket::error::closed)
                {
                    // Handle WebSocket closure
                    std::cout << "WebSocket closed by the client." << std::endl;
                    break;
                }
                else if (ec)
                {
                    // Handle other WebSocket errors
                    std::cerr << "WebSocket error: " << ec.message() << std::endl;
                    break;
                }
                stocksService.updateStockPricesAndNotify(ws);
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "Exception in WebSocket thread: " << e.what() << std::endl;
        } });

        // Start the stocks update thread with a WebSocket connection
        std::thread stocks_thread([&stocksService, &ws]()
                                  { stocksService.updateStockPricesAndNotify(ws); });

        // Wait for the threads to finish
        websocket_thread.join();
        stocks_thread.join();
    }
    return 0;
}
