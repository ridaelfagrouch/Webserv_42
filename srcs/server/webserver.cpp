/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rel-fagr <rel-fagr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 04:00:17 by garra             #+#    #+#             */
/*   Updated: 2023/02/20 17:42:49 by rel-fagr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/webserver.hpp"

void webServer::fdData(fds_info &fdtmp, int fd)
{
	fdtmp.tmp.fd = fd;
	fdtmp.tmp.events = POLLIN | POLLOUT | POLLERR | POLLHUP;
	fdtmp.totalRead = 0;
	fdtmp.totalSend = 0;
	fdtmp.bytesLeft = 0;
	fdtmp.responseLength = 0;
	fdtmp.isRecvComplet = false;
	fdtmp.strHeader = "";
	fdtmp.isFirstTimeRead = true;
	fdtmp.isFirstTimeSend = true;
	fdtmp.response = "";
	fdtmp.Connection = "";
	fdtmp.serverName = "";
	fdtmp.strHeader = "";
}

//--------------------------------------------------------------------------

void    webServer::setupServer()
{
    for (size_t i = 0; i < _serv.size(); i++)
    {
	    int optval = 1;

		if ((_serv[i].socket_fd = guard(socket(AF_INET, SOCK_STREAM, 0), "socket error")) < 0)
			continue;
		if (guard(setsockopt(_serv[i].socket_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)), "setsockopt error") < 0)
			continue;
	   	if(guard(bind(_serv[i].socket_fd, (struct sockaddr *) &_serv[i]._address, sizeof(_serv[i]._address)), "bind error") < 0)
			continue;
        if(guard(listen(_serv[i].socket_fd, BACKLOG), "listen error") < 0)
			continue;
        if(guard(fcntl(_serv[i].socket_fd, F_SETFL, O_NONBLOCK), "fcntl error") < 0)
			continue;

		fds_info fdtmp;
		fdData(fdtmp, _serv[i].socket_fd);
	
		fdsInfo.push_back(fdtmp);
		fds.push_back(fdtmp.tmp);
		std::cout << "listening to server host " << _serv[i].host << " port " << _serv[i]._port << std::endl;
    }
	std::cout << std::endl;
    acceptConnection();
}

//--------------------------------------------------------------------------

void    webServer::pollIn(int &i)
{
	for (size_t i = 0; i < _serv.size(); i++)
	{
		if (fds[i].fd == _serv[i].socket_fd)
		{
			struct sockaddr_in	clientAddress;
    		socklen_t 			addrlen;
			while((clientSockets = accept(fds[i].fd,(struct sockaddr *) &clientAddress, &addrlen)) > 0)
			{
				if (fcntl(clientSockets, F_SETFL, O_NONBLOCK) < 0)
				{
					perror("fcntl error");
					return ;
				}
				fds_info fdtmp;
				fdData(fdtmp, clientSockets);

				fdtmp.serverSock = fds[i].fd;
				fdtmp.port = _serv[i]._port;
				fds.push_back(fdtmp.tmp);
				fdsInfo.push_back(fdtmp);
				return ;
			}
		}
	}
    readHeader(i);
	fdsInfo[i].lastTime = getTimeMs();
    if ((fdsInfo[i].readLen <= -1 && errno != EAGAIN) || fdsInfo[i].readLen == 0)
	{
		if (fdsInfo[i].readLen == 0)
			perror("Client disconnected");
		close(fdsInfo[i].tmp.fd);
		fdsInfo.erase(fdsInfo.begin()+i);
		fds.erase(fds.begin()+i);
		i--;
	}
}

//--------------------------------------------------------------------------

void webServer::resetMyFdInfo(fds_info &my_fd)
{
	if (my_fd.Connection != "close")
		my_fd.Connection.clear();
	else
		my_fd.Connection = "";
	if (my_fd.serverName != "Not_Found")
		my_fd.serverName.clear();
	else
		my_fd.serverName = "";
	my_fd.contentLength = 0;
	my_fd.isRecvComplet = false;
	my_fd.isFirstTimeRead = true;
	my_fd.isFirstTimeSend = true;
	my_fd.my_servers.clear();
	my_fd.strHeader.clear();
	my_fd.response.clear();
	my_fd.totalRead = 0;
	my_fd.totalSend = 0;
	my_fd.bytesLeft = 0;
	my_fd.responseLength = 0;
}

//--------------------------------------------------------------------------

void     webServer::pollOut(int &i, fds_info &my_fd)
{
	int len;
	std::fstream fileIN;
	std::string line;
	std::string appendLine;
	fileIN.open(fileExemple, std::fstream::in);
    if (!fileIN.is_open()) std::cout << "Can't open file!" << std::endl , std::exit(EXIT_FAILURE);
    while (!std::getline(fileIN, line).eof())
	{
        appendLine.append(line);
	}
	len = appendLine.length();

	if (my_fd.isFirstTimeSend)
	{
		my_fd.response = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length:";
		my_fd.response.append(std::to_string(len));
		my_fd.response.append("\n\n");
		my_fd.response.append(appendLine);
		fileIN.close();
	
		my_fd.responseLength = my_fd.response.size();
		my_fd.bytesLeft = my_fd.response.size();
		my_fd.isFirstTimeSend = false;
	}
	sendData(my_fd, i);
}

//--------------------------------------------------------------------------

void webServer::sendData(fds_info &my_fd, int &i)
{
	int	sendLen = 0;
    if (my_fd.totalSend < my_fd.responseLength && sendLen >= 0)
    {
       	sendLen = send(my_fd.tmp.fd, my_fd.response.c_str() + my_fd.totalSend, my_fd.bytesLeft, 0);
		if (sendLen >= 0)
		{
			my_fd.totalSend += sendLen;
        	my_fd.bytesLeft -= sendLen;
		}
		else
			perror("send error");
    }
	if (my_fd.totalSend >= my_fd.responseLength || sendLen < 0)
	{
		std::cout << "totalSend : " << my_fd.totalSend << std::endl;
		std::cout << GRN << "-> the response was successfully sent " << END << std::endl;
		std::cout << "----------------------------------------------------------" << std::endl;
		std::cout << std::endl;
		if (!strncmp(my_fd.Connection.c_str(), "close", my_fd.Connection.size()) || sendLen < 0)
		{
			close(my_fd.tmp.fd);
			fdsInfo.erase(fdsInfo.begin()+i);
			fds.erase(fds.begin()+i);
			i--;
		}
		else
			resetMyFdInfo(my_fd);
	}
}

//--------------------------------------------------------------------------


void    webServer::acceptConnection(void)
{
    while(1)
	{
		if (poll(&fds[0], fds.size(), 0) < 0)
		{
			perror("poll error");
			exit(1);
		}
		for (int i = 0; i < (int)fds.size(); i++)
		{
			if (fds[i].revents & POLLIN)
				pollIn(i);
			if (fds[i].revents & POLLOUT && fdsInfo[i].isRecvComplet)
				pollOut(i, fdsInfo[i]);
			if (fds[i].revents & (POLLHUP | POLLERR))
			{
    			close(fds[i].fd);
				fdsInfo.erase(fdsInfo.begin()+i);
				fds.erase(fds.begin()+i);
    			i--;
            	continue;
			}
			if ((getTimeMs() - fdsInfo[i].lastTime) > 300 && !fdsInfo[i].isFirstTimeRead)
			{
				std::cerr << "timeout error 504" << std::endl;
				exit(1);
			}
		}
	}
    for (size_t i = 0; i < fds.size(); i++)
        close(fds[i].fd);
}

//--------------------------------------------------------------------------

int webServer::guard(int n, const char *er)
{
    if (n < 0)
    {
        perror(er);
        return -1;
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

std::string	webServer::foundKey(std::string str, std::string key)
{
	std::vector<std::string> splited = split(str, '\n');
	size_t pos;
	for (size_t i = 0; i < splited.size(); i++)
	{
		pos = splited[i].find(key.c_str(), 0);
		if(pos != std::string::npos)
		{
			std::string ptr = splited[i].erase(pos, key.size());
			splited.clear();
			return trim(ptr, 13);
		}
	}
	splited.clear();
	if (!strncmp(key.c_str(), "Host: ", key.size()))
		return "Not_Found";
	else
		return "close";
}

//--------------------------------------------------------------------------

void webServer::checkOtherServers(int _port, std::vector<Servers> &ServReserve, fds_info &my_fd)
{
	for(size_t i = 0; i < _servers.size(); i++)
	{
		if(_servers[i].isDuplicate)
		{
			std::vector<int>::iterator it;
			it = std::find (_servers[i].dup_port.begin(), _servers[i].dup_port.end(), _port);
			if (it != _servers[i].dup_port.end())
			{
				std::vector<std::string>::iterator it1;
				it1 = std::find (_servers[i].server_name.begin(), _servers[i].server_name.end(), my_fd.serverName);
				if (it1 != _servers[i].server_name.end())
				{
					my_fd.my_servers.push_back(_servers[i]);
					return;
				}
				else
				{
					ServReserve.push_back(_servers[i]);
					break;
				}
			}
		}
	}
	for(size_t i = 0; i < ServReserve.size(); i++)
		my_fd.my_servers.push_back(ServReserve[i]);
}

//--------------------------------------------------------------------------

void  webServer::foundServer(fds_info &my_fd)
{
	std::vector<Servers> ServReserve;
	size_t 	j = 0;
	for (; j < _serv.size(); j++)
	{
		if(_serv[j].socket_fd == my_fd.serverSock && _serv[j]._port == my_fd.port)
		{
			std::vector<std::string>::iterator it;
			it = std::find (_serv[j].server_name.begin(), _serv[j].server_name.end(), my_fd.serverName);
			if(it != _serv[j].server_name.end())
			{
				my_fd.my_servers.push_back(_serv[j]);
				return;
			}
			else
			{
				ServReserve.push_back(_serv[j]);
				checkOtherServers(_serv[j]._port, ServReserve, my_fd);
				return;
			}
		}
	}
}

//--------------------------------------------------------------------------

int webServer::checkContentLength(std::string str)
{
	std::vector<std::string> splited = split(str, '\n');
	for (size_t i = 0; i < splited.size(); i++)
	{
		size_t pos = splited[i].find("Content-Length: ", 0);
		if(pos != std::string::npos)
		{
			std::string ptr = splited[i].erase(pos, 16);
			splited.clear();
			return ft_stoi(trim(ptr,13));
		}
	}
	splited.clear();
	return 0;
}

//--------------------------------------------------------------------------

long int webServer::getTimeMs()
{
	struct timeval tp;
	long int	Time;
	
	gettimeofday(&tp, NULL);
	Time = tp.tv_sec * 1000 + tp.tv_usec / 1000;
	return (Time);
}

//--------------------------------------------------------------------------

void	webServer::checkFirstTime(fds_info &my_fd, std::string str)
{
	if (my_fd.isFirstTimeRead)
	{
		my_fd.serverName = foundKey(str, "Host: ");
		my_fd.Connection = foundKey(str, "Connection: ");
		my_fd.contentLength = checkContentLength(str);
		foundServer(my_fd);
		my_fd.isFirstTimeRead = false;
	}
}

//--------------------------------------------------------------------------

void webServer::readHeader(int i)
{
    char buffer[1024] = {0};
    fds_info &my_fd = fdsInfo[i];

	if (!my_fd.isRecvComplet)
	{
		my_fd.readLen = 0;
    	my_fd.readLen = recv(my_fd.tmp.fd, buffer, sizeof(buffer), 0);
    	std::string str(buffer, my_fd.readLen);
		checkFirstTime(my_fd, str);
    	my_fd.strHeader += str;
    	my_fd.totalRead += my_fd.readLen;
		if (my_fd.totalRead > (size_t)my_fd.my_servers[0].client_max_body_size && my_fd.my_servers[0].client_max_body_size != 0)
		{
			resetMyFdInfo(my_fd);
			std::cerr << "server 413 Request Entity Too Large" << std::endl;
			return ;
		}
		if (my_fd.contentLength == 0 || (my_fd.totalRead > my_fd.contentLength))
		{
			my_fd.isRecvComplet = true;
			// std::cout << my_fd.strHeader << std::endl ;
			std::cout << "----------------------------------------------------------" << std::endl;
			std::cout << GRN << "-> Header request is complet : "<< END << std::endl;
			std::cout << "	- port : "<< my_fd.port <<  std::endl;
			std::cout << "	- host : "<< my_fd.my_servers[0].host <<  std::endl;
			std::cout << "	- connection : "<< my_fd.Connection <<  std::endl;
			std::cout << "	- serverName : "<< my_fd.serverName <<  std::endl;
			std::cout << "	- contentLength : "<< my_fd.contentLength <<  std::endl;
			std::cout << "	- totalRead : " << my_fd.totalRead << std::endl;
			std::cout << "----------------------------------------------------------" << std::endl;
		}
	}
}

//--------------------------------------------------------------------------