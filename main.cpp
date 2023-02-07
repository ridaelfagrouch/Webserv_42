/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rel-fagr <rel-fagr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/15 13:31:22 by rel-fagr          #+#    #+#             */
/*   Updated: 2023/01/15 13:54:31 by rel-fagr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include<iostream> 
#include<string.h> 
#include<sys/types.h> 
#include<netinet/in.h> 
#include<arpa/inet.h> 
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>

using namespace std

int TcpIpSocket(void)
{
    int client = socket(AF_INET, SOCK_STREAM, 0);

    if (client < 0) {
        perror("Error establishing connection");
        exit(1);
    }
    cout << "Server socket connection created" << endl
    return client;
}


int main(int ac, char **av, char **env)
{
    int client, server;
    int portNum = 1500;
    bool isExit = false;
    int bufsize = 1024;
    char buffer[bufsize]

    struct sockaddr_in server_addr;
    socklen_t size;

    client = TcpIpSocket(void)
    
}