Logging Mechanism
=================

the n3rv framework comes with embedded logging features, which can be quite useful to follow the 
behaviour of your different nodes. All the logging is handled through the `n3rv::logger` class.
Each service instance or service controller has its own internal `n3rv::logger` object, which member is
`n3rv::service::ll`

service::ll member
------------------

`service::ll` is a public member, so you can acces it outside of your service class to make
settings to your internal service logger:

.. code-block:: c++
  
   /* We instanciate a new node */
   n3rv::service s1("127.0.0.1",10001);

   /* We set the loglevel for our node logger to WARNING */
   s1.ll->set_loglevel(n3rv::LOGLV_WARN);


Log() function
--------------

Whenever you want to log something, you will have to use the method `n3rv::logger::log()`:

.. code-block:: c++

   /** Adds str to log buffer and flushes it. */    
   void logger::log(int log_level, std::string str);

if log_level is lesser than or equal to the calling logger's log level, 
the log is made and str is written in destinations, else we discard it.


.. code-block:: c++
  
   n3rv::logger* ll = new n3rv::logger(n3rv::LOGLV_CRIT);

   /* this log line will not be used since LOGLV_WARN > LOGLV_CRIT */
   ll->log(n3rv::LOGLV_WARN,"blabla");

   /* On the other hand this one will go to destinations. */
   ll->log(n3rv::LOGLV_PANIC,"HAAAAAAAA");


Log Levels
----------

In n3rv the log levels are basically the same as Syslog:

.. list-table:: Available log levels
   :widths: 25 25 50
   :header-rows: 1

   * - Level
     - Name
     - n3rv Name
  
   * - 0
     - PANIC
     - LOGLV_PANIC

   * - 1
     - ALERT
     - LOGLV_ALERT
    
   * - 2
     - CRITICAL
     - LOGLV_CRIT

   * - 3
     - ERROR
     - LOGLV_ERR

   * - 4
     - WARNING
     - LOGLV_WARN

   * - 5
     - NOTICE
     - LOGLV_NOTICE

   * - 6
     - INFO
     - LOGLV_INFO

   * - 7
     - DEBUG
     - LOGLV_DEBUG

| 
| Note that when you instanciate a new `n3rv::logger` object, you must specify as argument the 
| maximum log level you want your logger to output.

Logging Destinations
--------------------

In order to add a destination for a logger object, one must use the `logger::add_dest()` method.
the logger class supports 3 different types outputs for logging (which can be combined without any issue)

stdout
******

Explicitely enough, setting this destination will display the logs on the standard output.

.. code-block:: c++
  
  n3rv::logger* ll = new n3rv::logger(LOGLV_DEBUG);
  /* tells logger to display its logs on stdout */
  ll->add_dest("stdout");

file://<path>
*************

Also quite straightforward, this tells the logger to append its logs in a file.

.. code-block:: c++

  /* tells logger to append its logs in /opt/logs/n3rv.log */
  ll->add_dest("file:///opt/logs/n3rv.log");

syslog:<ident>:<facility>
*************************

Probably the most interesting feature of the logger, you can use syslog to log your nodes activity
and thus centralize all your logs on a single remote-syslogd (got logstash ? ;-))

.. code-block:: c++

  /* tells logger to use syslog for logging, with ident "n3rv" and facility "user" */
  ll->add_dest("syslog:n3rv:user");

Note: Supported syslog facilities are local0..7, and user.

Sharing a single logger across multiple nodes / service controller
------------------------------------------------------------------

As we saw earlier, when instanciated `n3rv::service` and `n3rv::servicecontroller` automatically create
an internal logger which is accessible through their `ll` member. But they can also take
an existing logger object in their constructor and use it instead of the default one.

.. code-block:: c++

  /* We create and setup a new logger */
  n3rv::logger* log = new n3rv::logger(n3rv::LOGLV_DEBUG);
  log->add_dest("stdout");
  log->add_dest("syslog:n3rv:user");

  /* We pass this logger as argument for nodes & sc to use it. */

  n3rv::service s1("127.0.0.1",10001,log);
  n3rv::service s2("127.0.0.1",10001,log);
  n3rv::servicecontroller sc("0.0.0.0",10001,log);





