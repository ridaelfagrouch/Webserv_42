/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ouzhamza <ouzhamza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 19:15:48 by ouzhamza          #+#    #+#             */
/*   Updated: 2023/04/03 16:41:34 by ouzhamza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/response.hpp"

void Response::error()
{
	for (size_t i = 0; i < server.error_page.size(); i++)
	{
		if (_ret == server.error_page[i].status_code)
				spError(server.error_page[i].page);
	}
	defError(); //^ return the default error page 
}

/* ************************************************************************** */



void Response::spError(std::string page)
{
	_Resbody = page;
}

/* ************************************************************************** */


void Response::defError()
{
	_Resbody = setErrorPage("./srcs/Conf/error/error.html", to_String(_ret));
}