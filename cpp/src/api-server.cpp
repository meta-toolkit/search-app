#include <chrono>
#include "cpptoml.h"
#include "index/make_index.h"
#include "logging/logger.h"
#include "my_stub_server.h"

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " config.toml" << std::endl;
        return 1;
    }

    using namespace meta;
    logging::set_cerr_logging();

    LOG(info) << "Loading inverted index from config file: " << argv[1]
              << ENDLG;

    auto start = std::chrono::high_resolution_clock::now();
    auto idx = index::make_index<index::inverted_index>(argv[1]);

    LOG(info) << "Done loading index. ("
              << std::chrono::duration_cast<std::chrono::milliseconds>(
                     std::chrono::high_resolution_clock::now() - start).count()
              << "ms)" << ENDLG;

    LOG(info) << "Starting API server on port 8088" << ENDLG;

    start = std::chrono::high_resolution_clock::now();
    HttpServer httpserver{8088};
    MyStubServer s{httpserver, idx};

    LOG(info) << "Done starting server. ("
              << std::chrono::duration_cast<std::chrono::milliseconds>(
                     std::chrono::high_resolution_clock::now() - start).count()
              << "ms)" << ENDLG;

    s.StartListening();
    getchar();
    s.StopListening();
}
