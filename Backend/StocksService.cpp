#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "json-develop/single_include/nlohmann/json.hpp"
#include "cpp-httplib-master/httplib.h"
#include "dal/dtos/StockDTO.h"
#include "dal/DatabaseHandler/DatabaseHandler.h"
#include "controllers/StockController.h"

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

//------------------------------------------------------------------------------

class StocksService
{
private:
    std::vector<StockDTO> stocks;
    std::mutex mutex_;
    std::condition_variable cv_;
    bool data_ready = false;
    int maxAvailableStocks;

public:
    std::vector<StockDTO> getStocks()
    {
        return stocks;
    }
    int getMaxAvailableStocks()
    {
        return maxAvailableStocks;
    }
    void setStocks(std::vector<StockDTO> stocks)
    {
        this->stocks = stocks;
    }
    void setMaxAvailableStocks(int maxAvailableStocks)
    {
        this->maxAvailableStocks = maxAvailableStocks;
    }
    // Load stocks from JSON file
    void
    loadStocks(const std::string &jsonFile)
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

            if (available_stocks > maxAvailableStocks)
                maxAvailableStocks = available_stocks;

            StockDTO stockDTO = StockDTO(1, company_name, available_stocks, initial_price, initial_price);

            StockController *stockController = StockController::getInstance();
            auto response = stockController->addStock(stockDTO);
            if (response["status"] == "failed")
            {
                std::cout << response["message"] << std::endl;
                continue;
            }
            else
            {
                stockDTO.id = response["stock"]["id"];
                stocks.push_back(stockDTO);
            }
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
};

StocksService stocksService;

void updateStockPricesAndNotify()
{
    std::vector<StockDTO> stocks = stocksService.getStocks();
    int maxAvailableStocks = stocksService.getMaxAvailableStocks();

    auto response = StockController::getInstance()->getAllStocks();
    for (int i = 0; i < (int)response["stocks"].size(); i++)
    {
        auto stock = StockDTO(response["stocks"][i]["id"], response["stocks"][i]["company"], response["stocks"][i]["available_stocks"], response["stocks"][i]["initial_price"], response["stocks"][i]["current_price"], response["stocks"][i]["change"], response["stocks"][i]["profit"]);
        if (stock.available_stocks > maxAvailableStocks)
            maxAvailableStocks = stock.available_stocks;
        if (i >= (int)stocks.size())
            stocks.push_back(stock);
        else
            stocks[i] = stock;
    }

    for (auto &stock : stocks)
    {
        // Generate random value between -0.1 and 0.1
        double randomChange = stocksService.calculatePriceChange(stock.available_stocks);

        double tempCurrentPrice = stock.current_price;
        stock.current_price *= (1 + randomChange);
        stock.change = (stock.current_price - tempCurrentPrice) / tempCurrentPrice * 100;
        stock.profit = (stock.current_price - stock.initial_price) * stock.available_stocks;

        StockDTO stockDTO = StockDTO(stock.id, stock.company, stock.available_stocks, stock.initial_price, stock.current_price, stock.change, stock.profit);

        StockController *stockController = StockController::getInstance();
        auto response = stockController->updateStock(stockDTO);
        if (response["status"] == "failed")
        {
            std::cout << response["message"] << std::endl;
        }
    }

    json result = json::array();
    for (auto &stock : stocks)
    {
        std::cout << "Stock: " << stock.company << " " << stock.id << std::endl;
        json stockJson = {
            {"id", stock.id},
            {"company", stock.company},
            {"available_stocks", stock.available_stocks},
            {"initial_price", stock.initial_price},
            {"current_price", stock.current_price},
            {"change", stock.change},
            {"profit", stock.profit},
        };
        result.push_back(stockJson);
    }
    stocksService.setStocks(stocks);
    stocksService.setMaxAvailableStocks(maxAvailableStocks);
    std::cout << result.dump() << std::endl;
}

// Report a failure
void fail(beast::error_code ec, char const *what)
{
    std::cerr << what << ": " << ec.message() << "\n";
}

// Echoes back all received WebSocket messages
class session : public std::enable_shared_from_this<session>
{
    websocket::stream<beast::tcp_stream> ws_;
    beast::flat_buffer buffer_;

public:
    // Take ownership of the socket
    explicit session(tcp::socket &&socket)
        : ws_(std::move(socket))
    {
    }

    // Get on the correct executor
    void
    run()
    {
        // We need to be executing within a strand to perform async operations
        // on the I/O objects in this session. Although not strictly necessary
        // for single-threaded contexts, this example code is written to be
        // thread-safe by default.
        net::dispatch(ws_.get_executor(),
                      beast::bind_front_handler(
                          &session::on_run,
                          shared_from_this()));
    }

