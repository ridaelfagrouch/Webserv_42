/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sahafid <sahafid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 16:34:43 by sahafid           #+#    #+#             */
/*   Updated: 2023/01/26 15:54:25 by sahafid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.hpp"


void    Servers::split_locations(std::vector<std::string> &server_info)
{
    for (std::vector<std::string>::iterator it = server_info.begin(); it != server_info.end(); it++)
    {
        Locations location;
        int braket = 0;
        std::vector<std::string> info = split(*it, ' ');
        if (info[0] == "location")
        {
            if (info.size() != 3 || (info[1][0] != '/' && info[1] != "*.php") || info[2] != "{")
                throw std::invalid_argument("Syntax Error: invalid location");
            braket++;
            std::vector<std::string> rawlocation;
            rawlocation.push_back(info[1]);
            it = server_info.erase(it);
            while (it != server_info.end() && (*it)[0] != '}')
            {
                if (!((*it).empty()))
                    rawlocation.push_back(*it);
                it = server_info.erase(it);
            }
            location.raw_location = rawlocation;
            locations.push_back(location);
        }
        if ((*it)[0] == '}')
            *it = "";
    }
}

void    Locations::enterLocationData(std::vector<std::string> info)
{
    if (info[info.size() -1].empty())
        info.erase(info.end()-1);
    if (info.size() > 1)
    {
        if (info[0] == "uplaod_enable")
            enterUploadEnab(info);
        else if (info[0] == "upload_store")
            this->upload_store = info[1];
        else if (info[0] == "allow_methods")
            enterAllowedMethodes(info);
        else if (info[0] == "root")
        {
            if (info.size() == 2)
                this->root = info[1];
            else
                throw std::invalid_argument("Syntax Error: wrong number of arguments"); 
        }
        else if (info[0] == "return")
            enterReturn(info);
        else if (info[0] == "autoindex")
            enterIndex(info);
        else if (info[0] == "fastcgi_pass")
        {
            if (info.size() == 2)
                this->fatscgi_pass = info[1];
            else
                throw std::invalid_argument("Syntax Error: wrong number of arguments"); 
        }
    }
    else
        throw std::invalid_argument("Syntax Error: wrong number of arguments"); 
}

void    check_syntax(std::vector<Locations> &locations)
{
    for (std::vector<Locations>::iterator it = locations.begin(); it != locations.end(); it++)
    {
        Locations &currentDirective = *it;
        std::vector<std::string> location = currentDirective.raw_location;
        (*it).directive = location[0];
        if (location.size() < 2)
            throw std::invalid_argument("Syntax Error: Location block empty");
        for (std::vector<std::string>::iterator iter = location.begin() + 1; iter != location.end(); iter++)
        {
            std::vector<std::string> info = split(*iter, ' ');
            checkSemicolone(info);
            trim(info[info.size() - 1], ';');
            currentDirective.enterLocationData(info);
        }
    }
}

