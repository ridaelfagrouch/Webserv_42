/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houazzan <houazzan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 09:31:18 by ouzhamza          #+#    #+#             */
/*   Updated: 2023/04/06 01:33:49 by houazzan         ###   ########.fr       */
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
	else if (isIndex())
		return (readDefault());
	else if (_autoindex)
        return (_Resbody = getIndex(), 200);
	return (_ret = 404, 0);
}