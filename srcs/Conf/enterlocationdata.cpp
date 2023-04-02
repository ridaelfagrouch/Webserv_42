/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enterlocationdata.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sahafid <sahafid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 15:53:17 by sahafid           #+#    #+#             */
/*   Updated: 2023/04/02 22:03:30 by sahafid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/config.hpp"


void    Locations::enterUploadEnab(std::vector<std::string> &info)
{
    if (info.size() == 2)
    {
        if (info[1] == "on")
            this->upload_enable = "true";
        else if (info[1] == "off")
            this->upload_enable = "false";
        else
            throw std::invalid_argument("Syntax Error: unknow argument");  
    }
    else
        throw std::invalid_argument("Syntax Error: wrong number of arguments");
}


void    Locations::enterAllowedMethodes(std::vector<std::string> info)
{
    if (info.size() == 1)
        throw std::invalid_argument("Syntax Error: wrong number of arguments");
    for (std::vector<std::string>::iterator it = info.begin() + 1; it != info.end(); it++)
    {
        if (*it == "GET")
        {
            if (find(this->allow_methods.begin(), this->allow_methods.end(), *it) != this->allow_methods.end())
                throw std::invalid_argument("Syntax Error: duplicate methode");
            this->allow_methods.push_back(*it);
        }
        else if (*it == "POST")
        {
            if (find(this->allow_methods.begin(), this->allow_methods.end(), *it) != this->allow_methods.end())
                throw std::invalid_argument("Syntax Error: duplicate methode");
            this->allow_methods.push_back(*it);
        }
        else if (*it == "DELETE")
        {
            if (find(this->allow_methods.begin(), this->allow_methods.end(), *it) != this->allow_methods.end())
                throw std::invalid_argument("Syntax Error: duplicate methode");
            this->allow_methods.push_back(*it);
        }
        else
            throw std::invalid_argument("Syntax Error: unknow methode");
    }
}

int     redirectionCodes(std::string code)
{
    if (code == "300" || code == "301" || code == "302" || code == "303" || code == "304" ||code == "305" || code == "306" || code == "307" || code == "308" || code == "310")
        return 0;
    return 1;
}

void    Locations::enterReturn(std::vector<std::string> info)
{
    if (info.size() == 3)
    {
        if (redirectionCodes(info[1]))
            throw std::invalid_argument("Syntax Error: wrong status code number");
        this->returned.append(info[1] + " ");
        this->returned.append(info[2]);
    }
    else
        throw std::invalid_argument("Syntax Error: wrong number  of arguments");
}


void    Locations::enterIndex(std::vector<std::string> info)
{
    if (info[1] == "on")
        this->autoindex = "true";
    else if (info[1] == "off")
        this->autoindex = "false";
    else
        throw std::invalid_argument("Syntax Error: unknow argument");
}