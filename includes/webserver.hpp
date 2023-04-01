/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ouzhamza <ouzhamza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 03:30:01 by garra             #+#    #+#             */
/*   Updated: 2023/03/30 15:48:53 by ouzhamza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once

# include <stdio.h>
# include <sys/socket.h>
# include <unistd.h>
# include <stdlib.h>
# include <netinet/in.h>
# include <poll.h>
# include <errno.h>
// # include <bits/stdc++.h>
# include <arpa/inet.h>
# include <sys/time.h>
# include <cstring>

# include "config.hpp"

#define RED "\033[1;31m"
#define GRN "\033[1;32m"
#define END "\033[0m"

class Servers;


class fds_info
{
public:
    std::vector<Servers>	my_servers;//
    std::vector<Servers>    all_servers;

    std::string             response;//
    std::string 			strHeader;//
    std::string 			serverName;//
    std::string				Connection;//
    pollfd              	tmp;

    size_t      			contentLength;//
    size_t                  responseLength;
    long int  				lastTime;
    
    bool        			isFirstTimeRead;//
    bool        			isFirstTimeSend;//
    bool        			isRecvComplet;//
    bool                    isTimeOut;
    
    int     				serverSock;
    int     				port;
    int         			readLen;
    int        			    HeaderLength;
    size_t         			totalRead;//
    size_t                  totalSend;//
    size_t                  bytesLeft;//
};


class webServer
{

private:
    std::vector<Servers>	_servers;
    std::vector<pollfd> 	fds;
    std::vector<Servers> 	_serv;
    std::vector<fds_info> 	fdsInfo;
    int                     clientSockets;

public:
    const char *fileExemple;

    fds_info	FoundFd(int fd);
    std::string foundKey(std::string str, std::string key);
    long int	getTimeMs();
    int     	guard(int n, const char *er);
    void     	pollIn(int &i);
    int     	checkContentLength(std::string str);
    void    	setupServer();
    void    	acceptConnection();
	void		sendData(fds_info &my_fd, int &i);
    void    	readHeader(int i);
    void    	pollOut(int &i, fds_info &my_fd);
    void    	foundServer(fds_info &my_fd);
    void    	fdData(fds_info &fdtmp, int fd);
    void    	checkOtherServers(int _port, std::vector<Servers> &ServReserve, fds_info &my_fd);
    void		checkFirstTime(fds_info &my_fd, std::string str);
    void    	resetMyFdInfo(fds_info &my_fd);
    int         getHeaderLength(const std::string& requestHeader);
    
    webServer(std::vector<Servers> servers);
    ~webServer();
};


