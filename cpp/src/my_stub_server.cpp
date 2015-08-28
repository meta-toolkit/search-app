#include <algorithm>
#include <sstream>
#include "my_stub_server.h"
#include "corpus/document.h"
#include "index/ranker/ranker_factory.h"
#include "logging/logger.h"
#include "util/time.h"

MyStubServer::MyStubServer(AbstractServerConnector& connector,
                           std::shared_ptr<meta::index::inverted_index> idx)
    : AbstractStubServer{connector}, idx_(std::move(idx))
{
}

Json::Value MyStubServer::search(const std::string& query_text,
                                 const std::string& ranker_method)
{
    using namespace meta;

    LOG(info) << "Running query using " << ranker_method << ": \""
              << query_text.substr(0, 40) << "...\"" << ENDLG;

    Json::Value json_ret{Json::objectValue};
    meta::corpus::document query;
    query.content(query_text);

    json_ret["results"] = Json::arrayValue;

    auto elapsed = meta::common::time(
        [&]()
        {
            if(last_ranker_method_ != ranker_method)
            {
                std::ofstream rconfig{"ranker.toml"};
                rconfig << "method = \"" << ranker_method << "\"\n";
                rconfig.close();
                last_ranker_method_ = ranker_method;
                try
                {
                    ranker_ = meta::index::make_ranker(
                        cpptoml::parse_file("ranker.toml"));
                }
                catch(meta::index::ranker_factory::exception& ex)
                {
                    LOG(error) << " -> couldn't create ranker!" << ENDLG;
                }
            }
            for (auto& result : ranker_->score(*idx_, query, 20))
            {
                Json::Value obj{Json::objectValue};
                obj["score"] = result.score;
                obj["name"] = idx_->doc_name(result.d_id);
                obj["path"] = idx_->doc_path(result.d_id);
                json_ret["results"].append(obj);
            }
        });

    json_ret["elapsed_time"] = static_cast<double>(elapsed.count());

    LOG(info) << "Done running query. (" << elapsed.count() / 1000 << "ms)"
              << ENDLG;
    return json_ret;
}
