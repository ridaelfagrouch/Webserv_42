/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houazzan <houazzan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/11 11:19:03 by ouzhamza          #+#    #+#             */
/*   Updated: 2023/04/08 17:44:54 by houazzan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/response.hpp"


int Response::redirection()
{
    std::stringstream nb;
    size_t i;
    if (!server.locations[_l].returned.empty())
    {
        i = server.locations[_l].returned.find(" ");
        nb << server.locations[_l].returned.substr(0, i);
        nb >> _ret;
        return(1);
    }
    // std::cout << "here " << server.locations[_l].directive << std::endl;
    return(0);
}
