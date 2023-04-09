/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reading.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sahafid <sahafid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 02:28:13 by ouzhamza          #+#    #+#             */
/*   Updated: 2023/04/08 20:04:22 by sahafid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/response.hpp"


int Response::readFile()
{
    std::ifstream file;
	std::stringstream buff;
    file.open(server.root.c_str(), std::ifstream::in);
	if (file.is_open())
	{	
		buff << file.rdbuf();
		_Resbody = buff.str();
		file.close();
		return (_ret = 200, 1);
	}
	return(_ret = 403, 0); // Forbidden 
}

int Response::readDefault()
{
    std::ifstream file;
	std::stringstream buff;
    std::string path;

	_index = "/" + _index;

    path = _root + _pathExtra + _index;
	if (iscgi(_index))
		return (runcgi());
    file.open(path.c_str(), std::ifstream::in);
	if (file.is_open())
	{	
		buff << file.rdbuf();
		_Resbody = buff.str();
		file.close();
		return (_ret = 200, 1);
    }
	if (_autoindex)
		return (_Resbody = getIndex(), 200);
    return (_ret = 404, 0);
}







