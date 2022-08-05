<p align="center">
<img src="https://api.codiga.io/project/34314/status/svg">
</img>
</p>

<h1 align="center"> CPP-Socket-Library </h1>

## Description 
The easiest way of creating TCP sockets and implementing message transfers. Through this library you can create your own servers and clients. Bear in mind that this project is not **product-safe**. It is only for experimenting socket programming, and having a good understanding of the underlying structure of the networks. 


## Usage
1. Download the repo.
2. Copy the src folder to your workspace
3. Add `#include "src/connection.h"` to the file you want to use the sockets.
4. Use the methods described below 

*One of the aims of this project is to make it a library, however, it is not at that stage yet.*

### Methods

`connect_socket(char *hostname, int port):`
Returns a socket file descriptor that is connected to the hostname on the given port.

`accept_connection(int sockfd):` 
Accepts a connection on the specified socket.

`listen_socket(int port):`
Returns a socket file descriptor that is listening on the specified port.

`recv_msg(int sockfd, std::string* message):`
Receives a message from the socket, assigns it to the message parameter for further processing.

### TO-DO: 
- [ ] Turn the package into a library
- [ ] Send message over the tcp socket
- [X] Receive message over the tcp socket
- [ ] Protobuf support
- [ ] Async IO - nonblocking accept connection and message transfer
- [ ] Concurrency support
