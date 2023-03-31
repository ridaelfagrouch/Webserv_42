/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sahafid <sahafid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/10 16:57:09 by sahafid           #+#    #+#             */
/*   Updated: 2023/03/31 16:30:03 by sahafid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../../includes/config.hpp"
#include "../../../includes/response.hpp"
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <fcntl.h>

// "REDIRECT_STATUS","DOCUMENT_ROOT", "SERVER_SOFTWARE"
//     ,"SERVER_PORT","GATEWAY_INTERFACE"
//     ,"SERVER_NAME","SCRIPT_FILENAME"
//     ,"REQUEST_METHOD", "SERVER_PROTOCOL"
//     , "CONTENT_TYPE", "CONTENT_LENGTH"
//     , "PATH_INFO", "QUERY_STRING"
//     , "HTTP_COOKIE" 



char    **setEnv(Response::Cgi cgi, std::string fileName)
{
    char **envirement = (char**)calloc(13 , sizeof(char*));
    
    std::string variable;
    variable = "CONTENT_LENGTH=";
    variable.append(cgi.getCgicontentLength());
    envirement[0] = strdup(variable.c_str());
    
    variable = "CONTENT_TYPE=";
    variable.append(cgi.getCgicontentType());
    envirement[1] = strdup(variable.c_str());
    
    variable = "QUERY_STRING=";
    variable.append(cgi.getCgiQuery());
    envirement[2] = strdup(variable.c_str());  

    variable = "DOCUMENT_ROOT=";
    variable.append(cgi.getCgiroot());
    envirement[3] = strdup(variable.c_str());

    
    variable = "PATH_INFO=";
    variable.append(cgi.getCgiPath());
    envirement[4] = strdup(variable.c_str());

    variable = "REQUEST_METHOD=";
    variable.append(cgi.getCgiMethode());
    envirement[5] = strdup(variable.c_str());
    
    variable = "SERVER_PORT=";
    variable.append(cgi.getCgiPort());
    envirement[6] = strdup(variable.c_str());

    variable = "SERVER_NAME=";
    variable.append(cgi.getCgiServerName());
    envirement[7] = strdup(variable.c_str());

    variable = "REDIRECT_STATUS=";
    variable.append(cgi.getCgiredirectStatus());
    envirement[8] = strdup(variable.c_str());


    variable = "SERVER_PROTOCOL=";
    variable.append(cgi.getCgiserverProtocol());
    envirement[9] = strdup(variable.c_str());


    variable = "HTTP_COOKIE=";
    variable.append("");
    envirement[10] = strdup(variable.c_str());

    variable = "SCRIPT_FILENAME=";
    variable.append(fileName);
    envirement[11] = strdup(variable.c_str());
    
    envirement[12] = NULL;
    return envirement;
}

std::string  executeCgiPhp(std::string fileName, Response::Cgi cgi)
{
    
    std::ifstream check;
    check.open(fileName);
    int out_fd = dup(1);
    if (!check.is_open())
    {
        std::cout << "no file found\n";
        return "";
    }
    std::string cmd = "/Users/sahafid/Desktop/webserv_copy/srcs/Conf/php/php-cgi";
    remove("./tmpFile");
    int fd = open("./tmpFile", O_CREAT | O_RDWR | O_TRUNC);

    int pid = fork();
    if (pid == 0)
    {
        std::cout << fileName << " " << cgi.getCgiQuery() << std::endl;
        dup2(fd, 1);
        close(fd);
        char *argv[3];
        argv[0] = strdup((char*)cmd.c_str());
        argv[1] = strdup((char *)fileName.c_str());
        argv[2] = NULL;

        char **envp = setEnv(cgi, fileName);
        execve(argv[0], argv, envp);
        exit(0);
    }
    waitpid(-1, NULL, 0);
    close(fd);
    dup2(out_fd, 1);

    std::ifstream test;

    test.open("./tmpFile");


    std::string tmp;
    std::string lines;
    
    while(getline(test, tmp))
    {
        lines.append("\n");
        lines.append(tmp);
    }
    remove("./tmpFile");
    std::cout << "ready to exit" << std::endl;
    return lines;
}



std::string   executeCgiPy(std::string fileName, Response::Cgi cgi)
{
    
    std::ifstream check;

    check.open(fileName);
    int out_fd = dup(1);
    if (!check.is_open())
    {
        std::cout << "no file found\n";
        return "";
    }
    std::string cmd = "/usr/bin/python3";
    remove("./tmpFile");
    int fd = open("./tmpFile", O_CREAT | O_RDWR | O_TRUNC);
    int pid = fork();
    if (pid == 0)
    {
        dup2(fd, 1);
        close(fd);
        char *argv[3];
        argv[0] = (char *)cmd.c_str();
        argv[1] = (char *)fileName.c_str();
        argv[2] = NULL;
        char **envp = setEnv(cgi, fileName);
        execve(argv[0], argv, envp);
        exit(0);
    }
    waitpid(-1, NULL, 0);
    close(fd);
    dup2(out_fd, 1);

    std::ifstream test;

    test.open("./tmpFile");
    
    std::string tmp;
    std::string lines;
    

    while(getline(test, tmp))
    {
        lines.append("\n");
        lines.append(tmp);
    }
    remove("./tmpFile");
    
    return lines;
}


std::string    Response::executeCgi(std::string filename, Cgi cgi)
{
    int position = filename.find_last_of(".");
    if(filename.substr(position+1) == "py")
        return executeCgiPy(filename, cgi);
    else
        return executeCgiPhp(filename, cgi);
    
}
