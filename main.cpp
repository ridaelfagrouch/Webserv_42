/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garra <garra@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 00:19:14 by garra             #+#    #+#             */
/*   Updated: 2023/01/28 11:22:48 by garra            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "websocket.hpp"

int main(int ac, char const *av[])
{
    (void)ac;
    (void)av;
    webSocket Server;
    Server.setupServer();
    Server.acceptConnection();
    return 0;
}