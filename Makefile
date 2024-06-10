NAME = Webserv
CFLAG = -Wall -Werror -Wextra -g -std=c++98 #-fsanitize=address
SRC = Src/main.cpp Src/Logger.cpp Src/WebServ.cpp Src/ConfigParser.cpp Src/ServerConfigs.cpp Src/Location.cpp
CC = c++
OBJS = $(SRC:.cpp=.o)
HEADER = Includes/Logger.hpp Includes/WebServ.hpp Includes/ConfigParser.hpp Includes/ServerConfigs.hpp Includes/Location.hpp

debug-leaks:
	valgrind -s --tool=memcheck --leak-check=full --track-origins=yes ./$(NAME) /home/jbortolo/Desktop/42-webserver/config/test.conf

run:
	@./$(NAME) /home/jbortolo/Desktop/42-webserver/config/test.conf

%.o: %.cpp
	$(CC) -I. $(CFLAG) -c $< -o $@

$(NAME): $(OBJS) $(HEADER) Makefile
	$(CC) $(CFLAG) $(OBJS) -o $(NAME)

clean:
	@find . -name '*.o' -type f -delete

git: fclean
	@read -p "Enter commit message: " commit_message; \
    git add *; \
    git commit -m "$$commit_message"
	git push

fclean: clean
	rm -f $(NAME)

re: fclean $(NAME)

.PHONY: all re clean fclean
