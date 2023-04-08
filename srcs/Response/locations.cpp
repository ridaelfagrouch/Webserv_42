/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   locations.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houazzan <houazzan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 15:37:20 by ouzhamza          #+#    #+#             */
/*   Updated: 2023/04/08 18:52:34 by houazzan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/response.hpp"

void Response::nextpath(std::string &path)
{
	size_t i;
	i = path.find_last_of('/');
	if (i == 0) {
		_pathExtra = path.substr(i) + _pathExtra;
		path = "/";
	}
	else {
		
		_pathExtra = path.substr(i) + _pathExtra;
		path = path.substr(0, i);
		_pathMAtch = _path.substr(0, i);
	}
}

size_t Response::getLocation()
{
	if ((_l = regularExpretion()) != std::string::npos)
		return (_l);
	else if ((_l = exactLocation()) != std::string::npos)
		return (_l);
	else if ((_l = prefixLocation()) != std::string::npos)
		return (_l);
	return(_l = rootLocation());
	return(0);
}

size_t Response::regularExpretion()
{
	std::string ending;
	size_t i = _path.find(".");
	size_t j;
	if (i != std::string::npos){
		ending = "*" + _path.substr(i);
	}
	for (size_t i = 0; i != server.locations.size(); i++){
		if (!ending.compare(server.locations[i].directive)){
			_cgi = true;
			j = prefixLocation();
			return (i);
		}
	}
	return (std::string::npos);
}

size_t Response::exactLocation()
{
	std::string path = _path;
	for (size_t i = 0; i != server.locations.size(); i++){
		if (!_path.compare(server.locations[i].directive))
		{
			_pathMAtch = server.locations[i].directive;
			return (i);
		}
	}
	return (std::string::npos);
}


size_t Response::prefixLocation()
{
	std::string path;
	path = _path;
	while (path != "/")
	{
		for (size_t i = 0; i != server.locations.size(); i++)
		{
			if (!server.locations[i].directive.compare(path))
			{
				_pathMAtch = path;
				return(i);
			}
		}
		nextpath(path);
	}
	return (std::string::npos);
}

size_t Response::rootLocation()
{
	for(size_t i = 0; i != server.locations.size(); i++){
		if(server.locations[i].directive == "/"){
			_pathMAtch = "/";
			return (i);
		}
	}
	return (std::string::npos);
}