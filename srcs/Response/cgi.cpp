/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houazzan <houazzan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 13:36:29 by ouzhamza          #+#    #+#             */
/*   Updated: 2023/04/06 02:07:35 by houazzan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/response.hpp"

int Response::runcgi()
{
    if (!fillClass())
        return(_ret = 500, 0);
    return (_ret = 200, 1);

}

int Response::fillClass()
{
    Cgi cgi;
    cgi.setCgiMethode(request.get_methode());
    cgi.setCgiPath(*this);
    cgi.setCgiQuery(_query);
    cgi.setCgiroot(server.root);
    cgi.setCgiServerName(server.server_name[0]);
    cgi.setCgiredirectStatus(_ret);
    cgi.setCgicontentLength(request.get_header("Content-Length"));
    cgi.setCgicontentType(request.get_header("Content-Type"));
    cgi.setCoockies(request.get_header("Cookie"));
    cgi.setCgiserverProtocol();
    cgi.setCgiPort(request.get_port());
    try {
        _Resbody += executeCgi(cgi.getCgiPath(), cgi);
    }
    catch (std::exception &e)
    {
        return 0;
    }
    return 1;
}

void Response::Cgi::setCgiMethode(std::string _methode)
{
    Methode = _methode;
}

void Response::Cgi::setCgiPath(Response &response)
{
    if (!response._cgi && response._indexcgi)
        Path  = "/" + response._index;
    else
        Path = response._path;
}

void Response::Cgi::setCgiQuery(std::string _query)
{
    Query = _query;
}

void Response::Cgi::setCgiroot(std::string _root)
{
    root = _root;
}

void Response::Cgi::setCgiredirectStatus(int _ret)
{
    std::stringstream ss;
    ss << _ret;
    this->redirectStatus = ss.str();
}

void Response::Cgi::setCgicontentLength(std::string _lenght)
{
    contentLength = _lenght;
}

void Response::Cgi::setCoockies(std::string coockie)
{
    this->Coockies = coockie;
}

void Response::Cgi::setCgicontentType (std::string _type)
{
    contentType = _type;
}

void Response::Cgi::setCgiServerName(std::string _serverName)
{
    serverName = _serverName;
}

void Response::Cgi::setCgiserverProtocol()
{
    serverProtocol = "TCP";
}

void Response::Cgi::setCgiPort(int port)
{
    std::stringstream ss;
    ss << port;
    this->port = ss.str();
}





std::string		Response::Cgi::getCgicontentType()
{
    return contentType;
}

std::string		Response::Cgi::getCgicontentLength()
{
    return contentLength;
}

std::string		Response::Cgi::getCgiPath()
{
    return Path;
}

std::string		Response::Cgi::getCgiQuery()
{
    return Query;
}

std::string		Response::Cgi::getCgiredirectStatus()
{
    return redirectStatus;
}

std::string		Response::Cgi::getCgiroot()
{
    return root;
}

std::string		Response::Cgi::getCgiServerName()
{
    return serverName;
}

std::string		Response::Cgi::getCgiMethode()
{
    return Methode;
}

std::string		Response::Cgi::getCgiserverProtocol()
{
    return serverProtocol;
}

std::string		Response::Cgi::getCgiPort()
{
    return port;
}

std::string		Response::Cgi::getCoockies()
{
    return Coockies;
}