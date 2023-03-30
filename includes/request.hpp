/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ouzhamza <ouzhamza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 18:49:20 by houazzan          #+#    #+#             */
/*   Updated: 2023/03/30 17:53:56 by ouzhamza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include "fcntl.h"
#include <vector>
#include <fstream>
#include <string>
#include <map>
#include <exception>
#include <sstream>
#include "../includes/config.hpp"

class Request{

	private :
		std::string _methode;
		std::string _version;
		std::string _body;
		std::string _path;
		std::string _query;
		int 		_port;
		int			_ret;
		
		std::map<std::string, std::string> _header;
		std::vector<std::string> _methods;
		
		// ! parsing functions
		void 			tokenizingAndParsing(const std::string &str);
		int				parseFirstline(const std::string &line);
		int				set_methode(const std::string &line);
		int				set_path(const std::string &line);
		int				set_version(const std::string &line);
		int				checkHost();
		std::string		getLine(const std::string &str, size_t &i);
		std::string		set_key(const std::string &str);
		std::string		set_value(const std::string &str);
		std::string		extractPort();
		void			refinement();
		void			set_port();
		void			set_body(const std::string &str, size_t i);
		
		// ! initiation of data
		void	initMethodes();
	public:
		Request();
		Request(const std::string& str);
		~Request();


		int 			get_ret();
		int 			get_port();
		std::string 	get_methode();
		std::string 	get_version();
		std::string 	get_body();
		std::string 	get_path();
		std::string 	get_query();
		std::string		get_header(std::string str);
};
