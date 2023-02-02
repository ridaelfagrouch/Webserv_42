/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garra <garra@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 03:30:01 by garra             #+#    #+#             */
/*   Updated: 2023/02/02 18:59:29 by garra            ###   ########.fr       */
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
    std::vector<int> client_sockets;
    std::vector<pollfd>  fds;
    std::vector<Servers> servers;
    int     client_fd;
 
public:
    void    setupServer();
    void    acceptConnection();
    int     guard(int n, const char *er);
	int		sendall(int s, const char *buf, int len);
    void    hooks(int *i);
    webServer(std::vector<Servers> servers);
    ~webServer();
};

