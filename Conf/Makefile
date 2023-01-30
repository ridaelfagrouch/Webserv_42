# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sahafid <sahafid@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/19 13:44:53 by sahafid           #+#    #+#              #
#    Updated: 2023/01/26 15:55:02 by sahafid          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

FILES = main.cpp parsing.cpp utils.cpp servers.cpp location.cpp checkvalid.cpp enterlocationdata.cpp enterserverdata.cpp
OFILES = $(FILES:.cpp=.o)
HEADERS = config.hpp

CC = c++
NAME = webserv
FLAGS = -std=c++98

all : $(NAME)

$(NAME) : $(OFILES) 
	@$(CC) $(FLAGS) $(OFILES) -o $@
	@./webserv config.conf

%.o:%.cpp $(HEADERS)
	@$(CC) $(FLAGS) -c $< -o $@ 

clean:
	@rm -rf $(OFILES)

fclean: clean
	@rm -rf $(NAME)

re: fclean all