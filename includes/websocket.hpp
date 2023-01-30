/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   websocket.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garra <garra@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 03:30:01 by garra             #+#    #+#             */
/*   Updated: 2023/01/30 03:31:19 by garra            ###   ########.fr       */
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
# include <vector>
# include<bits/stdc++.h>

#define PORT 8080
#define MAX_CONNECTIONS 100
#define TIMEOUT -1

class webSocket
{
private:
    struct sockaddr_in  address;
    struct pollfd fds[MAX_CONNECTIONS];
    std::vector<int> client_sockets;
    int     addrlen;
    int     server_fd;
    int     client_fd;
    int     flag;
 
public:
    void    setupServer();
    void    CreateSocket();
    void    bindSocket();
    void    listenSocket();
    void    acceptConnection();
    void    read_request(int client_socket);
    int     guard(int n, const char *er);
	int		sendall(int s, const char *buf, int len);
    void    hooks(int *i);
    webSocket();
    ~webSocket();
};

