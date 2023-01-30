/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enterserverdata.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sahafid <sahafid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 15:55:13 by sahafid           #+#    #+#             */
/*   Updated: 2023/01/27 11:12:12 by sahafid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.hpp"


void    Servers::enterPorts(std::vector<std::string> info)
{
    for (std::vector<std::string>::iterator it = info.begin() + 1; it != info.end(); it++)
    {
        int port = ft_stoi(*it);
        if (port < 0)
            throw std::invalid_argument("Syntax Error: invalid port number");
        else
        {
            if (std::find(this->port.begin(), this->port.end(), port) != this->port.end())
                throw std::invalid_argument("Syntax Error: duplicate port number");
            this->port.push_back(port);
        }
    }
}



void    Servers::enterBodySize(std::vector<std::string> info)
{
    if (info.size() != 3)
    {
        std::string sizeBody = info[1];
        //  A value of '0' which means no limit and this is not recommended in production environment
        if (sizeBody.size() == 2 && sizeBody[0] != 0)
            throw std::invalid_argument("invalid max body size");
        char unit = tolower(sizeBody[sizeBody.size() - 1]);
        if (sizeBody.size() > 2 &&  (unit != 'm' && unit != 'k'))
            throw std::invalid_argument("invalid max body size");
        if (unit == 'k')
        {
            trim(sizeBody, 'k');
            client_max_body_size = ft_stoi(sizeBody);
            if (client_max_body_size < 0)
                throw std::invalid_argument("invalid max body size");
            client_max_body_size = client_max_body_size / 1000;                
        }
        else
        {
            trim(sizeBody, 'm');
            client_max_body_size = ft_stoi(sizeBody);
            if (client_max_body_size < 0)
                throw std::invalid_argument("invalid max body size");
        }
    }
    else
        throw std::invalid_argument("Syntax Error: wrong number of arguments");
}



void    Servers::enterData(std::vector<std::string> info)
{
    trim(info[info.size() - 1], ';');
    if (info[info.size() - 1].empty())
        info.erase(info.end()-1);
    if (info.size() > 1 && info[0] == "listen")
        enterPorts(info);
    else if (info.size() > 2 && info[0] == "server_name")
        for (std::vector<std::string>::iterator iter = info.begin() + 1; iter != info.end(); iter++)
            this->server_name.push_back(*iter);
    else if (info.size() > 1 && info[0] == "error_page")
    {
        if (info.size() == 3 || info.size() == 2)
            errorPage(info);
        else
            throw std::invalid_argument("wrong number of arguments");
    }
    else if (info.size() > 1 && info[0] == "client_max_body_size")
        enterBodySize(info);
    else if (info.size() > 1 && info[0] == "host")
    {
        if (info.size() == 2)
            checkHost(info[1]);
        else
            throw std::invalid_argument("Syntax Error: wrong number of arguments"); 
    }
    else if (info.size() > 1 && info[0] == "root")
    {
        if (info.size() == 2)
            this->root = info[1];
        else
            throw std::invalid_argument("Syntax Error: wrong number of arguments"); 
    }
    else
        if (!info[0].empty())
            throw std::invalid_argument("Syntax Error: unknow argument"); 
}