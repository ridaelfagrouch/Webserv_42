/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rel-fagr <rel-fagr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 04:00:17 by garra             #+#    #+#             */
/*   Updated: 2023/02/14 23:51:16 by rel-fagr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/webserver.hpp"

void webServer::fdData(fds_info &fdtmp, int fd)
{
	fdtmp.tmp.fd = fd;
	fdtmp.tmp.events = POLLIN;
	fdtmp.total = 0;
	fdtmp.is_complet = false;
	fdtmp.str_header = "";
	fdtmp.is_first_time = true;
}

//--------------------------------------------------------------------------

void    webServer::setupServer()
{
    for (size_t i = 0; i < _serv.size(); i++)
    {
	    int optval = 1;

	    _serv[i].socket_fd = guard(socket(AF_INET, SOCK_STREAM, 0), "socket_fd error");
	    guard(setsockopt(_serv[i].socket_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)), "setsockopt error");
	   	guard(bind(_serv[i].socket_fd, (struct sockaddr *) &_serv[i]._address, sizeof(_serv[i]._address)), "error bind");
        guard(listen(_serv[i].socket_fd, BACKLOG), "listen error");
        guard(fcntl(_serv[i].socket_fd, F_SETFL, O_NONBLOCK), "fcntl error");

		fds_info fdtmp;
		fdData(fdtmp, _serv[i].socket_fd);
	
		fdsInfo.push_back(fdtmp);
		fds.push_back(fdtmp.tmp);
		std::cout << "lestening to server host " << _serv[i].host << " port " << _serv[i]._port << std::endl;
    }
	std::cout << std::endl;
    acceptConnection();
}

//--------------------------------------------------------------------------

int webServer::is_socket(int fd)
{
	for (size_t i = 0; i < _serv.size(); i++)
	{
		if (fd == _serv[i].socket_fd)
		{
			while((client_sockets = accept(server_sock,(struct sockaddr *) &client_address, &addrlen)) > 0)
			{
				guard(fcntl(client_sockets, F_SETFL, O_NONBLOCK), "fcntl error");
				fds_info fdtmp;
				fdData(fdtmp, client_sockets);

				fds.push_back(fdtmp.tmp);
				fdsInfo.push_back(fdtmp);
			}
			server_sock = fd;
			port = _serv[i]._port;
			ip_host = _serv[i].host;
			return 1;
		}
	}
	return 0;
}


//--------------------------------------------------------------------------

int     webServer::Poll_in(int &i)
{
	if (is_socket(fds[i].fd))
		return 1;
	else
	{
        read_header(i);
        if ((fdsInfo[i].read_len <= -1 && errno != EAGAIN) || fdsInfo[i].read_len == 0)
		{
			if (fdsInfo[i].read_len == 0)
				perror("Client disconnected");
			close(fdsInfo[i].tmp.fd);
			fdsInfo.erase(fdsInfo.begin()+i);
			fds.erase(fds.begin()+i);
			i--;
			return 1;
		}
        else if (fdsInfo[i].is_complet == true)
        	fds[i].events = POLLOUT;
	}
	return 0;
}

//--------------------------------------------------------------------------

void     webServer::Poll_out(int i)
{
	std::fstream file;
	std::string line;
	std::string appendLine;
	std::string response = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length:";
	// Servers my_server = FoundServer();
	int len;
	
	file.open(fileExemple, std::fstream::in);
    if (!file.is_open()) std::cout << "Can't open file!" << std::endl , std::exit(EXIT_FAILURE);
    while (!std::getline(file, line).eof())
	{
        appendLine.append(line);
	}

	len = appendLine.length();
	response.append(std::to_string(len));
	response.append("\n\n");
	response.append(appendLine);
	
	sendall(fdsInfo[i].tmp.fd, response, response.size());
	
	close(fdsInfo[i].tmp.fd);
	fdsInfo.erase(fdsInfo.begin()+i);
	fds.erase(fds.begin()+i);
}


//--------------------------------------------------------------------------


void    webServer::acceptConnection(void)
{
    while(1)
	{
		guard(poll(&fds[0], fds.size(), 0), "poll error");
		for (int i = 0; i < (int)fds.size(); i++)
		{
			if (fds[i].revents & POLLIN)
			{
				if(Poll_in(i))
					continue;
			}
			else if (fds[i].revents & POLLOUT)
				Poll_out(i);
			// else if (fds[i].revents & (POLLHUP | POLLERR))
			// {
    		// 	close(fds[i].fd);
			// 	fdsInfo.erase(fdsInfo.begin()+i);
			// 	fds.erase(fds.begin()+i);
    		// 	i--;
            // 	continue;
			// }
		}
	}
    for (int i = 0; i < fds.size(); i++)
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
	this->_servers = servers;
    for (size_t i = 0; i < servers.size(); i++)
    {
        for(size_t j = 0; j < servers[i].port.size(); j++)
        {
            Servers _servers = servers[i];
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

fds_info webServer::FoundFd(int fd)
{
	size_t j = 0;
	for (; j < fdsInfo.size(); j++)
	{
		if(fd == fdsInfo[j].tmp.fd)
			break;
	}
	std::cout << "j = " << j << std::endl; 
	return fdsInfo[j];
}

//--------------------------------------------------------------------------

std::string	webServer::FoundServerName(std::string str)
{
	std::vector<std::string> splited = split(str, '\n');
	size_t pos;
	for (size_t i = 0; i < splited.size(); i++)
	{
		pos = splited[i].find("Host:", 0);
		if(pos != std::string::npos)
			return splited[i].erase(pos, 6);
	}
	return "Not_Found";
}

//--------------------------------------------------------------------------

void  webServer::FoundServer(std::vector<Servers> &my_servers, fds_info my_fd)
{
	size_t j = 0;
	for (; j < _serv.size(); j++)
	{
		if(_serv[j].socket_fd == server_sock && _serv[j]._port == port && _serv[j].host == ip_host)
		{
			for(size_t i; i < _serv[j].server_name.size(); i++)
			{
				if (my_fd.serverName == _serv[j].server_name[i])
				{
					my_servers.push_back(_serv[j]);
					return;
				}
			}
			if (my_fd.serverName == "Not_Found" || _serv[j].server_name.size() == 0)
			{
				my_servers.push_back(_serv[j]);
				for(size_t i; i < _servers.size(); i++){;}
				//I stope here
				//I stope here
				//I stope here
				//I stope here
				//I stope here
				//I stope here
				//I stope here
				//I stope here
				//I stope here
				//I stope here
			}
		}
	}
}

//--------------------------------------------------------------------------

int webServer::checkContentLength(std::string str)
{
	std::vector<std::string> splited = split(str, '\n');
	size_t pos;
	for (size_t i = 0; i < splited.size(); i++)
	{
		pos = splited[i].find("Content-Length:", 0);
		if(pos != std::string::npos)
		{
			std::string ptr = splited[i].erase(pos, 16);
			return ft_stoi(trim(ptr,13));
		}
	}
	return 0;
}

//--------------------------------------------------------------------------

void webServer::read_header(int i)
{
	std::vector<Servers> my_servers;
    char buffer[50000] = {0};
    fds_info &my_fd = fdsInfo[i];

	my_fd.read_len = 0;
    my_fd.read_len = read(my_fd.tmp.fd, buffer, sizeof(buffer));
	if (my_fd.read_len > 0)
	{
    	std::string str(buffer, my_fd.read_len);
		if(my_fd.is_first_time)
		{
			my_fd.serverName = FoundServerName(str);
			my_fd.content_length = checkContentLength(str);
			FoundServer(my_servers, my_fd);
			my_fd.is_first_time = false;
		}
		
    	my_fd.str_header += str;
    	my_fd.total += my_fd.read_len;
		if (my_fd.total > my_servers[0].client_max_body_size && my_servers[0].client_max_body_size != 0)
		{
			my_fd.str_header = "";
			std::cerr << "server 413 Request Entity Too Large" << std::endl;
			return ;
		}
		if (my_fd.content_length == 0 || (my_fd.total == my_fd.content_length))
			my_fd.is_complet = true;
    }
	
	std::cout << "str_header = "<< my_fd.str_header << std::endl;
	// std::cout <<"is_complet = " << my_fd.is_complet<<std::endl;
	// std::cout <<"is_first_time = " << my_fd.is_first_time<<std::endl;
	// std::cout <<"content_length = " << my_fd.content_length<<std::endl;
	// std::cout <<"read_len = " << my_fd.read_len<<std::endl;
	// std::cout <<"total = " << my_fd.total<<std::endl;
	std::cout << "--------------------------------------------" << std::endl;
	
}

//--------------------------------------------------------------------------


void webServer::sendall(int fd, std::string response, int len)
{
    int total = 0;
    int bytesleft = len;
    int n;
    while(total < len)
    {
        n = send(fd, response.c_str()+total, bytesleft, 0);
        if (n == -1)
            break;
        total += n;
        bytesleft -= n;
    }
}