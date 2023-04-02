/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkvalid.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sahafid <sahafid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 19:43:04 by sahafid           #+#    #+#             */
/*   Updated: 2023/04/02 21:59:49 by sahafid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/config.hpp"


void    checkFile(std::vector<errorPages> &error_pages)
{
    for (std::vector<errorPages>::iterator it = error_pages.begin(); it != error_pages.end(); it++)
    {
        std::ifstream file;
        file.open((*it).path.c_str());
        if (!file)
        {
            file.open((const char *)("/home/ouzhamza/Desktop/Webserv_42" + (*it).path).c_str());
            if (!file)
                (*it).path = "./srcs/Conf/error/error.html";
        }
    }
}

void    checkLocationValidity(Locations &location, std::string root)
{
    if (location.root.empty())
        location.root = root;
    if (location.autoindex.empty())
        location.autoindex = "off";
}

void    checkDataValidity(Servers &server)
{
    std::vector<int> status_code;

    if (server.root.empty())
        throw std::invalid_argument("invalid input: no root");
        
    if (server.host.empty())
        throw std::invalid_argument("invalid input: no host");

    if (server.port.size() < 1)
        throw std::invalid_argument("invalid input: no port number");

    for (std::vector<errorPages>::iterator it = server.error_page.begin(); it != server.error_page.end(); it++)
    {
        if (std::find(status_code.begin(), status_code.end(), (*it).status_code) != status_code.end())
            throw std::invalid_argument("Syntax error: duplicate status code");
        status_code.push_back((*it).status_code);
    }
    if (server.client_max_body_size == -1)
        server.client_max_body_size = 1;

    checkFile(server.error_page);
    std::vector<std::string> directive;
    for (std::vector<Locations>::iterator it = server.locations.begin(); it != server.locations.end(); it++)
    {
        if (find(directive.begin(), directive.end(), (*it).directive) != directive.end())
            throw std::invalid_argument("invalid input: duplicate directive");
        checkLocationValidity(*it, server.root);
    }
}


void    checkDuplicates(Servers &server1, Servers &server2)
{
    
    if ((server1.host != server2.host))
        return ;
    for (std::vector<int>::iterator it = server1.port.begin(); it != server1.port.end(); it++)
    {
        if (find(server2.port.begin(), server2.port.end(), *it) != server2.port.end())
        {
            server2.dup_port.push_back(*it);
            server2.isDuplicate = 1;
            std::vector<int>::iterator iter = find(server2.port.begin(), server2.port.end(), *it);
            server2.port.erase(iter);
        }
    }
}


void    checkSpecialCase(std::vector<Servers> &servers)
{
    for (std::vector<Servers>::iterator it = servers.begin(); it != servers.end(); it++)
    {
        for (std::vector<Servers>::iterator iter = it + 1; iter != servers.end(); iter++)
            checkDuplicates(*it, *iter);
    }
}

