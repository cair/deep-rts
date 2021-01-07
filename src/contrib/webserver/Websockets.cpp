//
// Created by per on 1/5/21.
//

//
// Created by per on 18.10.2020.
//

#include "Websockets.h"

#include <effolkronium/random.hpp>

struct UserInfo{
    int id;
};

using json = nlohmann::json;
void Websockets::handleNewMessage(const drogon::WebSocketConnectionPtr &conn,
                                  std::string &&message,
                                  const drogon::WebSocketMessageType &type)
{
    if(message == "ping"){
        return;
    }


    LOG_DEBUG << "new websocket message:" << message;
    json data;
    try{
        data = json::parse(message);
    }catch(json::parse_error &jsonError){
        send(conn, "error", jsonError.what());
    }

    // Todo User Input
    if(data["event"] == "ping-me"){
        send(conn, "ping-me", "pong");
    }
}

void Websockets::handleConnectionClosed(const drogon::WebSocketConnectionPtr &conn)
{
    LOG_DEBUG << "websocket closed!";
    std::shared_ptr<UserInfo> info = conn->getContext<UserInfo>();
    _mapMtx.lock();
    _clientsMap.erase(info->id);
    _mapMtx.unlock();
}

void Websockets::handleNewConnection(const drogon::HttpRequestPtr &,
                                     const drogon::WebSocketConnectionPtr &conn)
{


    auto info = std::make_shared<UserInfo>();
    info->id = effolkronium::random_static::get(1, INT32_MAX);


    conn->setContext(info);

    _mapMtx.lock();
    _clientsMap[info->id] = conn;
    _mapMtx.unlock();

    LOG_DEBUG << "new websocket connection!";
    send(conn, "connect", json{"hello"});
}




void Websockets::send(const drogon::WebSocketConnectionPtr &conn, const std::string& channel, const json& data){
    conn->send(json{
            {"event", channel},
            {"data", data}
    }.dump(), drogon::WebSocketMessageType::Text);
}





Websockets::Websockets()= default;