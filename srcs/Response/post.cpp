/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   post.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sahafid <sahafid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 09:32:36 by ouzhamza          #+#    #+#             */
/*   Updated: 2023/04/03 21:01:14 by sahafid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/response.hpp"

void Response::postMethode()
{
	// std::cout << "posting " << std::endl;
	_ret = 201;
	if (_ret <= 300 || _ret >= 307){
		if (!postObject())
			error();
	}
}

/* ************************************************************************** */

int Response::postObject()
{
	_ret = 204; //^ status code for not found
	if (_cgi && request.get_header("Content-Type").find("x-www-form-urlencoded") != std::string::npos)
	{
		_query = _Reqbody;
		return(runcgi());
	}
	else if (!upload_On() || _Reqbody.empty() || !upload_Store())
		return (0);
	tokenizing_Body();
	return (creat());
}

/* ************************************************************************** */

int Response::upload_On()
{
	if (server.locations[_l].upload_enable == "true")
		return(1);
	return (0);
}

/* ************************************************************************** */

int Response::upload_Store()
{
	if (server.locations[_l].upload_store.empty())
		return(0);
	server.root.append(server.locations[_l].upload_store);
	return(1);
}
/* ************************************************************************** */

void Response::tokenizing_Body()
{
	_postType = getPostType();
	get_Data(); 
}


std::string Response::getPostType()
{
	if (request.get_header("Content-Type").find("multipart") != std::string::npos)
		return ("form_data");
	else if (request.get_header("Content-Type").find("x-www-form-urlencoded") != std::string::npos)
		return ("x-www-form-urlencoded");
	return ("regular");
}

void Response::get_Data()
{
	if (!_postType.compare("form_data")){
		_bounadry = get_Boundary();
		form_Data();
	}
	else
		regularFile();
}

std::string Response::get_Boundary()
{
	std::stringstream s(_Reqbody);
	std::getline(s, _bounadry);
	_bounadry.erase(_bounadry.find("\r"));
	return (_bounadry);
}


void Response::form_Data()
{
	size_t pos;
	std::string token;
	std::string fileName;
	// int i = 0;
	// while (i < 2000)
	// {
	// 	std::cout << _Reqbody[i];
	// 	i++;
	// }
	
	// std::cout << _Reqbody << std::endl;
	while ((pos = _Reqbody.find(_bounadry)) != std::string::npos){
		if (pos == 0){
			_Reqbody.erase(0, _bounadry.length() + 2);
			// int i = 0;
			// while (i < 2000)
			// {
			// 	std::cout << _Reqbody[i];
			// 	i++;
			// }
		}
		else{
			token = _Reqbody.substr(0, pos);
			// std::cout << token << std::endl;
			if (token.find("Content-Type: ") != std::string::npos)
			{
				conType = token.substr(token.find("Content-Type: ") + 14);
				conType.erase(conType.find("\r\n\r\n"));
				token.erase(0,  token.find("\r\n\r\n") + 4);
				_postObject[form_Name()] = token;
				_Reqbody.erase(0, pos + _bounadry.length() + 2);
			}
			else {
				size_t pos = token.find("name") + sizeof("name=");
				size_t Spos = token.find("\r\n\r\n");
				fileName = token.substr(pos, Spos - pos - 1);
				token.erase(0,  Spos + 4);
				_postObject[fileName.append(".txt")] = token;
				conType = "text/plain";
				_Reqbody.erase(0, pos + _bounadry.length() + 2);
			}
		}
	}
}

std::string	Response::form_Name()
{
	size_t	pos;
	int		fiIndex = 1;
	std::string _postName;
	std::stringstream body(_Reqbody);
	getline(body, _postName);
	// std::cout << _postName << std::endl;
	if ((pos = _postName.find("filename=")) != std::string::npos){
		_postName = _postName.substr(pos + sizeof("filename="));
		if ((pos = _postName.find(";")) != std::string::npos)
			_postName.erase(pos - 1);
		else
			_postName.erase(_postName.length() - 2);
	}
	for (std::map<std::string, std::string>::iterator it = _postObject.begin(); it != _postObject.end(); it++){
		if (!it->first.compare(_postName)) {
			_postName.insert(_postName.find(".") , ("_" + to_String(fiIndex++)));
		} 	
	}
	return (_postName);

}


void	Response::regularFile()
{
	std::string file = "regular";
	if (!_postType.compare("x-www-form-urlencoded"))
		conType = "text/plain";
	else
		conType =  request.get_header("Content-Type");
	_postObject[file.append(".").append(getKey(conType))] = _Reqbody;
}

int	Response::creat()
{
	struct stat st;
	std::string file_path;
	std::string file;
	// for (std::map<std::string, std::string>::iterator it = _postObject.begin(); it != _postObject.end(); it++)
	// 	std::cout << it->first << " " << it->second << std::endl; //! printing the file content
	if (stat(server.root.c_str(), &st) || !valid_Type()){
		std::cout << valid_Type()  << std::endl;
		std::cout << server.root << std::endl;
		return (0);
	}
	for (std::map<std::string, std::string>::iterator it = _postObject.begin(); it != _postObject.end(); it++){
		file_path = server.root + "/" + it->first;
		std::ofstream file (file_path.c_str(), std::ios::binary);
		if (file.is_open())
			file << it->second << std::endl;
	}
	_Resbody = postAnswer(file_path);
	return (_ret = 201, 1);
}

int Response::valid_Type()
{
	std::cout << ":" << conType << ":" << std::endl;
	for(std::map<std::string, std::string>::iterator it = _contentType.begin(); it != _contentType.end(); it++){
		if (!conType.compare(it->first)){
			return (1);
		}
	}
	// std::cout << "the content type: " << conType << std::endl; //! printing the type of file uploaded
	return (0);
}


std::string Response::getKey(std::string value)
{
	for (std::map<std::string, std::string>::iterator it = _contentType.begin(); it != _contentType.end(); it++){
		if (!value.compare(it->first))
			return (it->second);
	}
	return ("unkown");
}


std::string Response::postAnswer(std::string path)
{
	std::string unsewer =\
	 "<!DOCTYPE html>\n\
    <html>\n\
    <head>\n\
	<meta charset=\"UTF-8\">\n\
            <title> File Created </title>\n\
    </head>\n\
    <body style=\"text-align:center;\">\n\
   <h1> File Created Successfully! </h1>\n\
   <pre style=\"text-align:center;\">\n\
    <p>Your file has been created in "+ path +" </p> \n\
	</pre>\n\
    </body>\n\
    </html>\n";
	return(unsewer);
}
