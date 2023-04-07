/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sahafid <sahafid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 00:52:50 by ouzhamza          #+#    #+#             */
/*   Updated: 2023/04/07 02:47:54 by sahafid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/response.hpp"


Response::Response(Request &_request,  fds_info &_fd) : request(_request), fd(_fd), server(fd.my_servers[0])
{
	initErrorMap();
	initRespMaps();
	initContentMap();
	setPath();
	setBool();
	if (isAbsoluteURI())
		changeHost();
	if (_fd.isTimeOut == true)
	{
		std::cout << "here" << std::endl;
		_ret = 504;
	}
}


/* ************************************************************************** */

Response::~Response(){}

Response::Cgi::Cgi(){}

Response::Cgi::~Cgi(){}

/* ************************************************************************** */

int Response::isAbsoluteURI()
{
	size_t i;
	if ((i = _path.find("http://"))!= std::string::npos ||(i = _path.find("https://")) != std::string::npos)
		return (1);
	return (0);
}

/* ************************************************************************** */

void Response::changeHost()
{
	size_t i;
	size_t j;
	std::string newHost;
	if ((i = _path.find("http://")) != std::string::npos) {
		
		j = _path.find("/", i + 7);
		newHost = _path.substr(i + 7, j);
	}
	
	else if ((i = _path.find("http://")) != std::string::npos) {
		
		j = _path.find("/", i + 8);
		newHost = _path.substr(i + 8, j);
	}
	
	if ((i = newHost.find("localhost")) != std::string::npos) {

		newHost.erase(i, 9);
		newHost = "127.0.0.1" + newHost;
	}

	for (std::vector<Servers>::iterator it = fd.all_servers.begin(); it != fd.all_servers.end(); it++) {
		
		for(std::vector<int>::iterator i = it->port.begin(); i != it->port.end(); i++) {
		
 			if (!newHost.compare(it->host + ":" + to_String(*i)))				
				server = *it;
		}
	}
}

/* ************************************************************************** */

void	Response::initErrorMap()
{
	_status_code[100] = "Continue";
	_status_code[200] = "OK";
	_status_code[201] = "Created";
	_status_code[202] = "Accepted";
	_status_code[204] = "No Content";
	_status_code[300] = "Multiple Choices";
	_status_code[301] = "Moved Permanently";
	_status_code[302] = "Found";
	_status_code[303] = "See Other";
	_status_code[304] = "Not Modified";
	_status_code[305] = "Use Proxy";
	_status_code[307] = "Temporary Redirect";
	_status_code[400] = "Bad Request";
	_status_code[403] = "Forbidden";
	_status_code[404] = "Not Found";
	_status_code[405] = "Method Not Allowed";
	_status_code[413] = "Payload Too Large";
	_status_code[500] = "Internal Server Error";
	_status_code[504] = "Gateway Timeout";
}


void Response::setBool()
{
	_autoindex = false;
	_cgi   = false;
	_indexcgi = false;
	_l = std::string::npos;
}
/* ************************************************************************** */
void	Response::initRespMaps()
{
	_header["Date"] = "Date: ";
	_header["Connection"] = "Connection: ";
	_header["Content-Type"] = "Content-Type: ";
	_header["Content-Length"] = "Content-Length: ";
	_header["Location"] = "Location: ";
	_header["Allow"] = "Allow: ";
}

/* ************************************************************************** */

void	Response::initContentMap()
{
	std::ifstream file;
	std::string line;
	
	file.open("srcs/Conf/mime.types");
	if (file.is_open())
	{
		while (getline(file, line))
			_contentType[get_key(line)] = get_value(line);
		file.close();
	}
	else
		std::cout << "Can't open mime.types file" << std::endl;
}

/* ************************************************************************** */

std::string Response::call()
{
	if (_ret != 200)
		error();
	else {
	_ret = ParsingResponse();
	if (_ret != 200)
		error();
	else if(_methode == "GET")
		getMethode();
	else if (_methode == "POST")
		postMethode();
	else if (_methode == "DELETE")
		deleteMethode();
	}
	headerGen();
	return (_response);
}

/* ************************************************************************** */

void	Response::setPath()
{
	if (server.root[server.root.length() - 1] == '/')
		server.root.erase(server.root.length() - 1);
	this->_path = request.get_path();
	_path = decodePath();
	_root = server.root;
	setVersion();
}


std::string Response::decodePath()
{
	std::stringstream output;
	char decoded[3];
	std::string var;
	for (std::string::const_iterator it = _path.begin(); it != _path.end(); it++) {
		if (*it == '%')
		{
			decoded[0] = *(++it);
			decoded[1] = *(++it);
			var  = std::strtol(decoded, NULL, 16);
			output << var;
		}
		else if (*it == '+')
            output << ' ';
         else 
            output << *it;
	}
	return (output.str());
}

void	Response::setVersion()
{
	this->_version = "HTTP/1.1 ";
	setQuery();
}

void	Response::setQuery()
{
	this->_query = request.get_query();
	setRet();
}

void	Response::setRet()
{
	this->_ret = request.get_ret();
	setMethode();
}


void	Response::setMethode()
{
	this->_methode = request.get_methode();
	setPort();
}

void	Response::setPort()
{
	this->_port = request.get_port();
	setHost();
}

void	Response::setHost()
{
	this->_host = request.get_header("Host");
	setBody();
}

void	Response::setBody()
{
	this->_Reqbody = request.get_body();
}

std::string Response::get_value(std::string line)
{
	size_t i = line.find(":");
	return (line.substr(i + 1));
}

std::string Response::get_key(std::string line)
{
	size_t i = line.find(":");
	return (line.substr(0, i));	
}

std::string	Response::get_index()
{
	return (_index);
}

