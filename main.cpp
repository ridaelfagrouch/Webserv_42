/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garra <garra@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 15:11:58 by sahafid           #+#    #+#             */
/*   Updated: 2023/02/04 16:01:17 by garra            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/config.hpp"


int main(int ac, char **av)
{
    (void)ac;
    try {
        Config conf;
        if (!av[1])
        {
            std::cerr << "file not found\n";
            return 0;
        }
	    conf.parse(av[1]);
        webServer Server(conf.servers);
        Server.setupServer();
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}