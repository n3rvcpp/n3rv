FAQ
===

The following section will regroup all the questions you may ask about n3rv 
framework, being general, architecture or code related.

General
-------

Q: Who develops n3rv ?
**********************
A: My name is Clément Gamé and i am the main and only developper of n3rv framework.

Q: I did a useful code modification , can i send it ?
*****************************************************
A: Yes, feel free to send a pull request on the main n3rv code repository, on github.

Q: How can i contact the developper ?
*************************************
A: You can contact me by mail at clement.game[__at__]gmail.com, or via twitter (@WintermeW)

Architecture
------------

Q: Can i attach multiple receive callbacks to a single ZMQ socket ?
*******************************************************************
A: No, you can't. If you call attach() more than once on the same qhandler* pointer, the last attached callback will win! 
But you can break your message processing logics into multiple functions that your single callback will call conditionally.

Q: In many cases, ZeroMQ allows a single socket to connect to multiple endpoints. Can i do that with n3rv ?
***********************************************************************************************************
A: Yes you can ! n3rv::service::connect() has a qhandler* ref overloading, which means you can
use it to connect() multiple times with the same connection (& incidentally zeroMQ socket) reference.

Q: Can i use multiple service controllers inside a n3rv cluster, for redundency purpose ?
*****************************************************************************************
A: Well, yes and no..You can perfectly instanciate multiple service controllers on the same network
and subscribe some nodes on all of them, but as we speak a given node can't connect to multiple 
service controllers at the same time and use indiferently one or the other 
(by the usual means at least).


Code
----

Q: Given that a node's service_class is always named after its implemented C++ class name, why do we have to set it manually in uid ?
*************************************************************************************************************************************
A: Mainly because of C++'s lack of reflection capacities, and because:

- typeid(this) return value is dependant of the compiler's implementation (g++ for instance returns the mangled class name)
- typeid(this) does not see inheritence and returns the object's parent class.

Q: Why do n3rv receive callbacks have to be static methods ?
************************************************************
A: Again this is due to a language limitation. At compile time, the signature of a non static method 
changes (mangling, *this* pointer adding,..), this is why they can't be used as callbacks. 
The only way to go is to declare a static method and pass the calling object pointer as a void*, 
casting it back in its true type later in the callback.

Q: why do most of your object-returning functions don't return refs ?
*********************************************************************
A: Copy elision, baby ! ;)

Q: Why is the n3rv library not released as a "headers only" source, as most of the other C++ libs do nowadays ?
***************************************************************************************************************
A: Good question, i just thought that since multiple library linkings are necessary in order to use n3rv,
one more wouldn't make much difference.

