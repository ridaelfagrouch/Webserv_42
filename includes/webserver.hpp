/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sahafid <sahafid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 03:30:01 by garra             #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2023/02/10 15:54:08 by sahafid          ###   ########.fr       */
=======
/*   Updated: 2023/02/10 15:56:00 by garra            ###   ########.fr       */
>>>>>>> b928aa8 (test)
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
#include <fstream>
#include <string> 

#include "config.hpp"

#define BACKLOG 200

class Servers;
class webServer
{

private:
    std::vector<pollfd> fds;
    std::vector<Servers> _serv;
    std::vector<int> fdsclose;
    struct sockaddr_in client_address;
    socklen_t addrlen;
    std::string str_header;
    int     fds_len;
    int     server_sock;
    int     port;
    int     client_sockets;

public:
    const char *fileExemple;
    void    setupServer();
    void    acceptConnection();
    int     guard(int n, const char *er);
	void	sendall(int s, std::string response, int len);
    int     is_socket(int fd);
    void    read_all(int fd, int &read_len);
    int     Poll_in(int i);
    void    Poll_out(int i);
    void    Poll_HupErr(int &i);
    Servers  FoundServer();
    webServer(std::vector<Servers> servers);
    ~webServer();
};


