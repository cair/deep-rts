//
// Created by per on 18.10.2020.
//

#ifndef HORIZON_WEBSERVER_H
#define HORIZON_WEBSERVER_H


#include <memory>
#include <functional>
#include <string>
namespace drogon{
    class HttpRequest;
    class HttpResponse;
}

class AppSettings;
class AI;

class Webserver {
    const std::string host;
    const int port;
    const bool development;

public:
    Webserver(const std::string& _host, int _port, bool development);
    void start();
    void handle(const std::shared_ptr<drogon::HttpRequest> &req,
                std::function<void(const std::shared_ptr<drogon::HttpResponse> &)> &&callback, int p1,
                const std::string&p2, const std::string &p3, int p4) const;
};


#endif //HORIZON_WEBSERVER_H
