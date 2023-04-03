/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utiles.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sahafid <sahafid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 11:58:00 by ouzhamza          #+#    #+#             */
/*   Updated: 2023/04/03 02:48:15 by sahafid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/response.hpp"

std::string Response::get_Current_Time()
{
    char buffer[80];
    std::time_t now = std::time(nullptr);
    std::strftime(buffer, sizeof(buffer), "%a, %d %b %Y %T GMT", std::gmtime(&now));
    return (std::string(buffer) + "\r\n");
}

/* ************************************************************************** */

std::string Response::get_Connection()
{
    return (request.get_header("Connection") + "\r\n");
}

std::string Response::get_Content_Type()
{
    size_t i = _path.find(".");
    if (i == std::string::npos || _ret != 200 || _index)
        return ("text/html \r\n");
    else
        return(_contentType[_path.substr(i + 1)] + "\r\n");
}

/* ************************************************************************** */

std::string  Response::get_Content_Length()
{
    if(_ret == 504)
        return (to_String(_Resbody.length()) + "\r\n");
    return (to_String(_Resbody.length()) + "\r\n\r\n");
}

/* ************************************************************************** */

std::string Response::get_Rederiction()
{
    size_t i;
    i = server.locations[_l].returned.find(" ");
    return (server.locations[_l].returned.substr(i));
}

/* ************************************************************************** */

std::string Response::get_Allow_Methodes()
{
    std::string allowed;
    for (std::vector<std::string>::iterator it = server.locations[_l].allow_methods.begin(); \
        it != server.locations[_l].allow_methods.end(); it++) {
            allowed.append(" ").append(*it);
        }
    return (allowed.append("\r\n\r\n"));
}


/* ************************************************************************** */

std::string	Response::to_String(int n)
{
	std::stringstream tmp;

	tmp << n;

	return tmp.str();
}


std::string Response::getcgiheader()
{
    std::string line = "";
    for (std::vector<std::string>::iterator it = cgi_header.begin(); it != cgi_header.end(); it++)
	{
		std::vector<std::string> data = split(*it, ':');
    	if (data.size() == 2){
            
			if (data[0] == "Content-type")
				data[0] = "Content-Type";
            if (data[0] != "X-Powered-By")
                line += data[0] + ": " + data[1] + "\r\n";
                // std::cout << data[0] << std::endl;
		}
	}
    return line;
}