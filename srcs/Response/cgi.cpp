/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ouzhamza <ouzhamza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 13:36:29 by ouzhamza          #+#    #+#             */
/*   Updated: 2023/03/28 22:22:13 by ouzhamza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/response.hpp"

int Response::runcgi()
{
    fillClass();
    return(1);
}

void Response::fillClass()
{
    Cgi cgi;
    cgi.setCgiMethode(request.get_methode());
    cgi.setCgiPath(request.get_path());
    cgi.setCgiQuery(_query);
    cgi.setCgiroot(server.root);
    cgi.setCgiServerName(server.server_name[0]); //! NOT SUR OF THIS
    cgi.setCgiredirectStatus(_ret);
    cgi.setCgicontentLength(request.get_header("Content-Length"));
    cgi.setCgicontentType(request.get_header("Content-Type"));
    cgi.setCgiserverProtocol();
    // std::cout << server.port[0] << std::endl; 
}

void Response::Cgi::setCgiMethode(std::string _methode)
{
    Methode = _methode;
}

void Response::Cgi::setCgiPath(std::string _path)
{
    Path = _path;
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
    redirectStatus = _ret;
}

void Response::Cgi::setCgicontentLength(std::string _lenght)
{
    contentLength = _lenght;
}

void Response::Cgi::setCgicontentType (std::string _type)
{
    contentType = _type;
}

void Response::Cgi::setCgiServerName(std::string _serverName)
{
    sreverName = _serverName;
}

void Response::Cgi::setCgiserverProtocol()
{
    serverProtocol = "TCP";
}
