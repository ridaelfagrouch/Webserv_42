/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sahafid <sahafid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 13:45:25 by sahafid           #+#    #+#             */
/*   Updated: 2023/01/27 10:45:58 by sahafid          ###   ########.fr       */
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


class Locations {
    public:
        std::vector<std::string> raw_location;


        std::string root;
        std::string directive;
        std::string returned;
        std::string autoindex;
        std::string upload_enable;
        std::string upload_store;
        std::vector<std::string> allow_methods;
        std::string fatscgi_pass;

        
        void    enterLocationData(std::vector<std::string> info);
        void    enterUploadEnab(std::vector<std::string> info);
        void    enterAllowedMethodes(std::vector<std::string> info);
        void    enterReturn(std::vector<std::string> info);
        void    enterIndex(std::vector<std::string> info);
};

class errorPages {
    public:
        int status_code;
        std::string path;
};



class Servers {
    public:

        std::vector<std::string> raw_server;

        std::string host;
        std::vector<int> port;
        std::vector<std::string> server_name;
        std::string root;
        std::string returned;
        long client_max_body_size;

        std::vector<Locations> locations;
        std::vector<errorPages> error_page;

        void    errorPage(std::vector<std::string> info);
        void    checkHost(std::string info);
        void    split_locations(std::vector<std::string> &server_info);
        void    check_syntaxError(std::vector<std::string> &server_info);
        void    enterData(std::vector<std::string> info);
        void    enterPorts(std::vector<std::string> info);
        void    enterBodySize(std::vector<std::string> info);
};


class Config {
    private:
        std::vector<Servers> servers;
    public :
        void    parse(std::string filename);
        void    parse_servers(Servers &server);
};

std::vector<std::string>    split(std::string s, char c);
std::string                 trim(std::string &s, char c);
int                         ft_stoi(std::string str);
void                        check_syntax(std::vector<Locations> &locations);
void                        checkSemicolone(std::vector<std::string> &info);
void                        checkDataValidity(Servers &server);