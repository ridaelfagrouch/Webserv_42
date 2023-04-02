/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responce_head.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sahafid <sahafid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 16:06:32 by ouzhamza          #+#    #+#             */
/*   Updated: 2023/04/02 21:40:33 by sahafid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/response.hpp"


void Response:: headerGen()
{
    _statusline = _version.append(to_String(_ret)).append(" ").append(_status_code[_ret]).append("\r\n");
    _response.append(_statusline);
    headMap();
	// reset(); //! to reset all data
}

/* ************************************************************************** */
void Response::headMap()
{
	
	if (_ret >= 300 &&  _ret <= 307)
	{
		_header["Location"].append(get_Rederiction());
		_response.append(_header["Location"]);
	}
	else
	{
		_header["Date"] += get_Current_Time();
		_header["Connection"] += get_Connection();
		_header["Content-Type"] += get_Content_Type();
		_header["Content-Length"]  += get_Content_Length();
		_header["Allow"] += get_Allow_Methodes();
		_header["cgi"] += getcgiheader();
		
		if (_cgi)
		{
			_response.append(_header["Date"]).append(_header["Connection"]).append(_header["cgi"]).append(_header["Content-Length"]).append(_Resbody);
		}
		else if (_ret == 405) {
			// std::cout << "hello" << std::endl;
		_response.append(_header["Date"]).append(_header["Connection"]).append(_header["Content-Type"]).append
		(_header["Content-Length"]).append(_header["Allow"].append(_Resbody));
		}
		else {
			_response.append(_header["Date"]).append(_header["Connection"]).append(_header["Content-Type"]).append\
			(_header["Content-Length"]).append(_Resbody);
			
		}

	}
} 