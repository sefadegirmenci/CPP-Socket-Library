#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * It takes a hostname and a port number, and returns a socket file descriptor that is connected to
     * the hostname on the given port
     *
     * @param hostname the name of the host you want to connect to
     * @param port the port number to connect to
     *
     * @return The socket file descriptor.
     */
    int connect_socket(const char *hostname, const int port);
    /**
     * @brief Accept a connection on the specified sockfd.
     * @return sockfd of new connection when successful, -1 on failure
     */
    int accept_connection(int sockfd);

    /**
     * @brief Open a SOCK_STREAM socket listening on the specified port.
     * @return sockfd if successful, -1 on failure
     */
    int listen_port(int port);

    /**
     * @brief Read one message from sockfd and pass it to the parameter.
     *        Make sure that the whole message is read. (TODO: check this)
     * @return 0 on success, -1 on failure
     */
    int recv_msg(int sockfd, std::string* message);
}
