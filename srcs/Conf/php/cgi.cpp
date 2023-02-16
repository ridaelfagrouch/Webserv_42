/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sahafid <sahafid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/10 16:57:09 by sahafid           #+#    #+#             */
/*   Updated: 2023/02/16 12:42:36 by sahafid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../../includes/config.hpp"
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <fcntl.h>


// void  Servers::executeCgi(std::string fileName, Locations &location)
// {
    
//     std::ifstream check;

//     check.open(location.root + fileName);

//     if (!check.is_open())
//     {
//         std::cout << "no file found\n";
//         return ;
//     }
//     int fd = open("/tmp/tmpFile", O_WRONLY | O_TRUNC);
//     int pid = fork();
//     std::string cmd = location.fatscgi_pass;
//     if (pid == 0)
//     {
//         char *argv[1];
//         argv[0] = (char *)fileName.c_str();
//         argv[1] = NULL;
//         char **envp;
//         execve(cmd.c_str(), argv, envp);
//         std::cout << "ana hna\n";
//     }
    
    
// }


char    **setEnv()
{
    char **envirement = (char**)calloc(6 , sizeof(char*));
    
    std::string variable;
    variable = "CONTENT_LENGTH=";
    variable.append("16");
    envirement[0] = strdup(variable.c_str());
    
    variable = "CONTENT_TYPE=";
    variable.append("application/x-www-form-urlencoded");
    envirement[1] = strdup(variable.c_str());
    
    variable = "QUERY_STRING=";
    variable.append("");
    envirement[2] = strdup(variable.c_str());

    variable = "DOCUMENT_ROOT=";
    variable.append("");
    envirement[3] = strdup(variable.c_str());

    variable = "REQUEST_URI=";
    variable.append("");
    envirement[4] = strdup(variable.c_str());
    
    envirement[5] = NULL;
    return envirement;
}

void  executeCgi(std::string fileName)
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
        // dup2(fd, 1);
        close(fd);
        char *argv[3];
        argv[0] = (char*)cmd.c_str();
        argv[1] = (char *)fileName.c_str();
        argv[2] = NULL;
        char **envp = setEnv();
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


// int main()
// {
//     executeCgi("./file.php");
//     return 0;
// }
