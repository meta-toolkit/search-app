#include <algorithm>
#include "my_stub_server.h"
#include "corpus/document.h"
#include "index/ranker/ranker.h"
#include "index/ranker/ranker_factory.h"
#include "index/ranker/okapi_bm25.h"
#include "logging/logger.h"
#include "util/time.h"
#include "util/filesystem.h"

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

    auto config = cpptoml::make_table();
    auto elapsed = meta::common::time(
        [&]()
        {
            std::unique_ptr<meta::index::ranker> ranker;
            try
            {
                ranker = meta::index::ranker_factory::get().create(
                    ranker_method, *config);
            }
            catch (meta::index::ranker_factory::exception& ex)
            {
                LOG(error) << " -> couldn't create ranker, defaulting to bm25"
                           << ENDLG;
                ranker = make_unique<meta::index::okapi_bm25>();
            }

            for (auto& result : ranker->score(*idx_, query, 50))
            {
                Json::Value obj{Json::objectValue};
                obj["score"] = result.score;
                obj["name"] = idx_->doc_name(result.d_id);
                obj["path"] = idx_->doc_path(result.d_id);
                json_ret["results"].append(obj);
            }
        });

    json_ret["elapsed_time"] = static_cast<double>(elapsed.count());

    LOG(info) << "Done running query. (" << elapsed.count() << "ms)" << ENDLG;
    return json_ret;
}
