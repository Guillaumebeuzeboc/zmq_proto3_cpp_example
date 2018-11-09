#include <zmq.hpp>
#include <string>
#include <iostream>
#include <unistd.h>

#include "message.pb.h"
#include <google/protobuf/text_format.h>


int main () {
    //  Prepare our context and socket
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_PAIR);
    socket.bind ("tcp://*:5555");

    while (true) {
        zmq::message_t request;
        //  Wait for next request from client
        socket.recv (&request);
        std::cout << "Received" << std::endl;
        nav::Path path;
        std::string msg_str(static_cast<char*>(request.data()), request.size());
        path.ParseFromString(msg_str);
        std::string text_str;
        google::protobuf::TextFormat::PrintToString(path, &text_str);
        std::cout << text_str << std::endl;
    }
    return 0;
}
