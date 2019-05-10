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

.. code-block:: c++

    /* We create a new httpservice instance */
    n3rv::httpservice hs1("127.0.0.1",10001);

    /* We initialize and bind the http service on port 80 */
    hs1.init_http("0.0.0.0","80");

    /* We start HTTP service */
    hs1.run_http_async();


Distribute content: Attach URI to Callback 
******************************************

.. code-block:: c++

      hs1.attach_http("/foo",n3rv::httpservice::http_callback);


Extra linking
-------------







