/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sahafid <sahafid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 15:11:58 by sahafid           #+#    #+#             */
/*   Updated: 2023/01/27 10:42:52 by sahafid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.hpp"



int main(int ac, char **av)
{
    try {
        Config conf;
        if (!av[1])
        {
            std::cerr << "file not found\n";
            return 0;
        }
        
	    conf.parse(av[1]);
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}