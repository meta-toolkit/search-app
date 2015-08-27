#include <chrono>
#include "cpptoml.h"
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

    LOG(info) << "Starting API server on port 8088" << ENDLG;
    auto start = std::chrono::high_resolution_clock::now();

    HttpServer httpserver{8088};

    auto config = cpptoml::parse_file(argv[1]);
    MyStubServer s{httpserver, config};

    LOG(info) << "Done. ("
              << std::chrono::duration_cast<std::chrono::milliseconds>(
                     std::chrono::high_resolution_clock::now() - start).count()
              << "ms)" << ENDLG;

    s.StartListening();
    getchar();
    s.StopListening();
}
