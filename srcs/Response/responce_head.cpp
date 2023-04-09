/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responce_head.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houazzan <houazzan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 16:06:32 by ouzhamza          #+#    #+#             */
/*   Updated: 2023/04/08 23:28:32 by houazzan         ###   ########.fr       */
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
		_header["Cache-Control"] += get_cash_control();
		_header["Content-Length"]  += "0";
		_response.append(_header["Location"]).append(_header["Cache-Control"]).append(_header["Content-Length"].append("\r\n\r\n"));
		
	}
	else
	{
		_header["Date"] += get_Current_Time();
		_header["Connection"] += get_Connection();
		_header["Content-Type"] += get_Content_Type();
		_header["Content-Length"]  += get_Content_Length();
		_header["Allow"] += get_Allow_Methodes();
		_header["cgi"] += getcgiheader();
		_header["Cache-Control"] += get_cash_control();
		
		if (_cgi)
		{
			_response.append(_header["Date"]).append(_header["Connection"]).append(_header["cgi"]).append(_header["Cache-Control"]).append(_header["Content-Length"]).append(_Resbody);
		}
		else if (_ret == 405) {
			
		_response.append(_header["Date"]).append(_header["Connection"]).append(_header["Content-Type"]).append \
		(_header["Content-Length"]).append(_header["Cache-Control"]).append(_header["Allow"].append(_Resbody));
		}
		else {
			_response.append(_header["Date"]).append(_header["Connection"]).append(_header["Content-Type"])\
			.append(_header["Cache-Control"]).append(_header["Content-Length"]).append(_Resbody);
			
		}

	}
} 