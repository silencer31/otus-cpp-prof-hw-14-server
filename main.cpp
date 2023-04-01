#include <iostream>

#include "task_server.h"


namespace ba = boost::asio;


int main(int argc, char* argv[])
{
    std::locale::global(std::locale(""));

    try
    {
        if (argc != 2) {
            std::cerr << "Usage: task_server <port>\n";
            return 1;
        }

        int port = std::atoi(argv[1]);

        if (port <= 0) {
            std::cerr << "Incorrect port value\n";
            return 1;
        }

        ba::io_context io_context;
        
        TaskServer task_server(io_context, static_cast<unsigned short>(port), std::make_shared<DataStorage>());

        std::cout << "___ io run ___ waiting" << std::endl;
        io_context.run();
        std::cout << "___ io run ___ finished" << std::endl;

    }
    catch (const std::exception& ex)
    {
        std::cerr << "Exception: " << ex.what() << "\n";
    }

    return 0;
}