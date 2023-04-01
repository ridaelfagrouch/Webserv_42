/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sahafid <sahafid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 09:31:18 by ouzhamza          #+#    #+#             */
/*   Updated: 2023/03/31 21:33:27 by sahafid          ###   ########.fr       */
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
	if (_cgi){
		return (runcgi());
	}
	else if (isFile())
		return(readFile());
	else if (_index)
        return (_Resbody = getIndex(), 200);
	else if (!server.index.empty())
		return (readDefault());
	return (_ret = 404, 0);
}