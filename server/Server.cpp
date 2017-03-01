//
// Created by Per-Arne on 01.03.2017.
//

#include <zmq.hpp>
#include <string>
#include <iostream>
#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>

#define sleep(n)    Sleep(n)
#endif
#include "Server.h"
#include "../game/Game.h"

#include <google/protobuf/text_format.h>
#include "../game/proto/TestMessage.pb.h"

Server::Server(Game *game){
    std::cout << "Started!" << std::endl;

    //  Prepare our context and socket
    zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_PAIR);
    socket.bind ("tcp://*:5555");

    int interval = 1000;
    clock_t now = clock();
    clock_t next = now + interval;
    int msgpsec = 0;

    while (true) {
        now = clock();

        zmq::message_t request;
        socket.recv (&request);

        TestMessage msg;
        std::string msg_str(static_cast<char*>(request.data()), request.size());
        msg.ParseFromString(msg_str);
        msgpsec++;

        if (now >= next) {
            next = now + interval;
            std::cout << "Messages per sec: " << msgpsec << std::endl;
            msgpsec = 0;
        }



    }



}