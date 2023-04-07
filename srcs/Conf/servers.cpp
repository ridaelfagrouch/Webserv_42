/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   servers.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sahafid <sahafid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 12:19:21 by sahafid           #+#    #+#             */
/*   Updated: 2023/04/07 02:55:05 by sahafid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/config.hpp"
#include "../../includes/response.hpp"

// class Response;


void    checkSemicolone(std::vector<std::string> &info)
{
    int semi = 0;
    if (info[info.size() - 1][info[info.size() - 1].size() -1] == ';')
    {
        for (std::vector<std::string>::iterator it = info.begin(); it != info.end(); it++)
        {
            std::string word = *it;
            for (size_t i =0; i < word.length(); i++)
                if (word[i] == ';')
                    semi++;
        }
    }
    else
        if (!(info[0]).empty())
            throw std::invalid_argument("Syntax Error: semicolone missing"); 
    if (semi > 1)
        throw std::invalid_argument("Syntax Error: extra semicolone");
}




void    Servers::checkHost(std::string info)
{
    int point = 0;
    std::string check;
    std::string host;
    if (info == "localhost")
    {
        this->host = "127.0.0.1";
        return ;
    }
    for (std::string::iterator it = info.begin(); it != info.end(); it++)
    {
        if (*it == '.')
        {
            int number = ft_stoi(check);
            if (number > 255 || number < 0)
                throw std::invalid_argument("Syntax Error: invalid host");
            point++;
            host += check;
            host += '.';
            check.clear();
        }
        else if (point <= 3 && isdigit(*it))
            check += *it;
        else
            throw std::invalid_argument("Syntax Error: invalid host");
    }
    if (point != 3)
        throw std::invalid_argument("Syntax Error: invalid host");
    
    int number = ft_stoi(check);
    if (number > 255 || number < 0)
        throw std::invalid_argument("Syntax Error: invalid host");
    
    host += check;
    this->host = host;
}

std::map<int, std::string>	initErrorMap()
{

    std::map<int, std::string> status_code;
    
	status_code[100] = "Continue";
	status_code[200] = "OK";
	status_code[201] = "Created";
	status_code[202] = "Accepted";
	status_code[204] = "No Content";
	status_code[300] = "Multiple Choices";
	status_code[301] = "Moved Permanently";
	status_code[302] = "Found";
	status_code[303] = "See Other";
	status_code[304] = "Not Modified";
	status_code[305] = "Use Proxy";
	status_code[307] = "Temporary Redirect";
	status_code[400] = "Bad Request";
	status_code[403] = "Forbidden";
	status_code[404] = "Not Found";
	status_code[405] = "Method Not Allowed";
	status_code[413] = "Payload Too Large";
	status_code[500] = "Internal Server Error";
	status_code[504] = "Gateway Timeout";
    
    return status_code;
}

std::string setErrorPage(std::string path, std::string status_code)
{
    std::ifstream file;
    file.open(path.c_str());

    std::string line;
    std::string lines;

    
    while (getline(file, line))
        lines.append(line + "\n");
    if (path == "./srcs/Conf/error/error.html")
    {

        int pos = lines.find("404");
        lines[pos] = status_code[0];
        lines[pos+1] = status_code[1];
        lines[pos+2] = status_code[2];

        std::map<int, std::string> status_codes_map = initErrorMap();
        if (status_codes_map.find(atoi(status_code.c_str())) != status_codes_map.end())
        {
            pos = lines.find("Not Found");
            int length = strlen("Not Found");
            std::string sub = lines.substr(0, pos);
            sub += status_codes_map.find(atoi(status_code.c_str()))->second;
            pos += length;
            sub += lines.substr(pos, lines.length());
            lines = sub;
        }
    }
    return lines;
}

int    allcodes(int code)
{
    std::vector<int> codes;

    // 400's error codes 

    for (int i = 400; i < 420; i++)
        codes.push_back(i);
    for (int i = 421; i < 429; i++)
        codes.push_back(i);

    codes.push_back(431);
    codes.push_back(449);
    codes.push_back(450);
    codes.push_back(451);
    codes.push_back(456);
    codes.push_back(444);
    codes.push_back(495);
    codes.push_back(496);
    codes.push_back(497);
    codes.push_back(498);
    codes.push_back(499);

    // 500's error codes 


    for (int i = 500; i < 512; i++)
        codes.push_back(i);

    
    for (int i = 520; i < 528; i++)
        codes.push_back(i);
        
    if (std::find(codes.begin(), codes.end(), code) != codes.end())
        return true;
    return false;
}

std::string	to_String(int n)
{
	std::stringstream tmp;

	tmp << n;

	return tmp.str();
}


void    Servers::errorPage(std::vector<std::string> info)
{
    std::vector<int> saved_codes;

    int status = ft_stoi(info[1]);


    if (allcodes(status))
    {
        saved_codes.push_back(status);
        errorPages current;
        std::ifstream file;
        current.status_code = status;
        if (info.size() == 3)
        {
            file.open(info[2].c_str());
            current.path = info[2];
            current.page = setErrorPage(current.path, to_String(status));
            if (!file)
            {
                current.path = "./srcs/Conf/error/error.html";
                current.page = setErrorPage(current.path, to_String(status));
            }
        } 
        else
        {
            current.path = "./srcs/Conf/error/error.html";
            current.page = setErrorPage(current.path, to_String(status));
        }
        this->error_page.push_back(current);
    }
    else
        throw std::invalid_argument("Syntax error: invalid status code");
}


void    Servers::check_syntaxError(std::vector<std::string> &server_info)
{
    server_info.erase(server_info.begin());        
    for (std::vector<std::string>::iterator it = server_info.begin(); it != server_info.end(); it++)
    {
        std::vector<std::string> info = split(*it, ' ');
        checkSemicolone(info);
        enterData(info);
    }
}