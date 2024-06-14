NAME			:=test
SRCS			:=TestResponse.cpp Response.cpp ResponseHeader.cpp
CFLAG			:=-Wall -Werror -Wextra -g
EXTRAFLAG		:=-std=c++98
DEBUGFLAG		:=-fsanitize="leak, address"
OBJDIR			:=.objs
OBJS			:=$(patsubst %.cpp, $(OBJDIR)/%.o, $(SRCS))

all				: $(OBJDIR) $(NAME)

$(OBJDIR)		:
	mkdir -p $@

$(NAME)			: $(OBJS)
	g++ $(CFLAG) $(EXTRAFLAG) $(DEBUG) -o $@ $^

$(OBJDIR)/%.o	: %.cpp
	g++ $(CFLAG) $(EXTRAFLAG) -c $< -o $@

clean			:
	rm -rf $(OBJDIR)

fclean			: clean
	rm -rf $(NAME)

re				: fclean all

.PHONY			: all clean fclean re