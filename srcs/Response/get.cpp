/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ouzhamza <ouzhamza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 09:31:18 by ouzhamza          #+#    #+#             */
/*   Updated: 2023/04/03 02:43:43 by ouzhamza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/response.hpp"

void Response::getMethode()
{
	if (_ret <= 300 || _ret >= 307)
    	if (_ret != 200 || !readObject())
			error();
}


/* ************************************************************************** */

int Response::readObject()
{
	if (_cgi)
		return (runcgi());
	else if (isFile())
		return(readFile());
	else if (_index)
	{
		// std::cout << << std::endl;
        return (_Resbody = getIndex(), 200);
	}
	else if (!server.index.empty())
		return (readDefault());
	return (_ret = 404, 0);
}