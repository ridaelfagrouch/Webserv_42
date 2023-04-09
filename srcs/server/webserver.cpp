/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sahafid <sahafid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 04:00:17 by garra             #+#    #+#             */
/*   Updated: 2023/04/09 00:57:26 by sahafid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/webserver.hpp"
#include "../../includes/request.hpp"
#include "../../includes/response.hpp"

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
	fdtmp.isTimeOut = false;
}

//--------------------------------------------------------------------------

void    webServer::setupServer()
{
	int countNoBind = 0; 
    for (size_t i = 0; i < _serv.size(); i++)
    {
	    int optval = 1;
		
		if ((_serv[i].socket_fd = guard(socket(AF_INET, SOCK_STREAM, 0), "socket error")) < 0)
			continue;
		if (guard(setsockopt(_serv[i].socket_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)), "setsockopt error") < 0)
			continue;
		if(guard(bind(_serv[i].socket_fd, (struct sockaddr *) &_serv[i]._address, sizeof(_serv[i]._address)), "bind error") < 0)
		{
			countNoBind++;
			if (countNoBind == (int)_serv.size())
				exit(1);
			continue;
		}
        if(guard(listen(_serv[i].socket_fd, SOMAXCONN), "listen error") < 0)
			continue;
        if(guard(fcntl(_serv[i].socket_fd, F_SETFL, O_NONBLOCK), "fcntl error") < 0)
			continue;

		fds_info fdtmp;
		fdData(fdtmp, _serv[i].socket_fd);
	
		fdsInfo.push_back(fdtmp);
		fds.push_back(fdtmp.tmp);
		std::cout << "listening to server host " << _serv[i].host << " port " << _serv[i]._port << std::endl;
    }
    acceptConnection();
}

//--------------------------------------------------------------------------

int    webServer::pollIn(int &i)
{
	if (i < (int)_serv.size() && fds[i].fd == _serv[i].socket_fd)
	{
		struct sockaddr_in	clientAddress;
    	socklen_t 			addrlen = sizeof(clientAddress);
		if((clientSockets = accept(_serv[i].socket_fd,(struct sockaddr *) &clientAddress, &addrlen)) > 0)
		{
			if (fcntl(clientSockets, F_SETFL, O_NONBLOCK) < 0)
			{
				perror("fcntl error");
				return (0);
			}
			fds_info fdtmp;
			fdData(fdtmp, clientSockets);
			fdtmp.serverSock = fds[i].fd;
			fdtmp.port = _serv[i]._port;
			fds.push_back(fdtmp.tmp);
			fdsInfo.push_back(fdtmp);
		}
		return (0);
	}
	else
	{
    	readHeader(fdsInfo[i]);
		fdsInfo[i].lastTime = getTimeMs();
    	if (fdsInfo[i].readLen <= 0)
		{
			if (fdsInfo[i].readLen == 0)
				perror("Client disconnected");
			close(fdsInfo[i].tmp.fd);
			fdsInfo.erase(fdsInfo.begin()+i);
			fds.erase(fds.begin()+i);
			return (-1);
		}
	}
	return (0);
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
	my_fd.isTimeOut = false;
}

//--------------------------------------------------------------------------

void     webServer::pollOut(int &i, fds_info &my_fd)
{

	// my_fd.isTimeOut //! this should be checked at the responce begining (if yes "error page should be genarated")
	if (my_fd.isFirstTimeSend)
	{
		Request request(my_fd.strHeader);
		Response response(request, my_fd);
		// my_fd.response = "HTTP/1.1 405 OK\nContent-Type: text/plain\nContent-Length: 19\n\fdnMethod Not Allowed!";
		my_fd.response = response.call();
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
		// std::cout << "totalSend : " << my_fd.totalSend << std::endl;
		// std::cout << GRN << "-> the response was successfully sent " << END << std::endl;
		// std::cout << "----------------------------------------------------------" << std::endl;
		// std::cout << std::endl;
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
		if (poll(&fds[0], fds.size(), -1) < 0 && errno != EAGAIN && errno != EWOULDBLOCK)
		{
			perror("poll error");
			continue;
		}
		for (int i = 0; i < (int)fds.size(); i++)
		{
			if (fds[i].revents & POLLIN )
			{
				if (pollIn(i) == -1)
					continue;
			}
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
			if ((getTimeMs() - fdsInfo[i].lastTime) >= 5000 && !fdsInfo[i].isFirstTimeRead)
			{
				fds[i].revents = POLLOUT;
				fdsInfo[i].isTimeOut = true;
				fdsInfo[i].isRecvComplet = true;
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
	return !strncmp(key.c_str(), "Host: ", key.size()) ? "Not_Found" : "close";
}

//--------------------------------------------------------------------------

void webServer::checkOtherServers(int _port, std::vector<Servers> &ServReserve, fds_info &my_fd)
{
	my_fd.all_servers = _servers;
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
	
	gettimeofday(&tp, NULL);
	return (tp.tv_sec * 1000 + tp.tv_usec / 1000);
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
		my_fd.HeaderLength = getHeaderLength(str);
	}
}

//--------------------------------------------------------------------------


std::string clean_chunked_request(std::string request) {
    std::string clean_request = "";
    int body_start = request.find("\r\n\r\n") + 4;

    if (body_start == 3) {
        clean_request = request;
        return clean_request;
    }

    std::string header = request.substr(0, body_start);
    std::string body = request.substr(body_start);

    size_t found = header.find("Transfer-Encoding: chunked\r\n");
    if (found != std::string::npos) {
        header.erase(found, 29);
    }
	else
		return clean_request;

    while (true) {
        found = body.find("\r\n");
        if (found == std::string::npos)
            break;
        int length = stoi(body.substr(0, found), nullptr, 16);
        body.erase(0, found + 2);
        if (length == 0)
            break;
        clean_request += body.substr(0, length);
        body.erase(0, length + 2);
    }

    clean_request = header + "\r\n" + clean_request;

    return clean_request;
}

//--------------------------------------------------------------------------

int webServer::getHeaderLength(const std::string& requestHeader) {
    size_t endOfHeaderPos = requestHeader.find("\r\n\r\n");
    if (endOfHeaderPos == std::string::npos) {
        return -1;
    }
    return endOfHeaderPos + 4;
}

//--------------------------------------------------------------------------

void webServer::readHeader(fds_info &my_fd)
{
    char buffer[50000] = {0};

	if (!my_fd.isRecvComplet)
	{
		my_fd.readLen = 0;
    	my_fd.readLen = recv(my_fd.tmp.fd, buffer, 50000, 0);
		if (my_fd.readLen > 0)
		{
    		std::string str(buffer, my_fd.readLen);
			checkFirstTime(my_fd, str);
    		my_fd.strHeader.append(str);
			if (my_fd.readLen < 0)
				my_fd.readLen = 0;
    		my_fd.totalRead += my_fd.readLen;
			if (my_fd.HeaderLength > 0 && my_fd.totalRead > 0)
			{
				my_fd.totalRead -= my_fd.HeaderLength;
				my_fd.HeaderLength = 0;
			}
			if (my_fd.totalRead > (size_t)my_fd.my_servers[0].client_max_body_size && my_fd.my_servers[0].client_max_body_size != 0)
			{
				close(my_fd.tmp.fd);
				resetMyFdInfo(my_fd);
				std::cerr << "server 413 Request Entity Too Large" << std::endl;
				return ;
			}
			if (my_fd.contentLength == 0 || (my_fd.totalRead >= my_fd.contentLength))
			{
				my_fd.isRecvComplet = true;
				std::string clean_request = clean_chunked_request(my_fd.strHeader);
				if (clean_request.size() > 0)
				{
					my_fd.strHeader.clear();
					my_fd.strHeader = clean_request;
					clean_request.clear();
				}

			}
		}
	}

}
