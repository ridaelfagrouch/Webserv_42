/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houazzan <houazzan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/01 10:41:11 by houazzan          #+#    #+#             */
/*   Updated: 2023/04/07 21:33:19 by houazzan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/request.hpp"
#include "../../includes/config.hpp"

Request::Request(const std::string& str) :
 _methode(""), _version(""), _body(""), _path(""), _port(80), _ret(200)
{
	// size_t i = 0;
	// while (i != 1000)
	// std::cout << str << std::endl;
	// std::cout << "########" << std::endl;
	tokenizingAndParsing(str);
}

/* *************************************************************************** */

Request::~Request(){}

/* *************************************************************************** */

int Request::get_ret()
{
	return (_ret);
}

int Request::get_port()
{
	return (_port);
}

std::string Request::get_methode()
{
	return(_methode);
}

std::string Request::get_version()
{
	return (_version);
}

std::string Request::get_body()
{
	return(_body);
}

std::string Request::get_path()
{
	return (_path);
}


std::string Request::get_query()
{
	return (_query);
}


std::string Request::get_header(std::string str)
{
	return (_header[str]);
}


