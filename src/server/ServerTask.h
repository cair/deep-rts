//
// Created by Per-Arne on 02.03.2017.
//

#pragma once


//  This is our server task.
//  It uses the multithreaded server model to deal requests out to a pool
//  of workers and route replies back to clients. One worker can handle
//  one request at a time but one client can talk to multiple workers at
//  once.
#include "ServerWorker.h"
#include "../game/Game.h"
class ServerTask {
public:
    ServerTask(Game *game): game(game), ctx_(1){

    }

    void run() {
        int nPlayers = 4;

        /*zmq::context_t context (1);

        zmq::socket_t pub (context, ZMQ_PUB);
        pub.bind("tcp://127.0.0.1:5000");

        zmq::socket_t receive (context, ZMQ_PULL);
        receive.bind("tcp://127.0.0.1:5001");

        zmq::message_t message;
        while(true) {
            receive.recv(&message, ZMQ_NOBLOCK);
            // DO something with received message
            pub.send(message, ZMQ_NOBLOCK);
        }*/

        std::vector<ServerWorker *> worker;
        std::vector<std::thread *> worker_thread;
        int counter = 0;
        for (int i = 0; i < nPlayers; ++i) {

            ServerWorker *in = new ServerWorker(ctx_, ZMQ_PAIR, counter++, game, game->players[i], true);
            ServerWorker *out = new ServerWorker(ctx_, ZMQ_PAIR, counter++, game, game->players[i], false);

            worker.push_back(in);
            worker.push_back(out);

            auto w1 = std::bind(&ServerWorker::work, in);
            auto w2 = std::bind(&ServerWorker::work, out);
            auto t1 = new std::thread(w1);
            auto t2 = new std::thread(w2);

            worker_thread.push_back(t1);
            worker_thread.push_back(t2);

            t1->detach();
            t2->detach();
        }

        //  Prepare our context and socket
        /*zmq::context_t context (1);
        zmq::socket_t socket (context, ZMQ_REP);
        socket.bind ("tcp://*:5556");

        int interval = 1000;
        clock_t now = clock();
        clock_t next = now + interval;
        int msgpsec = 0;

        while (true) {
            now = clock();
            zmq::message_t request;

            //  Wait for next request from client
            socket.recv (&request);

            //  Send reply back to client
            zmq::message_t reply (5);
            memcpy (reply.data (), "World", 5);
            socket.send (reply);

            msgpsec++;

            if (now >= next) {
                next = now + interval;
                std::cout << game->getSeconds() << std::endl;
                std::cout << "Messages per sec: " << msgpsec  << std::endl;
                msgpsec = 0;
            }


        }*/
    }


private:
    zmq::context_t ctx_;
    Game *game;
};