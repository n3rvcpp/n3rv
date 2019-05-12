n3rv, a ZeroMQ service framework
================================

![N3RV LOGO](/doxygen/breathe/source/_static/img/logo_large.png?raw=true "n3rv logo")

n3rv is a C++ library that leverages the power of ZeroMQ to create multiservices architectures.
Using ZMQ as core communication mechanism, n3rv basically provides all the components that 
modern services development requires, meaning:

* A Services Directory & subscribing mechanism.
* An events handler.
* Messages encapsulation methods.
* Logging.
* An HTTP/ZeroMQ gateway.
* Dynamic architecture reconfiguration.

Actually it is quite similar to [Facebook's fbzmq](https://github.com/facebook/fbzmq), but easier to use and with more features.

Full Documentation available at https://n3rvcpp.github.io


Architecture example
--------------------

![N3RV ARCH](/resources/images/arch.png?raw=true "n3rv arch")


How does it work ?
------------------

Running nodes create ZMQ socket bindings, then register them to a directory service 
(service controller). Each update of the directory triggers a broadcast-update on all the 
nodes, thus making the bindings joinable by everyone. 

![N3RV ARCH](/resources/images/working.png?raw=true "n3rv arch")


From the developer's perspective, n3rv lets you implement custom service classes, inheriting from 
the `n3rv::service` superclass. In these custom classes you will describe the complete behaviour 
of your services:

* Which sockets they must bind.
* Which nodes they must connect to.
* Backend interractions.
* Business rules, ..

After that, you just need to instanciate your defined service classes to let the magics begin !

![N3RV ARCH](/doxygen/breathe/source/_static/img/algorun.png?raw=true "n3rv arch")

(Running example of a proto-algotrading solution using a n3rv multinodal architecture)

Documentation
-------------

You can get the full n3rv framework documentation at the following address: 
https://n3rvcpp.github.io


Changelog
---------

### v0.2:

* Added support for protcol buffers messages exchange.
* Added support for syslog in logger.
* New lookup mechanism, allowing to deal with lots of subscribed nodes and bindings.
* Additionnal work on unit tests and code coverage
* Documentation largely extended
* Many bugfixes

### v0.15:

* Implementation of httpservice (HTTP<=>ZMQ gateway)
* started unit tests implementation.

### v0.1:

* Initial version.


TOTO
----

* Implement redundency mechanisms for the service controller component.
* Better support of NATed networks for service controller.
* Add FastCGI and easy file serving support for the HTTP Gateway.
* Improve unit tests code coverage.
* WYSIWYG edtior to generate topologies and class templates (separate project).
* Webmin interface for service controller.
* Possible replacement of ZeroMQ by Nanomsg for services I/O.
* Python binding (maybe)

Contact
-------

@WintermeW on twitter, or by email at clement.game@gmail.com
