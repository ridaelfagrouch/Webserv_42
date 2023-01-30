/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sahafid <sahafid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 15:12:23 by sahafid           #+#    #+#             */
/*   Updated: 2023/01/25 22:51:07 by sahafid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.hpp"

void    check_brackets(std::vector<std::string> &lines)
{
    int braket = 0;
    for (std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); it++)
    {
        std::vector<std::string> line = split(*it, ' ');
        if (line[0] == "server")
        {
            if (line.size() > 2 && line[1] != "{")
                throw std::invalid_argument("Syntax Error");
            braket++;
        }
        else
        {
            for (std::vector<std::string>::iterator iter = line.begin(); iter != line.end(); iter++)
            {
                if (*iter == "{")
                {
                    if (*(iter - 2) == "location")
                        braket++;
                    else if (braket == 2 && *(iter - 2) == "limit_except")
                        braket++;
                }
                else if (*iter == "}")
                    braket--;
            }
        }
    }
    if (braket != 0)
        throw std::invalid_argument("Syntax Error: missing bracket");
}


void    split_servers(std::vector<std::string> &lines, std::vector<Servers> &servers)
{
    int braket = 0;
    std::vector<std::string> raw;
    for (std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); it++)
    {
        std::vector<std::string> line = split(*it, ' ');
        if (it == lines.begin())
            braket++;
        else
        {
            for (std::vector<std::string>::iterator iter = line.begin(); iter != line.end(); iter++)
            {
                if (*iter == "{")
                    braket++;
                else if (*iter == "}")
                    braket--;
            }
        }
        raw.push_back(*it);
        if (braket == 0)
        {
            Servers server;
            for (std::vector<std::string>::iterator itera = raw.begin(); itera != raw.end(); itera++)
            {
                trim(*itera, ' ');
                trim(*itera, 9);
                trim(*itera, '\n');
            }
            server.raw_server = raw;
            servers.push_back(server);
            raw.clear();
        }
    }
}

void    Config::parse_servers(Servers &server)
{
    server.split_locations(server.raw_server);
    server.client_max_body_size = -1;
    server.check_syntaxError(server.raw_server);
    check_syntax(server.locations);
}

void    Config::parse(std::string filename)
{
    std::ifstream fd;
    fd.open(filename);

    std::vector<std::string> lines;
    std::string line;
    if (!fd.is_open())
		throw std::invalid_argument("can't open file");
    while (getline(fd, line))
        if (!line.empty())
		    lines.push_back(line.append(" \n"));
    check_brackets(lines);
    split_servers(lines, servers);

    for (std::vector<Servers>::iterator it = servers.begin(); it != servers.end(); it++)
    {
        parse_servers(*it);
        checkDataValidity(*it);
    }
}