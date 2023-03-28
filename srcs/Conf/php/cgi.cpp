/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sahafid <sahafid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/10 16:57:09 by sahafid           #+#    #+#             */
/*   Updated: 2023/03/28 00:48:52 by sahafid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../../includes/config.hpp"
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



char    **setEnv(std::string query, std::string methode, std::string pathinfo)
{
    char **envirement = (char**)calloc(8 , sizeof(char*));
    
    std::string variable;
    variable = "CONTENT_LENGTH=";
    variable.append("16");
    envirement[0] = strdup(variable.c_str());
    
    variable = "CONTENT_TYPE=";
    variable.append("application/x-www-form-urlencoded");
    envirement[1] = strdup(variable.c_str());
    
    variable = "QUERY_STRING=";
    variable.append(query);
    envirement[2] = strdup(variable.c_str());  

    variable = "DOCUMENT_ROOT=";
    variable.append("");
    envirement[3] = strdup(variable.c_str());

    variable = "REQUEST_URI=";
    variable.append("");
    envirement[4] = strdup(variable.c_str());
    
    variable = "PATH_INFO=";
    variable.append(pathinfo);
    envirement[5] = strdup(variable.c_str());

    variable = "REQUEST_METHOD=";
    variable.append(methode);
    envirement[6] = strdup(variable.c_str());
    
    variable = "SERVER_PORT=";
    variable.append("");
    envirement[7] = strdup(variable.c_str());

    variable = "SERVER_NAME=";
    variable.append("");
    envirement[8] = strdup(variable.c_str());

    variable = "SCRIPT_FILENAME=";
    variable.append("");
    envirement[9] = strdup(variable.c_str());


    variable = "SERVER_PROTOCOL=";
    variable.append("");
    envirement[10] = strdup(variable.c_str());


    variable = "HTTP_COOKIE=";
    variable.append("");
    envirement[11] = strdup(variable.c_str());

    
    envirement[7] = NULL;
    return envirement;
}

void  executeCgiPhp(std::string fileName, std::string query, std::string methode, std::string pathinfo)
{
    
    std::ifstream check;

    check.open(fileName);
    int out_fd = dup(1);
    if (!check.is_open())
    {
        std::cout << "no file found\n";
        return ;
    }
    std::string cmd = "./php-cgi";
    remove("./tmpFile");
    int fd = open("./tmpFile", O_CREAT | O_RDWR | O_TRUNC);
    int pid = fork();
    if (pid == 0)
    {
        dup2(fd, 1);
        close(fd);
        char *argv[3];
        argv[0] = (char*)cmd.c_str();
        argv[1] = (char *)fileName.c_str();
        argv[2] = NULL;
        char **envp = setEnv(query, methode, pathinfo);
        execve(argv[0], argv, envp);
        exit(0);
    }
    waitpid(-1, NULL, 0);
    close(fd);
    dup2(out_fd, 1);

    std::ifstream test;

    test.open("./tmpFile");
    std::string line;
    
    getline(test, line);
    
    std::cout << line;
}



void  executeCgiPy(std::string fileName, std::string query, std::string methode, std::string pathinfo)
{
    
    std::ifstream check;

    check.open(fileName);
    int out_fd = dup(1);
    if (!check.is_open())
    {
        std::cout << "no file found\n";
        return ;
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
        char **envp = setEnv(query, methode, pathinfo);
        execve(argv[0], argv, envp);
        exit(0);
    }
    waitpid(-1, NULL, 0);
    close(fd);
    dup2(out_fd, 1);

    std::ifstream test;

    test.open("./tmpFile");
    std::string line;
    
    getline(test, line);
    
    std::cout << line;
}


void    executeCgi(std::string filename)
{
    int position = filename.find_last_of(".");
    (void)position;
    // if(filename.substr(position+1) == "py")
    //     executeCgiPy(filename);
    // else
    //     executeCgiPhp(filename);
}


// int main()
// {
//     // executeCgi("./file.php");
//     return 0;
// }
