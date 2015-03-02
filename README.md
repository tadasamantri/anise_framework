# ANISE Framework: Anomaly Network IntruSion Environment #

ANISE is a C++ framework using the QT libraries for developing reusable, memory efficient and multi-threaded
capable algorithms without the need to worry much about memory management and multi-threading.

## ANISE Overview ##

### Data Management ###

The framework provides an environment for developing algorithms as a series of interconnected nodes. Nodes are
able to receive data from other nodes, process all the received data, and forward the processing results to
other nodes. Each node is only responsible for managing its own memory; the data sent to other nodes is
efficiently managed by the framework. The data is distributed using the CoW (Copy-on-Write) paradigm: The
forwarded data is able to be read without incurring in any memory operations. The data, however, is copied
whenever it needs to be modified.

### Automatic Multi-Threading ###

Every node runs in parallel, whenever it is possible, using as many kernel threads as the local system
provides. Nodes start processing data as soon as it is made available and can forward new data to other nodes.

### Algorithm Reusability ###

Algorithms are able to be logically split into nodes that process incoming data and forward data to other
nodes. A collection of nodes and how they are connected is known as a mesh. Nodes can be reused in multiple
meshes and contexts. Each node receives and forwards a particular (and predefined) type of data. As long as
the nodes receive the type of data they expect, they can be used in different places. This enables the
creation of reusable components that can be shared among different algorithms.

## ANISE Components ##

![ANISE Components](http://undeadkernel.github.io/anise_images/components.png)

### The Gate ###

Nodes in the ANISE framework communicate by receiving and forwarding data among each other. Gates are the
means by which nodes connect to each other. A gate *belongs to only one node* and specifies the *type of data*
the gate can handle. Gates can only connect to other gates of the same type.

### The Node ###

Nodes are the core of the ANISE framework. Each node is a logical unit capable of receiving, processing and
forwarding data to other nodes. Nodes
