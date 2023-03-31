/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sahafid <sahafid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 00:52:47 by ouzhamza          #+#    #+#             */
/*   Updated: 2023/03/31 19:43:59 by sahafid          ###   ########.fr       */
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
#include <sys/stat.h>
#include <dirent.h>
#include <ctime>
#include "../includes/config.hpp"
#include "../includes/request.hpp"
#include "../includes/webserver.hpp"

class Response{
	private:
		std::string _response;
		std::string _path;
		std::string _pathMAtch;
		std::string _pathExtra;
		std::string	_version;
		std::string _query;
		std::string _host;
		std::string _methode;
		std::string _type;
		std::string _Reqbody;
		std::string	_Resbody;
		std::string _statusline;
		std::string _bounadry;
		std::string _postType;
		std::string  conType;
		std::string _root;
		int 		_ret;
		int			_redret;
		int			_port;
		bool        _index;
		bool		_cgi;
		size_t		_l; // ^ stands for location
		
		Request &request;
		fds_info &fd;
		Servers &server;

		
		std::map<int, std::string> _status_code;
		std::map<std::string, std::string>_header;
		std::map<std::string, std::string> _contentType;
		std::map<std::string, std::string> _postObject;
		std::map<std::string, std::string> _Query;
		
		// std::map<std::string, std::string (*)(const std::string&)>_methodes;
	public:
		Response(Request &_request, fds_info &_fd);
		Response();
		~Response();


		//^ Datainit functions
		void				initErrorMap();
		void				initRespMaps();
		void				initContentMap();
		void				setBool();
		void				setPath();
		void				setVersion();
		void				setQuery();
		void				setRet();
		void				setMethode();
		int					isAbsoluteURI();
		void				changeHost();
		// void				setserver();
		// void				setHost();
		void				setPort();
		void				setBody();
		std::string			decodePath();
		std::string			get_key(std::string line);
		std::string			get_value(std::string line);

		
		int					ParsingResponse();
		size_t 				getLocation();
		size_t				regularExpretion();
		size_t				exactLocation();
		size_t				prefixLocation();
		void				nextpath(std::string &path);
		size_t				rootLocation();
		int 				redirection();
		void				changeRoot();
		int					deafIndex();
		int					allowed();
		int					checkPath();
		int					is_CGI();

		
		std::string			call();
		
		//! FUNCTIONS FOR CGI
		int					runcgi();
		void				fillClass();

		// void				tokenizeQuery();
		// std::string			setName(size_t i);
		// std::string			setValue(size_t i);

		//! FUNCTONS FOR GET METHODE
		void				getMethode();
		void 				isAutoindex(size_t i);
		int 				isFile();
		int					readObject();
		int					readFile();
		int					readDefault();
		std::string			getIndex();
		std::string			dirLink(struct dirent *dirname);
		std::string			getLastReadTime(struct dirent *dirName);
		std::string			getSize(struct dirent *dirName);
		


		//! FUNCTIONS FOR POST METHODE
		void				postMethode();
		void				tokenizing_Body();
		int					postObject();
		int					upload_On();
		int					upload_Store();
		std::string			get_Boundary();
		std::string			form_Name();
		std::string			getPostType();
		std::string			getKey(std::string value);
		void				get_Data();
		void				form_Data();
		void				regularFile();
		int					valid_Type();
		int					creat();
		std::string			postAnswer(std::string path);
		

		//! FUNCTIONS FOR DELETE METHODE
		void				deleteMethode();
		int					deleteObject();
		std::string			deleteAnswer();


		
		void				headerGen();
		void				headMap();
		
		// ^ ERROR functions
		void 				error();
		void				defError();
		void				spError(std::string path);
		
		// ^ HEADER GENERATING
		std::string			get_Current_Time();
		std::string			get_Connection();
		std::string			get_Content_Type();
		std::string			get_Content_Length();
		std::string			get_Rederiction();

		
		std::string			to_String(int n);

			class Cgi
		{
    		private:
    		    std::string Methode;
    		    std::string Path;
    		    std::string Query;
    		    std::string root;
    		    std::string redirectStatus;
    		    std::string contentLength;
    		    std::string contentType;
    		    std::string serverName;
    		    std::string serverProtocol;
				std::string port;
    		public:
				// void		setCgiserverProtocol(std::string protocol);
				void		setCgicontentType(std::string port);
				void		setCgicontentLength(std::string length);
				void		setCgiPath(std::string path);
				void		setCgiQuery(std::string query);
				void		setCgiredirectStatus(int redirect);
				void		setCgiroot(std::string root);
				void		setCgiServerName(std::string sserverName);
				void		setCgiMethode(std::string methode);
				void		setCgiserverProtocol();
				void		setCgiPort(int port);


				std::string		getCgicontentType();
				std::string		getCgicontentLength();
				std::string		getCgiPath();
				std::string		getCgiQuery();
				std::string		getCgiredirectStatus();
				std::string		getCgiroot();
				std::string		getCgiServerName();
				std::string		getCgiMethode();
				std::string		getCgiserverProtocol();
				std::string		getCgiPort();

    		    Cgi();
    		    ~Cgi();
		};

		std::string     executeCgi(std::string filename, Cgi cgi);

};
