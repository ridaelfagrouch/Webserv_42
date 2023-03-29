/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utiles.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ouzhamza <ouzhamza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 11:58:00 by ouzhamza          #+#    #+#             */
/*   Updated: 2023/03/23 01:16:49 by ouzhamza         ###   ########.fr       */
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

std::string	Response::to_String(int n)
{
	std::stringstream tmp;

	tmp << n;

	return tmp.str();
}
