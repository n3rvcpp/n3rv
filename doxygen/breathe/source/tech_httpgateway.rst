HTTP Gateway
============

n3rv framework provides a lightweight http server based on libevent, which allows your n3rv cluster
to communicate with the outside world over HTTP (the client part is at your discretion though)

httpservice class
-----------------

`n3rv::httpservice` is an extension of the `n3rv::service` superclass, which means it has 
the exact same features but with extra http service capacities.

Initializing HTTP Service
*************************

the `httpservice` class has 2 methods that you need to call to enable http service: `init_http()`
and then `run_http_async()`:

.. code-block:: c++

    /* We create a new httpservice instance */
    n3rv::httpservice hs1("127.0.0.1",10001);

    /* We initialize and bind the http service on port 80 */
    hs1.init_http("0.0.0.0","80");

    /* We start HTTP service */
    hs1.run_http_async();


Attach URI to Callback 
**********************

`n3rv::httpservice` uses libevent as underlying mechanism for http serving. libevent's 
inner mechanism requires binding an HTTP URI with a callback function that will produce 
the desired actions and return the appropriate content to the client for that URI.
This is precisely what `httpservice::attach_http()` does:

.. code-block:: c++

      /** We attach to the default httpservice callback, which responds
        "Hello World" when URI is requested */
      hs1.attach_http("/foo",n3rv::httpservice::http_callback);

Signature for HTTP callbacks is the following:

.. code-block:: c++

  static void http_callback(evhttp_request *req, void * objref);

`req` is the data representation of the client's http request, `objref` is the caller backref


Callback Structure
******************

Like Receive callbacks, HTTP callbacks must have a minimum of processing logic in order to be useful.

A typical HTTP callback implementation is as follows:

.. code-block:: c++

  void httpservice::http_callback(evhttp_request *req, void * objref) {

        /** we get back httpservice object */
        httpservice* self = (httpservice*) objref;

        /* we create an output buffer for response */
        auto *out_buff = evhttp_request_get_output_buffer(req);
        if (!out_buff) return;

        self->ll->log(LOGLV_DEBUG, "sending back http reply");

        /* We fill the buffer */
        evbuffer_add_printf(out_buff, "<html><body>Hello World!</body></html>");

        /* We send the response with the buffer's content */
        evhttp_send_reply(req, HTTP_OK, "", out_buff);
    }


Serve static files/directories from Filesystem
**********************************************

the `httpservice` class offers the possibility to serve static files coming from your local filesystem. To do this,
you have to 


Extra linking
-------------

 Important: Given that `n3rv::httpservice` uses libevent, whenever you use this class you need to link your generated binary with `-levent`
