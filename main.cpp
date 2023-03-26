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

        TaskServer task_server(io_context, static_cast<unsigned short>(port));

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


/*
    ba::io_context io_context;

    ba::ip::tcp::endpoint ep(
        ba::ip::tcp::v4(),
        9999
    );

    ba::ip::tcp::acceptor acc(io_context, ep);

    std::cout << "_1_" << std::endl;

    while (true) {
        std::cout << "_2_" << std::endl;

        auto sock = ba::ip::tcp::socket(io_context);

        std::cout << "_3_" << std::endl;

        acc.accept(sock);

        std::cout << "_4_" << std::endl;

        std::thread(client_session, std::move(sock)).detach();

        std::cout << "_5_" << std::endl;
    }
    */

/*
void client_session(ba::ip::tcp::socket sock) {
    while (true) {
        try {
            char data[4];
            size_t len = sock.read_some(ba::buffer(data));

            std::cout << "receive " << len << "=" << std::string{ data, len } << std::endl;

            if (0 == std::string{ data, len }.compare("exit")) {
                break;
            }

            ba::write(sock, ba::buffer("pong", 4));
        }
        catch (const std::exception& e) {
            std::cerr << "client_session exception! " << e.what() << std::endl;
            break;
        }
    }
}*/