# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jorge <jorge@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/06/03 14:32:42 by junghwle          #+#    #+#              #
#    Updated: 2024/06/24 17:05:19 by jorge            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			:=Webserv

SRCS			:=ConfigParser.cpp Location.cpp Logger.cpp main.cpp ServerConfigs.cpp WebServ.cpp \
				  HTTPRequest.cpp HttpRequestParser.cpp \
				  Response.cpp ResponseHeader.cpp GetResponse.cpp ErrorResponse.cpp
OBJDIR			:=.objs
OBJS			:=$(patsubst %.cpp, $(OBJDIR)/%.o, $(SRCS))
DEPS			:=$(OBJS:.o=.d)

INCLUDE			:=-I./Includes -I./HttpRequest -I./HttpResponse
CC				:=c++
CFLAGS			:=-Wall -Werror -Wextra -g
DEPFLAGS		:=-MMD
EXTRAFLAGS		:=-std=c++98
DEBUG			:=-fsanitize=address

all				: $(OBJDIR) $(NAME)

$(NAME)			: $(OBJS) Makefile
					$(CC) $(CFLAGS) $(EXTRAFLAGS) $(DEBUG) -o $@ $(OBJS)
					echo "(WebServ)COMPILING $@"

$(OBJDIR)/%.o	: Src/%.cpp Makefile 
					$(CC) $(DEPFLAGS) $(CFLAGS) $(EXTRAFLAGS) $(INCLUDE) $(DEBUG) -c -o $@ $<
					echo "(WebServ)COMPILING $@"

$(OBJDIR)/%.o	: HttpRequest/%.cpp Makefile 
					$(CC) $(DEPFLAGS) $(CFLAGS) $(EXTRAFLAGS) $(INCLUDE) $(DEBUG) -c -o $@ $<
					echo "(WebServ)COMPILING $@"

$(OBJDIR)/%.o	: HttpResponse/%.cpp Makefile 
					$(CC) $(DEPFLAGS) $(CFLAGS) $(EXTRAFLAGS) $(INCLUDE) $(DEBUG) -c -o $@ $<
					echo "(WebServ)COMPILING $@"

$(OBJDIR)		: Makefile
					mkdir -p $@

-include $(DEPS)

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

debug-leaks:
	valgrind -s --tool=memcheck --leak-check=full --track-origins=yes --show-leak-kinds=all ./$(NAME) config/test.conf

run:
	@  ./$(NAME) config/test.conf

git: fclean
	@read -p "Enter commit message: " commit_message; \
    git add *; \
    git commit -m "$$commit_message"
	git push

.PHONY: all clean fclean re debug-leaks run git
.SILENT:
