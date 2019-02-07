#include <zmq.hpp>
#include <string>
#include <iostream>
#include <unistd.h>

#include "message.pb.h"
#include <google/protobuf/text_format.h>


int main () {
    //  Prepare our context and socket
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_SUB);
    socket.connect("ipc:///tmp/feeds/0");
    std::string filter("hello");
    socket.setsockopt( ZMQ_SUBSCRIBE, filter.c_str(), filter.length());

    while (true) {
        zmq::message_t envelope;
        //  Wait for next request from client
        socket.recv (&envelope);
        std::string env_str(static_cast<char*>(envelope.data()), envelope.size());
        zmq::message_t request;
        //  Wait for next request from client
        socket.recv (&request);
        std::cout << "Received" << std::endl;
        nav::Path path;
        std::string msg_str(static_cast<char*>(request.data()), request.size());
        path.ParseFromString(msg_str);
        std::string text_str;
        google::protobuf::TextFormat::PrintToString(path, &text_str);
        std::cout << "topic: "<< env_str<< std::endl;
        std::cout << text_str << std::endl;
    }
    return 0;
}
