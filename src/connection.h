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
     * @brief Read a line from the specified socket.
     *
     * @param fd The socket file descriptor to read from.
     *
     * @return Number of bytes read
     */ 
    static auto read_n(int fd, char *buffer, size_t n) -> size_t;

    /**
     * @brief It reads the length of the message, then reads the message itself
     *
     * @param fd The file descriptor to read from
     *
     * @return A pair of size_t and unique_ptr<char[]>
     */
    auto secure_recv(int fd) -> std::pair<size_t, std::unique_ptr<char[]>>;

    /**
     * @brief Send one message to sockfd given the parameters.
     *       
     * @return 0 on success, -1 on failure
     */
    int send_msg(int sockfd, std::string message);
}
