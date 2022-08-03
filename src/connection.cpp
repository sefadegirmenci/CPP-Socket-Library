#include "string"
#include "connection.h"
#define MAXBUFFERSIZE 4098 /* Defining the maximum size of the buffer that will be used to read the socket. */
extern "C"
{

    int connect_socket(const char *hostname, const int port)
    {
        struct sockaddr_in serv_addr;
        struct hostent *server = gethostbyname(hostname);
        if (server == NULL)
        {
            perror("No such host");
            return -1;
        }

        /* Creating a socket and then checking if it was created successfully. */
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0)
        {
            perror("ERROR opening socket in connect_socket\n");
            return -1;
        }
        /* Setting the socket address. */
        bzero((char *)&serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
        serv_addr.sin_port = htons(port);
        /* Connecting the socket to the server. */
        if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        {
            perror("ERROR connecting in connect_socket\n");
            return -1;
        }

        return sockfd;
    }

    int accept_connection(int sockfd)
    {
        struct sockaddr_in cli_addr;
        socklen_t clilen = sizeof(cli_addr);
        int newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        if (newsockfd < 0)
        {
            return -1;
        }
        return newsockfd;
    }
}