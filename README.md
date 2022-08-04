# CPP-Socket-Library
TCP socket library for network(socket) programming in C++

### Methods


`connect_socket(char *hostname, int port):`
Returns a socket file descriptor that is connected to the hostname on the given port.

`accept_connection(int sockfd):` 
Accepts a connection on the specified socket.

`listen_socket(int port):`
Returns a socket file descriptor that is listening on the specified port.

### TO-DO: 
- [ ] Send message over the tcp socket
- [ ] Receive message over the tcp socket
- [ ] Protobuf support
- [ ] Async IO - nonblocking accept connection and message transfer
- [ ] Concurrency support
