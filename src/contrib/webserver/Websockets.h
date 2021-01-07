//
// Created by per on 18.10.2020.
//

#ifndef HORIZON_WEBSOCKETS_H
#define HORIZON_WEBSOCKETS_H
#include <drogon/WebSocketController.h>
#include <nlohmann/json.hpp>

class AI;


class Websockets : public drogon::WebSocketController<Websockets, false> {
private:
    std::map<int, drogon::WebSocketConnectionPtr> _clientsMap; //UserID--->connection
    std::mutex _mapMtx;
public:

    Websockets();
    void handleNewMessage(const drogon::WebSocketConnectionPtr &,
                                  std::string &&,
                                  const drogon::WebSocketMessageType &) override;
    void handleConnectionClosed(
            const drogon::WebSocketConnectionPtr &) override;
    void handleNewConnection(const drogon::HttpRequestPtr &,
                                     const drogon::WebSocketConnectionPtr &) override;
    WS_PATH_LIST_BEGIN
    // list path definations here;
    WS_PATH_ADD("/ws", "drogon::LocalHostFilter", drogon::Get);
    WS_PATH_LIST_END




    static void send(const drogon::WebSocketConnectionPtr &conn, const std::string& channel, const nlohmann::json & data);
};


#endif //HORIZON_WEBSOCKETS_H
