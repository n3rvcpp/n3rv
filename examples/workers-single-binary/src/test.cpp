#include <zmq.hpp>

int main() {


  zmq::context_t ctx(1);
  zmq::socket_t sock1(ctx,ZMQ_REQ);
  zmq::socket_t sock2(ctx,ZMQ_PUB);

  sock1.bind("tcp://*:12001");
  sock2.bind("tcp://*:12002");

  //zmq::pollitem_t* items  = (zmq::pollitem_t*) malloc(2 * sizeof(zmq::pollitem_t) );

  zmq::pollitem_t items [] = {
        { &sock1, NULL, ZMQ_POLLIN, 0 },
        { &sock2, NULL, ZMQ_POLLIN, 0 }
    };

  while(1) {
    //Throws error !! socket operation on non-socket.
    zmq::poll (&items [0],2, 500);
  }

}