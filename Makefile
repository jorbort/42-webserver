NAME = Webserv
CFLAG = -Wall -Werror -Wextra -g -std=c++98 ##-fsanitize=address
SRC = Src/main.cpp Src/Logger.cpp
CC = c++
OBJS = $(SRC:.cpp=.o)
HEADER = Includes/Logger.hpp Includes/WebServ.hpp

%.o: %.cpp
	$(CC) -I. $(CFLAG) -c $< -o $@

$(NAME): $(OBJS) $(HEADER) Makefile
	$(CC) $(CFLAG) $(OBJS) -o $(NAME)

clean:
	rm -f *.o

git: fclean
	git add *
	git commit -m "auto commit"
	git push

fclean: clean
	rm -f $(NAME)

re: fclean $(NAME)

.PHONY: all re clean fclean
