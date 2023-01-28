/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   websocket.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garra <garra@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 03:30:01 by garra             #+#    #+#             */
/*   Updated: 2023/01/28 11:02:53 by garra            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <stdio.h>
# include <sys/socket.h>
# include <unistd.h>
# include <stdlib.h>
# include <netinet/in.h>
# include <string.h>
# include <fcntl.h>
# include <poll.h>
# include <errno.h>
# include <iostream>
# include <exception>

#define PORT 8080
#define MAX_CONNECTIONS 100
#define TIMEOUT -1

class webSocket
{
private:
    struct sockaddr_in  address;
    int                 addrlen;
 
public:
    int     server_fd;
    int     client_fd;
    int     flag;
    struct pollfd fds[MAX_CONNECTIONS];
    void    setupServer();
    void    CreateSocket();
    void    bindSocket();
    void    listenSocket();
    void    acceptConnection();
    void    read_request(int client_socket);
    int     guard(int n, const char *er);
    webSocket();
    ~webSocket();
};

