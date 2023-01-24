/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   websocket.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garra <garra@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 03:30:01 by garra             #+#    #+#             */
/*   Updated: 2023/01/24 04:00:35 by garra            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <stdio.h>
# include <sys/socket.h>
# include <unistd.h>
# include <stdlib.h>
# include <netinet/in.h>
# include <string.h>

#define PORT 8080

class webSocket
{
private:
    struct sockaddr_in address;
    int addrlen;
 
public:
    int server_fd;
    int new_socket;
    void    CreateSocket();
    void    bindSocket();
    void    listenSocket();
    void    acceptConnection();
    webSocket();
    ~webSocket();
};

