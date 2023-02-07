/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garra <garra@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 04:00:17 by garra             #+#    #+#             */
/*   Updated: 2023/02/07 15:19:12 by garra            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/webserver.hpp"

void    webServer::setupServer()
{
    for (size_t i = 0; i < _serv.size(); i++)
    {
		static int index = 0;
        int socket_fd;
	    int optval = 1;

	    socket_fd = guard(socket(AF_INET, SOCK_STREAM, 0), "socket_fd error");
	    guard(setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)), "setsockopt error");
	    guard(bind(socket_fd, (struct sockaddr *) &_serv[i]._address, sizeof(_serv[i]._address)), "bind error");
        guard(listen(socket_fd, 100), "listen error");
        guard(fcntl(socket_fd, F_SETFL, O_NONBLOCK), "fcntl error");

	    fds[index].fd = socket_fd;
	    fds[index].events = POLLIN;
	    index++;
	    fds_len = index;
	    socket_list.push_back(socket_fd);
    }
    acceptConnection();
}


//--------------------------------------------------------------------------

int webServer::is_socket(int fd)
{
	for (size_t i = 0; i < socket_list.size(); ++i)
	{
		if (fd == socket_list[i])
		{
			server_sock = fd;
			return 1;
		}
	}
	return 0;
}

//--------------------------------------------------------------------------

int     webServer::Poll_in(int i)
{
	if (is_socket(fds[i].fd))
	{
		while((client_sockets = accept(server_sock,(struct sockaddr *) &client_address, &addrlen)) > 0)
		{
			guard(fcntl(client_sockets, F_SETFL, O_NONBLOCK), "fcntl error");
	    	fds[fds_len].fd = client_sockets;
	    	fds[fds_len].events = POLLIN;
	    	fds_len++;
		}
		return(1);
	}
	else
	{
		int read_len = 0;
        read_all(fds[i].fd, read_len);
        if (read_len <= -1 && errno != EAGAIN)
        	close(fds[i].fd);
        if (read_len == 0)
        {
        	perror("Client disconnected");
        	fds[i].fd = -1;
        	fds_len--;
        }
        else
        	fds[i].events = POLLOUT;
	}
	return(0);
}

//--------------------------------------------------------------------------

void     webServer::Poll_out(int i)
{
	(void)i;
	// sendall(fds[i].fd, response, len);
	// close(fds[i].fd);
	// fds[i].fd = -1;
}

//--------------------------------------------------------------------------

void	webServer::Poll_HupErr(int &i)
{
	perror("Connection error with client");
    close(fds[i].fd);
    i--;
}

//--------------------------------------------------------------------------


void    webServer::acceptConnection(void)
{
    while(1)
	{
		guard(poll(fds, fds_len, -1), "poll error");
		for (int i = 0; i < fds_len; ++i)
		{
			if (fds[i].revents & POLLIN)
			{
				if(Poll_in(i))
					continue;
			}
			else if (fds[i].revents & (POLLHUP | POLLERR))
			{
				Poll_HupErr(i);
            	continue;
			}
			// else if (fds[i].revents & POLLOUT)
			// 	Poll_out(i);
		}
	}
    for (int i = 0; i < fds_len; i++)
        close(fds[i].fd);
}

//--------------------------------------------------------------------------

int webServer::guard(int n, const char *er)
{
    if (n < 0)
    {
        perror(er);
        exit(EXIT_FAILURE);
    }
    return n;
}

//--------------------------------------------------------------------------

webServer::webServer(std::vector<Servers> servers)
{
    for (size_t i = 0; i < servers.size(); i++)
    {   
        for(size_t j = 0; j < servers[i].port.size(); j++)
        {
            Servers _servers;
            _servers._port = servers[i].port[j];
            _servers._address.sin_family = AF_INET;
            _servers._address.sin_addr.s_addr = inet_addr(servers[i].host.c_str());
            _servers._address.sin_port = htons(servers[i].port[j]);
            memset(_servers._address.sin_zero, '\0', sizeof(_servers._address.sin_zero));
            this->_serv.push_back(_servers);
        }
    }
}

webServer::~webServer(){}

//--------------------------------------------------------------------------


void webServer::read_all(int fd, int &read_len)
{
	this->str_header = "";
    char buffer[1024];
    size_t total;
    memset(buffer, '\0', sizeof(buffer));
    total = 0;
    while ((read_len = read(fd, buffer, sizeof(buffer))) > 0)
    {
    	total += read_len;
    	std::string str(buffer, read_len);
    	this->str_header.append(str);
    }
	std::cout << str_header << std::endl;
}

//--------------------------------------------------------------------------


void webServer::sendall(int fd, const char *buf, int len)
{
    int total = 0;
    int bytesleft = len;
    int n;
    while(total < len)
    {
        n = send(fd, buf+total, bytesleft, 0);
        if (n == -1)
            break;
        total += n;
        bytesleft -= n;
    }
}