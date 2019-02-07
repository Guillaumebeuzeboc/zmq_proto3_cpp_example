#include <zmq.hpp>
#include "message.pb.h"
#include <string>
#include <iostream>
#include <unistd.h>

void send_wrapped_msg(zmq::socket_t& socket, const std::string& topic, const std::string& message){
    zmq::message_t envelope (topic.size());
    memcpy ((void *) envelope.data (), topic.c_str(), topic.size());
    socket.send (envelope, ZMQ_SNDMORE);
    zmq::message_t request (message.size());
    memcpy ((void *) request.data (), message.c_str(), message.size());
    socket.send (request);

}
int main ()
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    nav::Path path; //fill a person data
    path.set_id(1234);
    path.set_name("myPath");
    nav::Path::Point* points = path.add_point();
    points->set_x(0.1);
    points->set_y(0.2);
    points->set_z(0.3);
    points->set_type(nav::Path::POSITION);
    points = path.add_point();
    points->set_x(0.4);
    points->set_y(0.5);
    points->set_z(0.6);
    points->set_type(nav::Path::VELOCITY);

    //  Prepare our context and socket
    zmq::context_t context (1);
    // Note we use here a PAIR socket, only 1 way message
    zmq::socket_t socket (context, ZMQ_PUB);

    std::cout << "Connecting to server" << std::endl;
    socket.bind ("ipc:///tmp/feeds/0");

    while(true){
        std::string msg_str;
        path.SerializeToString(&msg_str);
        // create a zmq message from the serialized string
        send_wrapped_msg(socket, "hella", msg_str);
        usleep(10000);
        send_wrapped_msg(socket, "hello", msg_str);
    }
    // Optional:  Delete all global objects allocated by libprotobuf.
    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}
