Messages
========

Message structure
-----------------

To exchange data betweeb its nodes, n3rv uses a specific message strucure 
which has the following format:

.. code-block:: c++

  /** Main n3rv message struct */
  typedef struct message_ {

    /** message sender (automatically filled) */
    std::string sender;
    std::string action;
    std::vector<std::string> args;
    std::string payload;

   } message;


This structure was choosen because it is in my opinion able to handle a wide variety of use cases.
Nevertheless if you need extra fields for your own usage, it is easilly modifiable in both 
`src/n3rvproto.hpp` and `src/protocol/(json|protobuf)`

Serialization / Deserialization
-------------------------------

As you may be already aware of, n3rv is able to encapsulate its messages either in JSON or Google Protocol Buffer
format. To be able to support both of these serialization methods, the n3rv messages handler has an
abstraction layer which is described in `src/n3rvproto.hpp`

.. code-block:: c++

  std::string serialize_msg(n3rv::message& msg);
   
  n3rv::message parse_msg(std::string msgstr);

Raw data (byte sequences) Exchange between nodes
------------------------------------------------

You may have seen that the type for the payload member of a n3rv message is *std::string*.
Although not very recommanded to avoid confusion, *std::string* makes a suitable container 
for byte arrays (remember, c++ strings are not null-terminated). Nevertheless, if you don't 
feel comfy with this statement, then feel free to change the n3rv::message.payload type to 
eg std::vector<std::byte> or whatever feels appropriate for you.