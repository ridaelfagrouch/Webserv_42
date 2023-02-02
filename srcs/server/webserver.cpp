/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garra <garra@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 04:00:17 by garra             #+#    #+#             */
/*   Updated: 2023/02/02 21:33:52 by garra            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/webserver.hpp"

int SetNonBlocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1)
      return -1;
    flags |= O_NONBLOCK;
    int ret = fcntl(fd, F_SETFL, flags);
    if (ret == -1)
      return -1;
    return 0;
}


void    webServer::setupServer()
{
    for (size_t i = 0; i < servers.size(); i++)
    {   
        for(size_t j = 0; j < servers[i].port.size(); j++)
        {
            int	opt(1);
            int server_fd = this->guard(socket(AF_INET, SOCK_STREAM, 0), "create TCP socket error");
            this->guard(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)), "Failed to set socket option 1");

            
            struct sockaddr_in  address;
        
            address.sin_family = AF_INET;
            memset(address.sin_zero, '\0', sizeof(address.sin_zero));
            address.sin_addr.s_addr = inet_addr(servers[i].host.c_str());
            address.sin_port = htons(servers[i].port[j]);
            
            guard(bind(server_fd, (struct sockaddr *)&address, sizeof(address)), "bind error");
            guard(listen(server_fd, MAX_CONNECTIONS), "listen error");
            // guard(SetNonBlocking(server_fd), "Failed to set listen socket as non-blocking");
            fcntl(server_fd,  F_SETFL, O_NONBLOCK);
            
            pollfd  tmpfd;
            tmpfd.fd = server_fd;
            tmpfd.events = POLLIN;
            tmpfd.revents = 0;
            this->fds.push_back(tmpfd);
        }
    }
    acceptConnection();
}


//--------------------------------------------------------------------------

void    webServer::acceptConnection(void)
{
    while (true) {
        int nfds = poll(&fds[0], fds.size(), TIMEOUT);
        if (nfds < 0)
        {
            perror("Error in poll()");
            break;
        }
        for (size_t i = 0; i < fds.size(); i++)
        {
            if (fds[i].fd == -1)
                continue;
            if (fds[i].events & POLLIN)
            {
	            struct sockaddr_in	client_addr;
	            int	len(sizeof(client_addr));
                int client_socket = accept(fds[i].fd, (struct sockaddr *)&client_addr, (socklen_t*)&len);
                if (client_socket < 0)
                {
                    std::cerr << "Failed to accept client connection\n";
                    continue;
                }
                fcntl(client_socket,  F_SETFL, O_NONBLOCK);
                char buffer[30000] = {0};
                int n = recv(client_socket, buffer, 1024, 0);
                if (n < 0)
                {
                    std::cerr << "Failed to receive data from client\n";
                    close(client_socket);
                    continue;
                }
        
                std::cout << "Received data: " << buffer << std::endl;
                close(client_socket);
            }
        }
    }
    for (size_t i = 0; i < fds.size(); i++)
        close(fds[i].fd);
}

//--------------------------------------------------------------------------

int webServer::guard(int n, const char *er)
{
    if (n < 0)
    {
        perror(er);
        exit(EXIT_FAILURE);
    }
    return n;
}

//--------------------------------------------------------------------------

webServer::webServer(std::vector<Servers> servers)
{
    this->servers = servers;
}

webServer::~webServer(){}

//--------------------------------------------------------------------------

int webServer::sendall(int s, const char *buf, int len)
{
    int total = 0; // how many bytes we’ve sent
    int bytesleft = len;
    int n;
    while(total < len)
    {
        n = send(s, buf+total, bytesleft, 0);
        if (n == -1)
            break;
        total += n;
        bytesleft -= n;
    }
    // *len = total; 
    return n==-1?-1:0;
}

//--------------------------------------------------------------------------

// void removeClosedSockets(std::vector<int> &client_sockets, const std::vector<int> &closed_sockets) {
//     for (unsigned int i = 0; i < closed_sockets.size(); i++) {
//         int closed_socket = closed_sockets[i];
//         client_sockets.erase(std::remove(client_sockets.begin(), client_sockets.end(), closed_socket), client_sockets.end());
//     }
// }