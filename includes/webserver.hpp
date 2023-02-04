/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garra <garra@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 03:30:01 by garra             #+#    #+#             */
/*   Updated: 2023/02/04 18:38:35 by garra            ###   ########.fr       */
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
# include <bits/stdc++.h>
# include <arpa/inet.h>

#include "config.hpp"

#define MAX_CONNECTIONS 100
#define TIMEOUT -1

class Servers;


class webServer
{

private:
    struct pollfd fds[200];
    std::vector<int> socket_list;
    int fds_len;
    std::vector<Servers> _serv;
    socklen_t addrlen;
    struct sockaddr_in remote;
    int     client_fd;
    int     m_socket;
    int     client_sockets;
    std::string str_header;

 
public:
    void    setupServer();
    void    acceptConnection();
    int     guard(int n, const char *er);
	int		sendall(int s, const char *buf, int len);
    int     is_socket(int fd);
    webServer(std::vector<Servers> servers);
    ~webServer();
};


