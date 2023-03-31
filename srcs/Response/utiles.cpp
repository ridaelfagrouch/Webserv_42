/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utiles.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ouzhamza <ouzhamza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 11:58:00 by ouzhamza          #+#    #+#             */
/*   Updated: 2023/03/31 17:32:29 by ouzhamza         ###   ########.fr       */
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
