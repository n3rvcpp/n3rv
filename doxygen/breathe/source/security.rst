A word on Security
==================

Besides the usual coding best-practices, the n3rv framework **DOES NOT** implement any security
mechanism for nodes authentication and data exchange, and probably never will. 
Therefore the use of multiple hosts inside a n3rv cluster necessarly implies **A TRUSTED NETWORK (LAN/VPN)**.

Knowing this, if you plan to use multiple n3rv nodes over the Internet, i HIGHLY RECOMMAND that you use it on 
top of L2TP/IPSEC, OpenVPN, SSH Tunnels or similar solution.

Also you can feel free to implement your own security mechanisms on top of n3rv, no problem.

PSK-based subscribing + TLS
---------------------------

As an example of security layer for your n3rv cluster, you could modify the subscribe() method
of the service controller in order to let your subscribing nodes provide a Pre-Shared Key. 
Nodes with no/incorrect PSK would not be registered and therefore would never be part of the cluster.

Additionally and to avoid node-spoofing by an attacker, you could request the PSK for every
exchange between your nodes, in the n3rv message payload.

Also in order to ensure the security of data exchanges between your nodes, you can add a 
TLS layer (with libreSSL / OpenSSL) that would encrypt the n3rv payloads before sending 
them over the net (encrypt right before zmq::send/ decypt right after zmq::recv).
I sugget you have a look at service::send() and service::run() inside the service.cpp source file
of the library. Just keep in mind that adding a crypto layer to your services may result in 
a degradation of the performance (data throughput).



