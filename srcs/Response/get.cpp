/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sahafid <sahafid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 09:31:18 by ouzhamza          #+#    #+#             */
/*   Updated: 2023/04/08 20:05:00 by sahafid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/response.hpp"

void Response::getMethode()
{
	if (_ret <= 301 || _ret >= 307)
	{
		std::cout << _ret << std::endl;
    	if (_ret != 200 || !readObject())
			error();
	}
}


/* ************************************************************************** */

int Response::readObject()
{
	if (_cgi)
		return (runcgi());
	else if (isFile())
		return(readFile());	
	else if (isIndex())
		return(readDefault());
	return (_ret = 404, 0);
}