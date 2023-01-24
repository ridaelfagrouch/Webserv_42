/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garra <garra@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 00:19:14 by garra             #+#    #+#             */
/*   Updated: 2023/01/24 04:08:44 by garra            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "websocket.hpp"

int main(int argc, char const *argv[])
{
    (void)argc;
    (void)argv;
    // long valread;
    webSocket Socket;

    char hello[] = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
    Socket.CreateSocket();
    Socket.bindSocket();
    Socket.listenSocket();
    while(1)
    {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        Socket.acceptConnection();
        
        char buffer[30000] = {0};
        read(Socket.new_socket , buffer, 30000);
        printf("%s\n",buffer );
        write(Socket.new_socket , hello , strlen(hello));
        printf("-----------------message sent-------------------");
        close(Socket.new_socket);
    }
    return 0;
}