/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rel-fagr <rel-fagr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/10 16:57:09 by sahafid           #+#    #+#             */
/*   Updated: 2023/02/15 20:07:23 by rel-fagr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../../includes/config.hpp"
#include <unistd.h>
#include <fstream>
#include <iostream>

// void  Servers::executeCgi(std::string fileName)
// {
//     std::ifstream test;
    
//     test.open("/tmp/tmpFile");
    
//     int fd = fileno(test);
    
//     std::ifstream file("example.txt");
//     if (file.is_open()) {
//         int fd = fileno(file);
//         std::cout << "File descriptor: " << fd << std::endl;
//     } else {
//         std::cout << "Could not open file" << std::endl;
//     }
// }