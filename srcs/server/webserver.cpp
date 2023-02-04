/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garra <garra@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 04:00:17 by garra             #+#    #+#             */
/*   Updated: 2023/02/04 18:59:23 by garra            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/webserver.hpp"

void    webServer::setupServer()
{
    for (size_t i = 0; i < _serv.size(); i++)
    {
        int socket_fd;
	    int optval = 1;

	    socket_fd = guard(socket(AF_INET, SOCK_STREAM, 0), "socket_fd error");
	    guard(setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)), "setsockopt error");
	    guard(bind(socket_fd, (struct sockaddr *) &_serv[i]._address, sizeof(_serv[i]._address)), "bind error");
        guard(listen(socket_fd, 100), "listen error");
        guard(fcntl(socket_fd, F_SETFL, O_NONBLOCK), "fcntl error");
		static int index = 0;

	    fds[index].fd = socket_fd;
	    fds[index].events = 0 | POLLIN;
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
			m_socket = fd;
			return 1;
		}
	}
	return 0;
}

//--------------------------------------------------------------------------

void    webServer::acceptConnection(void)
{
    while(1)
	{
		guard(poll(fds, fds_len, 0), "poll error");
		for (int i = 0; i < fds_len; ++i)
		{
			int fd = fds[i].fd;
			if (fds[i].revents & POLLIN)
			{
				if (is_socket(fd))
				{
					while ((client_sockets = accept(m_socket,(struct sockaddr *) &remote, &addrlen)) > 0)
	                {
	                	guard(fcntl(client_sockets, F_SETFL, O_NONBLOCK), "fcntl error");
	                	fds[fds_len].fd = client_sockets;
	                	fds[fds_len].events = 0 | POLLIN;
	                	fds_len++;
	                }
					continue;
				}
				else
				{
                    	this->str_header = "";
                    	int read_len;
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
                    
                    	if (read_len == -1 && errno != EAGAIN)
                    		close(fd);
                    	if (read_len == 0)
                    	{
                    		perror("Client disconnected");
                    		fds[i].fd = -1;
                    		fds_len--;
                    	}
                    	else
                    		fds[i].events = POLLOUT;
				}
			}
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

int webServer::sendall(int s, const char *buf, int len)
{
    int total = 0;
    int bytesleft = len;
    int n;
    while(total < len)
    {
        n = send(s, buf+total, bytesleft, 0);
        if (n == -1)
            break;
        total += n;
        bytesleft -= n;
    }
    // *len = total; 
    return n==-1?-1:0;
}