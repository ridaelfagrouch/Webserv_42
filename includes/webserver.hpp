/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rel-fagr <rel-fagr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 03:30:01 by garra             #+#    #+#             */
/*   Updated: 2023/02/14 15:37:55 by rel-fagr         ###   ########.fr       */
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
# include <fstream>
# include <string> 

# include "config.hpp"

#define BACKLOG 200
class Servers;


class fds_info
{
    public:
        std::string str_header;
        bool        is_first_time;
        int         content_length;
        bool        is_complet;
        int         read_len;
        int         total;
        pollfd      tmp;
};



class webServer
{

private:
    std::vector<pollfd> fds;
    std::vector<Servers> _serv;
    std::vector<fds_info> fdsInfo;
    std::vector<int> fdsclose;
    struct sockaddr_in client_address;
    socklen_t addrlen;
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
    void    read_header(int i);
    int     Poll_in(int &i);
    void    Poll_out(int i);
    void    Poll_HupErr(int &i);
    Servers  FoundServer();
    fds_info FoundFd(int fd);
    void    fdData(fds_info &fdtmp, int fd);
    int     checkContentLength(std::string str);
    webServer(std::vector<Servers> servers);
    ~webServer();
};


