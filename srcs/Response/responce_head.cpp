/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responce_head.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ouzhamza <ouzhamza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 16:06:32 by ouzhamza          #+#    #+#             */
/*   Updated: 2023/03/27 15:28:47 by ouzhamza         ###   ########.fr       */
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
		_header["Connection"] += get_Current_Time();
		_header["Content-Type"] += get_Content_Type();
		_header["Content-Length"]  += get_Content_Length();
		
		_response.append(_header["Date"]).append(_header["Connection"]).append(_header["Content-Type"]).append\
		(_header["Content-Length"]).append(_Resbody);
	}
} 