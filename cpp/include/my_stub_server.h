#include <jsonrpccpp/server/connectors/httpserver.h>
#include "abstractstubserver.h"
#include "index/inverted_index.h"
#include "index/ranker/ranker.h"

using namespace jsonrpc;

class MyStubServer : public AbstractStubServer
{
  public:
    MyStubServer(AbstractServerConnector& connector,
                 std::shared_ptr<meta::index::inverted_index> idx);
    virtual Json::Value search(const std::string& query_text,
                               const std::string& ranker_method);

  private:
    std::shared_ptr<meta::index::inverted_index> idx_;
    std::unique_ptr<meta::index::ranker> ranker_;
    std::string last_ranker_method_;
};
