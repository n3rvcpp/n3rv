Service Main Loop
=================

When calling `service::run()` on a service instance, a loop starts (which mainly consists in 
polling the open ZMQ sockets as we saw earlier). 
The loop keeps running until a call to `service::stop()` is made for the instanciated service.

Asynchronous Run
----------------

As you probably have figured out, `service::run()` is blocking. Therefore `n3rv::service` 
has another running method called `run_async()`, which starts the running loop inside a 
dedicated thread.

Tempering with the main loop
----------------------------

There will be many situations where you will want to add extra code in a service's running loop,
and for that n3rv provides 2 distinct mechanisms:

service::hkloop()
*****************

Probably the simplest method, `n3rv::service` defines the `hkloop()` virtual method, that is called for 
every iteration of the main loop. All you have to do then is to override `hkloop()` in your 
service-inherited class definition and put inside the code you want to be executed in the main loop.

.. code-block:: c++

  class example: public n3rv::service {
      using n3rv::service::service;

      public:
        /* We override the hkloop() virtual method.*/
        void hkloop() {

            /* Put HERE the code you want in 
               running loop*/
        }
  }
        
service::register_main()
************************

A more elaborate way to insert code in the main loop consists in using 
the `service::register_main()` method.

.. code-block:: c++

  class example: public n3rv::service {
      using n3rv::service::service;

      public:

        /* We declare a new loop callback */
        static void* loop_callback(void* objref) {

            example* self = static_cast<example*>(objref);

            /* Put HERE the code you want in 
               running loop*/
        }

        int initialize() {

            //We register the callback to be run inside main loop.
            this->register_main("00_loop_callback",example::loop_callback);
        }
  }

With this method, all the registered callbacks will be executed sequencially at
every iteration of the main loop. The execution order is determined by the 
callbabck identifier (`cbid`), so if this matters to you i suggest that you use 
callback identifiers like `00_name`, `01_name`,..

The main advantage of this method is that you can dynamically unregister a callback at any time
with the help of `service::unregister_main(const char* cbid)`, or register new ones.

A matter of choice
******************

Both machanisms are usable together. `hkloop()` gets executed first, then the registered loop
callbacks in the order you chose. Nevertheless to avoid confusion and errors, i suggest that you
stick to only one of these, following your needs.

Interaction with poll_timeout
-----------------------------

You may discover that your main loop is not running as fast as you'd want: it may be
because of the the value of poll_timeout. if this is the case you might consider lowering it.

Don't mess with the Loop
------------------------

Now a final warning: If you want to save yourself a lot of troubles and repeated headaches, 
don't put blocking instructions in your main loop's code. That would potentially result in 
horrible perf loss and unexpected behaviours.

.. code-block:: c++

  class example: public n3rv::service {
      using n3rv::service::service;

      public:
        /* We override the hkloop() virtual method.*/
        void hkloop() {
            
            //You're doing it wrong !
            while(1) {}

        }
  }












