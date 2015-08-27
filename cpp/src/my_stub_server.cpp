#include <algorithm>
#include <sstream>
#include "my_stub_server.h"
#include "logging/logger.h"

MyStubServer::MyStubServer(AbstractServerConnector& connector,
                           const cpptoml::table& config)
    : AbstractStubServer{connector}
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
