#include <zmq.hpp>
#include "message.pb.h"
#include <string>
#include <iostream>
#include <unistd.h>

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
    zmq::socket_t socket (context, ZMQ_PAIR);

    std::cout << "Connecting to server" << std::endl;
    socket.connect ("tcp://localhost:5555");

    while(true){
        std::string msg_str;
        path.SerializeToString(&msg_str);
        // create a zmq message from the serialized string
        zmq::message_t request (msg_str.size());
        memcpy ((void *) request.data (), msg_str.c_str(), msg_str.size());
        //std::cout << "Sending Person data ..." << std::endl;
        socket.send (request);
        usleep(10000);
    }
    // Optional:  Delete all global objects allocated by libprotobuf.
    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}
