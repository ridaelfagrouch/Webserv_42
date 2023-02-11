/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rel-fagr <rel-fagr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 03:30:01 by garra             #+#    #+#             */
/*   Updated: 2023/02/10 14:11:50 by rel-fagr         ###   ########.fr       */
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

#define BACKLOG 300

class Servers;
class webServer
{

private:
    std::vector<pollfd> fds;
    std::vector<Servers> _serv;
    std::vector<int> fdsclose;
    int fds_len;
    socklen_t addrlen;
    struct sockaddr_in client_address;
    // int     client_fd;
    int     server_sock;
    int     port;
    int     client_sockets;
    std::string str_header;

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
    webServer(std::vector<Servers> servers);
    ~webServer();
};


