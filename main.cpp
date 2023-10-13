#include "task_server.h"

#include <boost/filesystem/operations.hpp>

#include <iostream>

#ifdef WIN32
#define appname "task_server.exe"
#else
#define appname "./task_server"
#endif

namespace ba = boost::asio;


int main(int argc, char* argv[])
{
    std::locale::global(std::locale(""));
    
    if (argc != 3) {
        std::cout << "Usage: " << appname << " <port> <data base file name>" << std::endl;
        return 1;
    }

    int port = std::atoi(argv[1]);

    if (port <= 0) {
        std::cerr << "Incorrect port value\n";
        return 1;
    }

    std::string db_name{ argv[2] };
    if (!db_name.ends_with(".sqlite")) {
        std::cout << "Only sqlite data base file is supported!" << std::endl;
        return 1;
    }

    // ѕровер€ем наличие указанного файла с базой данных.
    if (!boost::filesystem::exists(db_name)) {
        std::cout << "Specified data base file not found!" << std::endl;
        return 1;
    }

    try
    {
        ba::io_context io_context;
        
        // —оздание сервера с одновременным созданием хранилища данных.
        std::shared_ptr<TaskServer> task_server_ptr = std::make_shared<TaskServer>(
            io_context,
            static_cast<unsigned short>(port), 
            std::make_shared<DataStorage>(db_name.c_str())
        );

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