/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   websocket.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garra <garra@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 04:00:17 by garra             #+#    #+#             */
/*   Updated: 2023/01/25 02:06:46 by garra            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "websocket.hpp"

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
    guard(listen(server_fd, 10), "listen error");
}

//--------------------------------------------------------------------------

void    webSocket::acceptConnection(void)
{
    char hello[] = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
    client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    if (client_fd == -1) 
    {
        if (errno == EWOULDBLOCK)
            std::cout << "No pending connections" << std::endl;
        else
        {
            perror("error when accepting connection");
            exit(1);
        }
        sleep(1);
    } 
    else
    {
        char buffer[30000] = {0};
        read(client_fd , buffer, 30000);
        std::cout << buffer << std::endl;
        write(client_fd , hello , strlen(hello));
        std::cout << "------message sent-------" << std::endl;
        close(client_fd);
    }
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