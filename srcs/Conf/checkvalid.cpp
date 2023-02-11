/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkvalid.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sahafid <sahafid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 19:43:04 by sahafid           #+#    #+#             */
/*   Updated: 2023/02/10 16:09:47 by sahafid          ###   ########.fr       */
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
            file.open((const char *)("/Users/sahafid/Desktop/webserv" + (*it).path).c_str());
            if (!file)
                (*it).path = "./error/";
        }
    }
}

void    checkDataValidity(Servers &server)
{
    std::vector<int> ports;
    if (server.port.size() < 1)
        throw std::invalid_argument("invalid input: no port number");
    if (server.host.empty())
        throw std::invalid_argument("invalid input: no host");
    for (std::vector<errorPages>::iterator it = server.error_page.begin(); it != server.error_page.end(); it++)
    {
        if (std::find(ports.begin(), ports.end(), (*it).status_code) != ports.end())
            throw std::invalid_argument("Syntax error: duplicate status code");
        ports.push_back((*it).status_code);
    }
    if (server.client_max_body_size == -1)
        server.client_max_body_size = 1;
    checkFile(server.error_page);
}