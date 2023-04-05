/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houazzan <houazzan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 09:33:04 by ouzhamza          #+#    #+#             */
/*   Updated: 2023/04/05 18:29:12 by houazzan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/response.hpp"


void Response::deleteMethode()
{
    if (_ret <= 300 || _ret >= 307){
		if (!deleteObject())
			error();
	}
}


int Response::deleteObject()
{
    if ((_path.find("upload") == std::string::npos) || _path.find("..") != std::string::npos)
        return(_ret = 204, 0);
    else if (remove(server.root.c_str())!= 0)
        return(_ret = 204, 0);
    _Resbody = deleteAnswer();
    return (1);
}


std::string Response::deleteAnswer()
{
	std::string unsewer =\
	 "<!DOCTYPE html>\n\
    <html>\n\
    <head>\n\
	<meta charset=\"UTF-8\">\n\
            <title> File Removed </title>\n\
    </head>\n\
    <body style=\"text-align:center;\">\n\
   <h1> File Removed Successfully! </h1>\n\
   <pre style=\"text-align:center;\">\n\
    <p>The file has been removed from </p> \n\
	</pre>\n\
    </body>\n\
    </html>\n";
	return(unsewer);
}

