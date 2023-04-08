/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requestUtiles.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houazzan <houazzan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 15:32:14 by houazzan          #+#    #+#             */
/*   Updated: 2023/04/07 23:39:06 by houazzan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/request.hpp"
#include "../../includes/config.hpp"

void Request::tokenizingAndParsing(const std::string &str)
{
    size_t i = 0;
    std::string line;
    _ret = parseFirstline(getLine(str, i));
    while ((line = getLine(str, i)) != "\r" && line != "" && _ret != 400)
        _header[set_key(line)] = set_value(line);
    _ret = checkHost();
    set_port();
    set_body(str, i);
}

 /* *************************************************************************** */


std::string Request::getLine(const std::string &str, size_t &i)
{
    size_t j = 0;
    std::string line;
    j = str.find_first_of('\n', i);
    if (i == std::string::npos || j == std::string::npos)
        return (_ret = 400, "");
    line = str.substr(i, j - i);
    if (line[line.size() - 1] == '\r')
         line.erase(line.size() - 1);
    i = j + 1;
    return (line);
}
 /* *************************************************************************** */

int Request::parseFirstline(const std::string &line)
{
    if (!set_methode(line) || !set_path(line) || !set_version(line))
        return(400);
    return(200);
}
 /* *************************************************************************** */

 int Request::set_methode(const std::string &line)
 {
    size_t i;
    i = line.find_first_of(' ');
    if (i == std::string::npos)
        return(0);
    _methode = line.substr(0, i);
    return (1);
 }
 /* *************************************************************************** */

 int Request::set_path(const std::string &line)
{
    size_t i;
    size_t j;
    i = line.find_first_of(' ');
    j = line.find_first_of(' ', i + 1);
    
    if (j == std::string::npos || i == std::string::npos)
        return(0);
    _path = line.substr(i + 1, j - i - 1);
    if (!_path.empty()){}
    refinement();
    return (1);
}
 /* *************************************************************************** */

void Request::refinement()
{
    size_t i = _path.find_first_of('?');
    if (i != std::string::npos)
    {
        _query = _path.substr(i + 1, std::string::npos);
        _path = _path.substr(0, i);
    }
}

 /* *************************************************************************** */
int Request::set_version(const std::string &line)
{
    size_t i;
    size_t j;
    i = line.find_first_of(' ');
    j = line.find_first_of(' ', i + 1);
    if (j == std::string::npos)
        return(0);
    _version = line.substr(j + 1);
    return(1); 
}
 /* *************************************************************************** */

std::string Request::set_key(const std::string &str)
{
    std::string ret;
    size_t i = str.find_first_of(':');
    if (i != std::string::npos)
        ret.assign(str, 0, i);
    return (ret);
}

 /* *************************************************************************** */

std::string Request::set_value(const std::string &str)
{
    std::string ret;
    size_t i  = str.find_first_of(':');
    ret = str.substr(i + 2);
    return(ret);
}

 /* *************************************************************************** */

void Request::set_body(const std::string &str, size_t i)
{
    if (i != std::string::npos)
        _body = str.substr(i);
}
 /* *************************************************************************** */

void Request::set_port()
{
   _port = atoi(extractPort().c_str());
}
 /* *************************************************************************** */


int Request::checkHost()
{
    for (std::map<std::string, std::string>::iterator it = _header.begin(); it != _header.end(); it++)
    {
        if (it->first.compare("Host") == 0)
            return(200);
    }
    return(400);
}


std::string Request::extractPort()
{
    std::string host;
    size_t i;
    host = get_header("Host");
    if ((i = host.find(":")) != std::string::npos)
        return (host.substr(i + 1));
    return ("0");
}














 