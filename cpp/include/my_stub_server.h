#include <jsonrpccpp/server/connectors/httpserver.h>
#include "abstractstubserver.h"
#include "cpptoml.h"

using namespace jsonrpc;

class MyStubServer : public AbstractStubServer
{
  public:
    MyStubServer(AbstractServerConnector& connector,
                 const cpptoml::table& config);
    virtual Json::Value search(const std::string& query);
};
