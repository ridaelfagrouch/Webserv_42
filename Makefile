# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: garra <garra@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/24 01:46:16 by garra             #+#    #+#              #
#    Updated: 2023/01/24 04:07:00 by garra            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CFILES = main.cpp websocket.cpp
OFILES = ${CFILES:.cpp=.opp}

CXX = c++
CXXFLAGS = -Wall -Werror -Wextra -std=c++98
NAME = webServer
HEADER = websocket.hpp

all : ${OFILES}
	@${CXX} ${CXXFLAGS} ${CFILES} -o ${NAME}

%.opp:%.cpp ${websocket}
	@${CXX} ${CXXFLAGS} -o $@ -c $<

clean:
	@rm -f ${OFILES} ${OBONUS}

fclean : clean
	@rm -f ${NAME}

re : fclean all
