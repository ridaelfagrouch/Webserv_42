/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseUtiles.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houazzan <houazzan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 21:34:49 by ouzhamza          #+#    #+#             */
/*   Updated: 2023/04/05 01:00:59 by houazzan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/response.hpp"


int	Response::ParsingResponse()
{
	_l = getLocation();
	if (_l == SIZE_MAX)
		return (deafIndex());
	else
	{
		changeRoot(); // ^ Root changing
 		if (redirection())
			return (_ret); // ^ Redirection
		if (!checkPath())
			return (403); //^ Forbidden
		if (!allowed())
			return(405); //^ METHODE NOT ALLOWED AND THE REPONCE SHOULD HAVE A HEADER OF ALLOWED METHODES FOR A REQUEST.
	}
	return (200);
}

/* ************************************************************************** */

int Response::allowed()
{
	if (!server.locations[_l].allow_methods.size() && _methode == "GET")
		return(1);
	for (size_t i = 0; i < server.locations[_l].allow_methods.size(); i++)
	{		if (server.locations[_l].allow_methods[i] == _methode)
			return(1);
	}
	return(0);
}

/* ************************************************************************** */

int Response::isFile()
{   
	struct stat st;
	if (!stat(server.root.c_str(), &st)){
		if (S_ISREG(st.st_mode))
			return (1);
		isAutoindex(_l);
	}
	return(0);
}

/* ************************************************************************** */

int Response::isIndex()
{
	std::cout << server.locations[_l].directive << std::endl;
	if(!server.locations[_l].index.empty()) {
		return(_index = server.locations[_l].index, 1);
	}
	else if (server.locations[_l].directive.compare("/") != 0) {
		return (0);
	}
	else if(!server.index.empty()) {
		std::cout << "FROM HERE 2" << server.index << std::endl;
		return(_index = server.index, 1);
	}
	else
		return(0);
	
}
/* ************************************************************************** */

int Response::checkPath()
{
	struct stat st;
	
	server.root.append(_pathExtra);
	if (stat(server.root.c_str(), &st) != 0)
		return (0);
	return (1);
}

void	Response::changeRoot()
{
	if (!server.locations[_l].root.empty() && 
	server.locations[_l].root.compare(server.root + "/"))
		server.root = server.locations[_l].root;
}