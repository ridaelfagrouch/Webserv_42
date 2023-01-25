/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garra <garra@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 00:19:14 by garra             #+#    #+#             */
/*   Updated: 2023/01/25 02:02:22 by garra            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "websocket.hpp"

int main(int ac, char const *av[])
{
    (void)ac;
    (void)av;
    webSocket Socket;
    Socket.CreateSocket();
    Socket.bindSocket();
    Socket.listenSocket();
    while(1)
    {
        std::cout << "\n+++++++ Waiting for new connection ++++++++\n" << std::endl;
        Socket.acceptConnection();
    }
    return 0;
}