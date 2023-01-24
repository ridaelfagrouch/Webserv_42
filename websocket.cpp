/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   websocket.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garra <garra@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 04:00:17 by garra             #+#    #+#             */
/*   Updated: 2023/01/24 04:01:45 by garra            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "websocket.hpp"

void    webSocket::CreateSocket(void)
{
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        perror("socket error");
        exit(EXIT_FAILURE);
    }
}

//--------------------------------------------------------------------------

void    webSocket::bindSocket(void)
{
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    memset(address.sin_zero, '\0', sizeof(address.sin_zero));
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("bind error");
        exit(EXIT_FAILURE);
    }
}

//--------------------------------------------------------------------------

void    webSocket::listenSocket(void)
{
    if (listen(server_fd, 10) < 0)
    {
        perror("listen error");
        exit(EXIT_FAILURE);
    }
}

//--------------------------------------------------------------------------

void    webSocket::acceptConnection(void)
{
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    if (new_socket<0)
    {
        perror("accept error");
        exit(EXIT_FAILURE);
    }
}

//--------------------------------------------------------------------------

webSocket::webSocket()
{
    addrlen = sizeof(address);
}

webSocket::~webSocket()
{
}