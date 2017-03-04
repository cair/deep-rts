#include <vector>
#include <thread>
#include <memory>
#include <functional>

#include <zmq.hpp>
#include <game/proto/TestMessage.pb.h>
#include "game/lib/zhelpers.hpp"

#include "game/proto/GameMessage.pb.h"

class ServerWorker {
public:
    ServerWorker(
            zmq::context_t &ctx,
            int sock_type,
            int id_,
            std::shared_ptr<Game> game,
            std::shared_ptr<Player> player,
            bool isInput): isInput(isInput), player(player), game(game), id(id_), ctx_(ctx), worker_(ctx_, sock_type) {}

    void work() {
        int port = 5000 + id;

        //std::string connectionString = std::string("ipc:///" + std::to_string(port));
        std::string connectionString = std::string("tcp://*:" + std::to_string(port));
        std::cout << connectionString << std::endl;

        //  Prepare our context and socket
        zmq::context_t context (1);
        zmq::socket_t socket (context, ZMQ_PAIR);
        socket.bind (connectionString);

        int interval = 1000;
        clock_t now = clock();
        clock_t next = now + interval;
        int msgpsec = 0;
        zmq::message_t request;


        while (true) {
            now = clock();



            if(isInput) {
                // Should only receive
                socket.recv(&request);
                std::string msgStr(static_cast<char *>(request.data()), request.size());
                onReceieve(msgStr);
                msgpsec++;
            } else {
                std::string toStr;

                GameMessage gMessage = game->serialize(player);
                toStr = gMessage.SerializeAsString();
                int sz = toStr.length();
                zmq::message_t *query = new zmq::message_t(sz);
                memcpy(query->data (), toStr.c_str(), sz);
                socket.send (*query);

                msgpsec++;
            }


            if (now >= next) {
                next = now + interval;
                std::cout << game->getSeconds() << std::endl;
                std::cout << "Messages per sec: " << msgpsec << ((isInput) ? " IN" : " OUT") << std::endl;
                msgpsec = 0;
            }
        }
    }

    void onReceieve(std::string &data){
        TestMessage msg;
        msg.ParseFromString(data);
    }


private:
    zmq::context_t &ctx_;
    zmq::socket_t worker_;
    int id;
    Game *game;
    Player *player;
    bool isInput;
};