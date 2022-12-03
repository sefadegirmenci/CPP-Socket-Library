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

    int listen_port(int port)
    {
        /* Creating a socket. */
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);

        if (sockfd < 0)
        {
            perror("Socket failed\n");
            return -1;
        }

        /* A structure that contains the address of the server. */
        struct sockaddr_in serv_addr;

        /* Setting the socket address. */
        bzero((char *)&serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(port);

        /* Setting the socket options. */
        int enable = 1;
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
            perror("setsockopt(SO_REUSEADDR) failed");
        if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        {
            perror("Binding failed\n");
            return -1;
        }

        /* Listening for incoming connections. */
        if (listen(sockfd, 1) < 0)
        {
            return -1;
        }
        return sockfd;
    }

    static auto read_n(int fd, char *buffer, size_t n) -> size_t
    {
        size_t bytes_read = 0;
        size_t retries = 0;
        constexpr size_t max_retries = 10000;
        while (bytes_read < n)
        {
            auto bytes_left = n - bytes_read;
            auto bytes_read_now = recv(fd, buffer + bytes_read, bytes_left, 0);
            // negative return_val means that there are no more data (fine for non
            // blocking socket)
            if (bytes_read_now == 0)
            {
                if (retries >= max_retries)
                {
                    return bytes_read;
                }
                ++retries;
                continue;
            }
            if (bytes_read_now > 0)
            {
                bytes_read += bytes_read_now;
                retries = 0;
            }
        }
        return bytes_read;
    }
    
    auto secure_recv(int fd) -> std::pair<size_t, std::unique_ptr<char[]>>
    {
        char dlen[4];

        if (auto byte_read = read_n(fd, dlen, length_size_field);
            byte_read != length_size_field)
        {
            debug_print("[{}] Length of size field does not match got {} expected {}\n",
                        __func__, byte_read, length_size_field);
            return {0, nullptr};
        }

        auto actual_msg_size_opt = destruct_message(dlen, length_size_field);
        if (!actual_msg_size_opt)
        {
            debug_print("[{}] Could not get a size from message\n", __func__);
            return {0, nullptr};
        }

        auto actual_msg_size = *actual_msg_size_opt;
        auto buf = std::make_unique<char[]>(static_cast<size_t>(actual_msg_size) + 1);
        buf[actual_msg_size] = '\0';
        if (auto byte_read = read_n(fd, buf.get(), actual_msg_size);
            byte_read != actual_msg_size)
        {
            debug_print("[{}] Length of message is incorrect got {} expected {}\n",
                        __func__, byte_read, actual_msg_size);
            return {0, nullptr};
        }

        if (actual_msg_size == 0)
        {
            debug_print("[{}] wrong .. {} bytes\n", __func__, actual_msg_size);
        }
        return {actual_msg_size, std::move(buf)};
    }

    int send_msg(int sockfd, std::string message)
    {
        /* Converting the string into a char array and then sending it. */
        char buffer[MAXBUFFERSIZE];
        bzero(buffer, MAXBUFFERSIZE - 1);
        sprintf(buffer, "%s", message.c_str());
        return send(sockfd, buffer, sizeof(buffer), MSG_NOSIGNAL);
    }
}