Logging Mechanism
=================

the n3rv framework comes with embedded logging features, which can be quite useful to follow the 
behaviour of your different nodes. All the logging is handled through the `n3rv::logger` class.
Each service instance or service controller has its own internal `n3rv::logger` object, which member is
`n3rv::service::ll`


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