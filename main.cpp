/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garra <garra@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 15:11:58 by sahafid           #+#    #+#             */
/*   Updated: 2023/01/31 11:57:20 by garra            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/config.hpp"


int main(int ac, char **av)
{
    (void)ac;
    try {
        Config conf;
        webSocket Server;
        if (!av[1])
        {
            std::cerr << "file not found\n";
            return 0;
        }
	    conf.parse(av[1]);
        Server.setupServer();
        Server.acceptConnection();
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}