//
// Created by per on 18.10.2020.
//

#include <iostream>
#include <drogon/drogon.h>
#include <cmrc/cmrc.hpp>
#include <utility>

#include "Webserver.h"
#include "Websockets.h"

CMRC_DECLARE(DeepRTSResources);

Webserver::Webserver(const std::string& _host, int _port, bool _development):
    host(_host.empty() ? "0.0.0.0" : _host),
    port(_port),
    development(_development)
{

}

void Webserver::start(){

    drogon::app().addListener(host, port);
    drogon::app().registerController(std::make_shared<Websockets>());
    drogon::app().registerHandler("/{path}",
                                  [this](const drogon::HttpRequestPtr& req,
                                         std::function<void(const drogon::HttpResponsePtr&)>&& callback,
                                         const std::string& path) {


                                      try {


                                          if(development){
                                              auto resp = drogon::HttpResponse::newRedirectionResponse("http://localhost:4201");
                                              callback(resp);

                                          }else{

                                              auto fs = cmrc::DeepRTSResources::get_filesystem();
                                              std::string calcPath;
                                              if (path.empty()) {
                                                  calcPath = "index.html";
                                              }
                                              else {
                                                  calcPath = path;
                                              }
                                              auto fileName = "webclient/dist/webclient/" + calcPath;
                                              auto file = fs.open(fileName);
                                              auto resp = drogon::HttpResponse::newHttpResponse();
                                              resp->setBody(file.begin());
                                              callback(resp);

                                          }



                                      }
                                      catch (const std::exception& e) {
                                          auto resp = drogon::HttpResponse::newHttpResponse();
                                          resp->setStatusCode(drogon::HttpStatusCode::k404NotFound);
                                          callback(resp);
                                      }



                                  });
    //drogon::app().enableSession(60);

    auto t = std::thread([this](){
        drogon::app().run();
    });
    t.detach();
}



void Webserver::handle(const drogon::HttpRequestPtr &req,
            std::function<void(const drogon::HttpResponsePtr &)> &&callback,
            int p1,
            const std::string &p2,
            const std::string &p3,
            int p4) const
{
    drogon::HttpViewData data;
    data.insert("title", std::string("ApiTest::get"));
    std::unordered_map<std::string, std::string> para;
    para["int p1"] = std::to_string(p1);
    para["string p2"] = p2;
    para["string p3"] = p3;
    para["int p4"] = std::to_string(p4);

    data.insert("parameters", para);
    auto res = drogon::HttpResponse::newHttpViewResponse("ListParaView", data);
    callback(res);
}