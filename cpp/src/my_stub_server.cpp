#include <algorithm>
#include <sstream>
#include "my_stub_server.h"
#include "logging/logger.h"

MyStubServer::MyStubServer(AbstractServerConnector& connector,
                           std::shared_ptr<meta::index::inverted_index> idx)
    : AbstractStubServer{connector}, idx_(std::move(idx))
{
}

Json::Value MyStubServer::search(const std::string& query)
{
    using namespace meta;

    LOG(info) << "Running query: \"" << query.substr(0, 40) << "...\"" << ENDLG;

    Json::Value json_ret{Json::objectValue};
    json_ret["results"] = Json::arrayValue;

    return json_ret;
}
