/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sahafid <sahafid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 15:11:58 by sahafid           #+#    #+#             */
/*   Updated: 2023/04/05 20:30:38 by sahafid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/config.hpp"

int main(int ac, char **av)
{
    try {
        Config conf;
        if (ac == 1)
            conf.parse("./srcs/Conf/default.conf");
        else
        {
            if (ac != 2)
            {
                std::cerr << "to many arguments\n";
                return 0;
            }
            if (!av[1])
            {
                std::cerr << "file not found\n";
                return 0;
            }
	        conf.parse(av[1]);
        }
        webServer Server(conf.servers);
        Server.fileExemple = av[2];
        Server.setupServer();
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}