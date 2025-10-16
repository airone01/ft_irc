# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/08 15:49:16 by elagouch          #+#    #+#              #
#    Updated: 2025/10/16 19:07:59 by elagouch         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# **************************************************************************** #
#                                   COMMANDS                                   #
# **************************************************************************** #

CXX				?=	c++
ECHO			:=	printf
VALGRIND	?=	valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --trace-children=yes --trace-children-skip=/bin/*,/usr/bin/*

BOLD			:=	$(shell tput bold)
RESET			:=	$(shell tput sgr0)
RED				:=	$(shell tput setab 1)
GREEN			:=	$(shell tput setab 2)
BLUE			:=	$(shell tput setab 4)
FGGRAY			:=	$(shell tput setaf 244)
# GREEN			:=	$(shell tput setab 2)
# GREEN			:=	$(shell tput setab 2)
# GREEN			:=	$(shell tput setab 2)

# **************************************************************************** #
#                                    FLAGS                                     #
# **************************************************************************** #

# == mandatory ==
CXXFLAGS	+=	-Wall -Werror -Wextra --std=c++98
# == code quality ==
CXXFLAGS	+=	-Wpedantic		# increas portability
CXXFLAGS	+=	-Wconversion	# warns against implicit coversion
CXXFLAGS	+=	-Wformat -Wformat=2 -Werror=format-security
														# harden against string vulns
CXXFLAGS	+=	-Wshadow			# warns against shadowing (common error)
CXXFLAGS	+=	-Wnon-virtual-dtor
														# warns if a class with virtual functions lacks a
														# virtual destructor (basically when you don't
														# actually know virtuality)
CXXFLAGS	+=	-Wold-style-cast
														# prefers CPP-style rather than C-style casts
CXXFLAGS	+=	-Wnull-dereference -Wunused -Wuninitialized
														# code hygiene
# == compiler additions ==
CXXFLAGS	+=	-MMD -MP			# deps
CXXFLAGS	+=	-D_GLIBCXX_ASSERTIONS
														# additional runtime checks for STL
# CXXFLAGS	+=	-fstrict-flex-arrays=3
# 														# ensures safe usage of flexible array members
# == security ==
CXXFLAGS	+=	-U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=3
														# better runtime checks for buffer overflows and such
CXXFLAGS	+=	-fstack-clash-protection -fstack-protector-strong
														# protects against buffer overflows and such attacks
CXXFLAGS	+=	-Wl,-z,noexecstack
														# prevents execution of code on the stack
CXXFLAGS	+=	-Wl,-z,nodlopen
														# restricts runtime libs loaing to known safe code

# == prod ==
CXXFLAGS	+=	-O2						# slower comp, faster runtime

# **************************************************************************** #
#                                     SRC                                      #
# **************************************************************************** #

NAME			:=		ft_irc
SRC_O			?=		main Logger
SRC				:=		$(addprefix src/, $(addsuffix .cpp, $(SRC_O)))
OBJ				:=		$(SRC:.cpp=.o)
DEPS			:=		$(SRC:.cpp=.d)
RM_LIST		:=		"$(OBJ) $(SRC:.cpp=.d)"

# **************************************************************************** #
#                                   TARGETS                                    #
# **************************************************************************** #

all: title $(NAME)

$(NAME): $(OBJ)
	@$(ECHO) "$(GREEN)$(BOLD) CC $(RESET)$(FGGRAY) $(NAME)$(RESET)\n"
	@$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJ)

%.o: %.cpp Makefile
	@$(ECHO) "$(BLUE)$(BOLD) CC $(RESET)$(FGGRAY) $@$(RESET)\n"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@$(ECHO) "$(RED)$(BOLD) RM $(RESET)$(FGGRAY) $(shell echo $(OBJ) $(SRC:.cpp=.d) | cut -c1-40)...$(RESET)\n"
	@$(RM) $(OBJ) $(SRC:.cpp=.d)

fclean: clean
	@$(ECHO) "$(RED)$(BOLD) RM $(RESET)$(FGGRAY) $(NAME)$(RESET)\n"
	@$(RM) $(NAME)

title:
	@$(ECHO) "\033[1;30;48;5;181m \033[0m\033[1;30;48;5;217m \033[0m\033[1;30;48;5;217m \033[0m\033[1;30;48;5;180m_\033[0m\033[1;30;48;5;180m_\033[0m\033[1;30;48;5;180m_\033[0m\033[1;30;48;5;180m_\033[0m\033[1;30;48;5;180m_\033[0m\033[1;30;48;5;150m \033[0m\033[1;30;48;5;150m \033[0m\033[1;30;48;5;151m \033[0m\033[1;30;48;5;115m \033[0m\033[1;30;48;5;115m \033[0m\033[1;30;48;5;116m_\033[0m\033[1;30;48;5;116m \033[0m\033[1;30;48;5;116m \033[0m\033[1;30;48;5;116m \033[0m\033[1;30;48;5;117m \033[0m\033[1;30;48;5;147m \033[0m\033[1;30;48;5;147m \033[0m\033[1;30;48;5;147m \033[0m\033[1;30;48;5;182m\`\033[0m\n"
	@$(ECHO) "\033[1;30;48;5;217m \033[0m\033[1;30;48;5;217m \033[0m\033[1;30;48;5;180m/\033[0m\033[1;30;48;5;180m \033[0m\033[1;30;48;5;180m_\033[0m\033[1;30;48;5;180m/\033[0m\033[1;30;48;5;180m \033[0m\033[1;30;48;5;150m/\033[0m\033[1;30;48;5;150m_\033[0m\033[1;30;48;5;151m \033[0m\033[1;30;48;5;115m \033[0m\033[1;30;48;5;115m \033[0m\033[1;30;48;5;116m(\033[0m\033[1;30;48;5;116m_\033[0m\033[1;30;48;5;116m)\033[0m\033[1;30;48;5;116m_\033[0m\033[1;30;48;5;117m_\033[0m\033[1;30;48;5;147m_\033[0m\033[1;30;48;5;147m_\033[0m\033[1;30;48;5;147m_\033[0m\033[1;30;48;5;182m_\033[0m\033[1;30;48;5;182m_\033[0m Made by:\n"
	@$(ECHO) "\033[1;30;48;5;217m \033[0m\033[1;30;48;5;180m/\033[0m\033[1;30;48;5;180m \033[0m\033[1;30;48;5;180m_\033[0m\033[1;30;48;5;180m/\033[0m\033[1;30;48;5;180m \033[0m\033[1;30;48;5;150m_\033[0m\033[1;30;48;5;150m_\033[0m\033[1;30;48;5;151m/\033[0m\033[1;30;48;5;115m \033[0m\033[1;30;48;5;115m \033[0m\033[1;30;48;5;116m/\033[0m\033[1;30;48;5;116m \033[0m\033[1;30;48;5;116m/\033[0m\033[1;30;48;5;116m \033[0m\033[1;30;48;5;117m_\033[0m\033[1;30;48;5;147m_\033[0m\033[1;30;48;5;147m/\033[0m\033[1;30;48;5;147m \033[0m\033[1;30;48;5;182m_\033[0m\033[1;30;48;5;182m_\033[0m\033[1;30;48;5;182m/\033[0m $(BOLD)nahamida$(RESET), $(BOLD)maximart$(RESET), $(BOLD)elagouch$(RESET).\n"
	@$(ECHO) "\033[1;30;48;5;180m/\033[0m\033[1;30;48;5;180m_\033[0m\033[1;30;48;5;180m/\033[0m\033[1;30;48;5;180m \033[0m\033[1;30;48;5;180m\\\\\033[0m\033[1;30;48;5;150m_\033[0m\033[1;30;48;5;150m_\033[0m\033[1;30;48;5;151m/\033[0m\033[1;30;48;5;115m_\033[0m\033[1;30;48;5;115m_\033[0m\033[1;30;48;5;116m/\033[0m\033[1;30;48;5;116m_\033[0m\033[1;30;48;5;116m/\033[0m\033[1;30;48;5;116m_\033[0m\033[1;30;48;5;117m/\033[0m\033[1;30;48;5;147m \033[0m\033[1;30;48;5;147m \033[0m\033[1;30;48;5;147m\\\\\033[0m\033[1;30;48;5;182m_\033[0m\033[1;30;48;5;182m_\033[0m\033[1;30;48;5;182m/\033[0m\033[1;30;48;5;182m\`\033[0m\n"
	@$(ECHO) "\033[1;30;48;5;180m \033[0m\033[1;30;48;5;180m \033[0m\033[1;30;48;5;180m \033[0m\033[1;30;48;5;180m \033[0m\033[1;30;48;5;150m \033[0m\033[1;30;48;5;150m \033[0m\033[1;30;48;5;151m/\033[0m\033[1;30;48;5;115m_\033[0m\033[1;30;48;5;115m_\033[0m\033[1;30;48;5;116m_\033[0m\033[1;30;48;5;116m/\033[0m\033[1;30;48;5;116m \033[0m\033[1;30;48;5;116m \033[0m\033[1;30;48;5;117m \033[0m\033[1;30;48;5;147m \033[0m\033[1;30;48;5;147m \033[0m\033[1;30;48;5;147m \033[0m\033[1;30;48;5;182m \033[0m\033[1;30;48;5;182m \033[0m\033[1;30;48;5;182m \033[0m\033[1;30;48;5;182m \033[0m\033[1;30;48;5;181m\`\033[0m\n"
	@$(ECHO) "\n"

run: all
	@$(VALGRIND) ./$(NAME)

re: fclean all

-include $(DEPS)

MAKEFLAGS	+= --no-print-directory
.PHONY: all clean fclean re