    // Start the asynchronous operation
    void
    on_run()
    {
        // Set suggested timeout settings for the websocket
        ws_.set_option(
            websocket::stream_base::timeout::suggested(
                beast::role_type::server));

        // Set a decorator to change the Server of the handshake
        ws_.set_option(websocket::stream_base::decorator(
            [](websocket::response_type &res)
            {
                res.set(http::field::server,
                        std::string(BOOST_BEAST_VERSION_STRING) +
                            " websocket-server-async");
            }));
        // Accept the websocket handshake
        ws_.async_accept(
            beast::bind_front_handler(
                &session::on_accept,
                shared_from_this()));
    }

    void
    on_accept(beast::error_code ec)
    {
        if (ec)
            return fail(ec, "accept");

        // Read a message
        do_read();
    }

    void
    do_read()
    {
        // Read a message into our buffer
        ws_.async_read(
            buffer_,
            beast::bind_front_handler(
                &session::on_read,
                shared_from_this()));
    }
    void
    on_read(
        beast::error_code ec,
        std::size_t bytes_transferred)
    {
        boost::ignore_unused(bytes_transferred);

        // This indicates that the session was closed
        if (ec == websocket::error::closed)
            return;

        if (ec)
            return fail(ec, "read");

        // Echo the message
        ws_.text(ws_.got_text());

        json root = json::array();
        updateStockPricesAndNotify();
        for (auto &stock : stocksService.getStocks())
        {
            std::cout << "Stock: " << stock.company << " " << stock.id << std::endl;
            json stockJson = {
                {"id", stock.id},
                {"company", stock.company},
                {"available_stocks", stock.available_stocks},
                {"initial_price", stock.initial_price},
                {"current_price", stock.current_price},
                {"change", stock.change},
                {"profit", stock.profit},
            };
            root.push_back(stockJson);
        }

        ws_.async_write(
            net::buffer(root.dump()),
            beast::bind_front_handler(
                &session::on_write,
                shared_from_this()));
    }
    void
    on_write(
        beast::error_code ec,
        std::size_t bytes_transferred)
    {
        boost::ignore_unused(bytes_transferred);

        if (ec)
            return fail(ec, "write");

        // Clear the buffer
        buffer_.consume(buffer_.size());

        // Do another read
        do_read();
    }
};

//------------------------------------------------------------------------------

// Accepts incoming connections and launches the sessions
class listener : public std::enable_shared_from_this<listener>
{
    net::io_context &ioc_;
    tcp::acceptor acceptor_;

public:
    listener(
        net::io_context &ioc,
        tcp::endpoint endpoint)
        : ioc_(ioc), acceptor_(ioc)
    {
        beast::error_code ec;

        // Open the acceptor
        acceptor_.open(endpoint.protocol(), ec);
        if (ec)
        {
            fail(ec, "open");
            return;
        }

        // Allow address reuse
        acceptor_.set_option(net::socket_base::reuse_address(true), ec);
        if (ec)
        {
            fail(ec, "set_option");
            return;
        }

        // Bind to the server address
        acceptor_.bind(endpoint, ec);
        if (ec)
        {
            fail(ec, "bind");
            return;
        }

        // Start listening for connections
        acceptor_.listen(
            net::socket_base::max_listen_connections, ec);
        if (ec)
        {
            fail(ec, "listen");
            return;
        }
    }

    // Start accepting incoming connections
    void
    run()
    {
        do_accept();
    }

private:
    void
    do_accept()
    {
        // The new connection gets its own strand
        acceptor_.async_accept(
            net::make_strand(ioc_),
            beast::bind_front_handler(
                &listener::on_accept,
                shared_from_this()));
    }

    void
    on_accept(beast::error_code ec, tcp::socket socket)
    {
        if (ec)
        {
            fail(ec, "accept");
        }
        else
        {
            // Create the session and run it
            std::make_shared<session>(std::move(socket))->run();
        }

        // Accept another connection
        do_accept();
    }
};

//------------------------------------------------------------------------------

int main()
{
    // Check command line arguments.
    // if (argc != 4)
    // {
    //     std::cerr << "Usage: websocket-server-async <address> <port> <threads>\n"
    //               << "Example:\n"
    //               << "    websocket-server-async 0.0.0.0 8080 1\n";
    //     return EXIT_FAILURE;
    // }
    auto const address = net::ip::make_address("127.0.0.1");
    auto const port = static_cast<unsigned short>(8083);
    auto const threads = std::max<int>(1, std::atoi("4"));

    // Create and initialize StocksService
    stocksService.loadStocks("../stocks.json");

    boost::asio::io_context io_context;
    tcp::socket socket(io_context);
    websocket::stream<tcp::socket> ws(std::move(socket));

    // The io_context is required for all I/O
    net::io_context ioc{threads};

    // Create and launch a listening port
    std::make_shared<listener>(ioc, tcp::endpoint{address, port})->run();

    // Run the I/O service on the requested number of threads
    std::vector<std::thread> v;
    v.reserve(threads - 1);
    for (auto i = threads - 1; i > 0; --i)
        v.emplace_back(
            [&ioc]
            {
                ioc.run();
            });
    ioc.run();

    return EXIT_SUCCESS;
}