Polling System
==============

As you may have noticed, the n3rv::service superclass does not have a recv() method, 
and this is perfectly normal. n3rv data receiving mechanism is event-driven: When running,
n3rv services use ZeroMQ's polling mechanism to detect if data is available on a given socket.
If this is effectively the case, the message is dequeued, parsed and then passed to 
a user-defined receive callback.

.. code-block:: c++

  /** Main service loop, listens to open connections and forwards 
   * the data to the correct callback.
   */
   while(this->running) {
      
      zmq::pollitem_t* items = this->refresh_pollitems(); 
      try {
        zmq::poll (items,this->last_connlist.size(), this->poll_timeout);
      }
      catch(const zmq::error_t& e) {}

       for (int j=0;j < this->last_connlist.size(); j++) {
         
         if (items[j].revents & ZMQ_POLLIN) {
       
            //If ZMQ polling found data ready, we call recv() on socket
            this->connections[this->last_connlist[j]].socket->recv(&message);
            if ( this->chmap.find(this->last_connlist[j]) != this->chmap.end() ) {
              
              //If socket has a registered callback, we call it with received message as argument.
              (*this->chmap[this->last_connlist[j]])(this, &message);
            }

Receive Callbacks and attach()
------------------------------

The mapping between a qhandler* n3rv connection/binding (& by extension ZMQ Socket)
and its receive callback is made with the help of the n3rv::service attach() method.
With this mechanism, you tell your service class which funtion to call whenever a given socket
receives data.


Polling Timeout
---------------

When a node's main loop needs to poll multiple sockets at the same time, 
it will wait n3rv::service.poll_timeout milliseconds before continuing. 
poll_timeout is set to 1000ms by default, which is quite large specially 
when you want to achieve high throughput for your services. But Of course you 
can lower this value by calling service::set_timeout(int) before running the service.






