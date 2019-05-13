Messages Customization
======================

In this tutorial we will see how to extend the n3rv message structure to fit your needs.
As a reminder, the default n3rv message struct is the following:

.. code-block:: c++

  /** Main n3rv message struct */
  typedef struct message_ {

    /** message sender (automatically filled) */
    std::string sender;
    std::string action;
    std::vector<std::string> args;
    std::string payload;

   } message;

1. Modify src/n3rvproto.hpp
---------------------------

Let's say we want to add a new field to the structure above, `int priority;`
Our first step will then be to edit `src/n3rvproto.hpp` to add the field in our message struct:

.. code-block:: c++

  /** Main n3rv message struct */
  typedef struct message_ {

    std::string sender;
    std::string action;
    std::vector<std::string> args;
    std::string payload;

    /* We add our extra field here */
    int priority;

   } message;


2. For JSON (default)
---------------------

JSON is the default serialization method in n3rv, so if you did not chose to use protobuf, this is
the section you want to read.

So after having edited our message struct, we need to modify our serialization / deserialization methods
to take this new field into account. To do this, we must modify `src/protocol/json/protocol.cpp`

2.1 n3rv::parse_msg()
*********************

First in the parse_msg method, we must add the priority field parsing:

.. code-block:: c++

    //..
    //msg_.action = d["action"].GetString();
    //msg_.payload = d["payload"].GetString();

    /* We add parsing for our new field */
    msg_.priority = d["priority"].GetInt();


2.2 n3rv::serialize_msg()
*************************

Then we repeat the operation but for the serializing method:

.. code-block:: c++

    //..
    //writer.String("payload");
    //writer.String(msg.payload.c_str());

    /* We add our field for serializing */
    writer.String("prioriy");
    writer.Int(msg.priority);
    
    //writer.EndObject();

2b. For Protobuf
------------

If you chose protobuf as serializing method, here is what you have to do.

2b.1 Update `src/protocol/n3rvproto.proto`
******************************************

In the `src/protocol/n3rvproto.proto` file, you will add our extra field in the n3rvmessage object def:

.. code-block:: console

   message n3rvmessage {
    required string sender = 1;
    required string action = 2;
    repeated string args = 3;
    required string payload = 4;
    
    required int priority = 5;

}
   
2b.2 recompile protobuf file
****************************

Now we are ready to recreate the protobuf source files:

.. code-block:: console
    pushd .
    cd src/protocol/protobuf
    protoc n3rvproto.proto --cpp-out=.
    popd

2b.3 n3rv::parse_msg()
**********************

Then we need to modify `src/protocol/protobuf/protocol.cpp` to take our new field into account
when deserializing:

.. code-block:: c++

   //..
   //msg_.action = msg__.action();
   //msg_.payload = msg__.payload();
   msg_.priority = msg__.priority();


2b.4 n3rv::serialize_msg()
**************************

Finally we need to modify serialize_msg() in the same file to take our new field into account
when serializing:

.. code-block:: c++

    //..
    //msg__.set_action(msg.action);
    //msg__.set_payload(msg.payload);
    msg__.set_priority(msg.priority);


3. Rebuild
----------

Now that we made all the required modifications, all we must do is to rebuild the library:

.. code-block:: console

   cmake [-DWITH_PROTOBUF=on] .
   make && sudo make install

That is all, you are now ready to use the n3rv::message::priority field inside your custom service classes !




