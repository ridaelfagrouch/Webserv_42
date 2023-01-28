/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   websocket.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garra <garra@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 04:00:17 by garra             #+#    #+#             */
/*   Updated: 2023/01/28 12:11:49 by garra            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "websocket.hpp"


void    webSocket::setupServer()
{
    CreateSocket();
    bindSocket();
    listenSocket();
}

//--------------------------------------------------------------------------

void    webSocket::CreateSocket(void)
{
    server_fd = this->guard(socket(AF_INET, SOCK_STREAM, 0), "create TCP socket error");
    flag = this->guard(fcntl(server_fd, F_GETFL), "could not get flags on TCP socket");
    this->guard(fcntl(server_fd, F_SETFL, flag | O_NONBLOCK), "could not set TCP socket to be non-blocking");
}

//--------------------------------------------------------------------------

void    webSocket::bindSocket(void)
{
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(PORT);
    memset(address.sin_zero, '\0', sizeof(address.sin_zero));
    guard(bind(server_fd, (struct sockaddr *)&address, sizeof(address)), "bind error");
}

//--------------------------------------------------------------------------

void    webSocket::listenSocket(void)
{
    guard(listen(server_fd, MAX_CONNECTIONS), "listen error");
    fds[0].fd = server_fd;
    fds[0].events = POLLIN;
    for (int i = 1; i < MAX_CONNECTIONS; i++)
        fds[i].fd = -1;
}

//--------------------------------------------------------------------------

void    webSocket::acceptConnection(void)
{
    while (true) {
        // Use poll() to monitor file descriptors for activity
        int nfds = poll(fds, MAX_CONNECTIONS, TIMEOUT);

        if (nfds == -1) {
            std::cerr << "Error in poll()" << std::endl;
            continue;
        }

        // Check if the server socket has activity
        if (fds[0].revents & POLLIN) {
            // Accept a new connection
            int client_socket = accept(server_fd, NULL, NULL);
            client_sockets.push_back(client_socket);

            // Add the new client socket to the pollfd array
            for (int i = 1; i < MAX_CONNECTIONS; i++) {
                if (fds[i].fd == -1) {
                    fds[i].fd = client_socket;
                    fds[i].events = POLLIN;
                    break;
                }
            }
        }

        // Check for activity on the client sockets
        for (int i = 1; i < MAX_CONNECTIONS; i++) {
            if (fds[i].fd == -1) {
                continue;
            }

            if (fds[i].revents & POLLIN) {
                // Read data from the client socket
                char buffer[1024];
                int bytes_received = recv(fds[i].fd, buffer, sizeof(buffer), 0);

                if (bytes_received <= 0) {
                    // Connection closed by client
                    close(fds[i].fd);
                    fds[i].fd = -1;

                    // Remove the client
                      } else {
                    // Process the data received from the client
                    std::string request(buffer, bytes_received);
                    std::cout << "Received request: " << request << std::endl;

                    // Send a response to the client
                    std::string response = "HTTP/1.1 200 OK\r\n\r\nHello, World!";
                    send(fds[i].fd, response.c_str(), response.length(), 0);
                }
            }
        }

        // Remove closed client sockets from the client_sockets vector
        std::vector<int>::iterator it = client_sockets.begin();
        while (it != client_sockets.end()) {
            if (std::find(fds, fds + MAX_CONNECTIONS, *it) == -1) {
                it = client_sockets.erase(it);
            } else {
                ++it;
            }
        }
    }

    close(server_fd);
}

//--------------------------------------------------------------------------

void webSocket::read_request(int client_socket)
{
    char buffer[30000] = {0};
    read(client_socket, buffer, 1024);
    std::cout << "Request: " << buffer << std::endl;
    close(client_fd);
}

//--------------------------------------------------------------------------

int webSocket::guard(int n, const char *er)
{
    if (n < 0)
    {
        perror(er);
        exit(EXIT_FAILURE);
    }
    return n;
}

//--------------------------------------------------------------------------

webSocket::webSocket()
{
    addrlen = sizeof(address);
}

webSocket::~webSocket(){}

//--------------------------------------------------------------------------

int sendall(int s, char *buf, int *len)
{
    int total = 0; // how many bytes we’ve sent
    int bytesleft = *len; // how many we have left to send
    int n;
    while(total < *len) 
    {
        n = send(s, buf+total, bytesleft, 0);
        if (n == -1)
            break;
        total += n;
        bytesleft -= n;
    }
    *len = total; // return number actually sent here
    return n==-1?-1:0; // return -1 on failure, 0 on success
}


