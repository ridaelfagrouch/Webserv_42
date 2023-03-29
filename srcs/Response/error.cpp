/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ouzhamza <ouzhamza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 19:15:48 by ouzhamza          #+#    #+#             */
/*   Updated: 2023/03/27 01:14:24 by ouzhamza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/response.hpp"

void Response::error()
{
	for (size_t i = 0; i < server.error_page.size(); i++)
	{
		if (_ret == server.error_page[i].status_code)
				spError(server.error_page[i].path);
	}
	defError(); //^ return the default error page 
}

/* ************************************************************************** */



void Response::spError(std::string path)
{
	std::ifstream file;
	std::stringstream buff;
	
	file.open(path.c_str(), std::ifstream::in);
	if (file.is_open())
	{
		buff << file.rdbuf();
		_Resbody = buff.str();
	}
	defError(); //^ return the default error page 
}

/* ************************************************************************** */


void Response::defError()
{
	std::ifstream file;
	std::stringstream buff;
	std::string path = "./srcs/Conf/error/error.html";
	file.open(path.c_str(), std::ifstream::in);
	if (file.is_open())
	{	
		buff << file.rdbuf();
		_Resbody = buff.str();
		file.close();
	}
}