#pragma once

#include <proto/faasm.pb.h>
#include <infra/infra.h>
#include <util/util.h>

#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>

using namespace Pistache;

namespace edge {
    class FunctionEndpoint {
    public:
        FunctionEndpoint();

        void start();

        void shutdown();

        std::string handleFunction(message::Message &msg);
    private:
        std::shared_ptr<Http::Endpoint> httpEndpoint;
        Rest::Router router;

        void setupRoutes();

        void handleFunctionWrapper(const Rest::Request &request, Http::ResponseWriter response);

        void handleAsyncFunctionWrapper(const Rest::Request &request, Http::ResponseWriter response);

        message::Message buildMessageFromRequest(const Rest::Request &request);
    };
}