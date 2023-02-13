# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sahafid <sahafid@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/30 13:27:48 by garra             #+#    #+#              #
#    Updated: 2023/02/10 17:00:55 by sahafid          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# --------------------------- Terminal Color Codes --------------------------- #
NNN := \033[0m
RED := \033[31m
YEL := \033[33m
GRA := \033[37m
CYN := \033[36m
GRN := \033[32m
MGN := \033[35m
BLU := \033[34m
Black:= \033[0;30m
White:= \033[0;37m
DARKVIOLET=\033[1;5;1;36m


define TITLE

		██╗    ██╗███████╗██████╗     ███████╗███████╗██████╗ ██╗   ██╗███████╗██████╗ 
		██║    ██║██╔════╝██╔══██╗    ██╔════╝██╔════╝██╔══██╗██║   ██║██╔════╝██╔══██╗
		██║ █╗ ██║█████╗  ██████╔╝    ███████╗█████╗  ██████╔╝██║   ██║█████╗  ██████╔╝
		██║███╗██║██╔══╝  ██╔══██╗    ╚════██║██╔══╝  ██╔══██╗╚██╗ ██╔╝██╔══╝  ██╔══██╗
		╚███╔███╔╝███████╗██████╔╝    ███████║███████╗██║  ██║ ╚████╔╝ ███████╗██║  ██║
		 ╚══╝╚══╝ ╚══════╝╚═════╝     ╚══════╝╚══════╝╚═╝  ╚═╝  ╚═══╝  ╚══════╝╚═╝  ╚═╝
                            \t\t\t- Created By ${BLU}${NNN}\n
endef
export TITLE


SRCS = main.cpp $(wildcard ./srcs/Conf/*.cpp) $(wildcard ./srcs/server/*.cpp) $(wildcard ./srcs/Conf/php/*.cpp)
OBJS	= $(SRCS:.cpp=.o)

CXXFLAGS = -I includes/ -Wall -Wextra -Werror
CXX = c++
NAME = webserver



all: $(NAME)

$(NAME): $(OBJS)
	@$(CXX) $(CXXFLAGS) $^ -o $(NAME)

title:
	echo "${DARKVIOLET}$$TITLE${NC}"

%.o: %.cpp 
	@$(CXX) $(CXXFLAGS) -c $< -o $@
# @echo "${GRN}=> $@${NC}"

clean:
	@rm -rf $(OBJS)

fclean: clean
	@rm $(NAME)

re: fclean all

.PHONY: all clean fclean re title 