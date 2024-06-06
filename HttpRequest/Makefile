NAME			:=test
SRCS			:=main.cpp HttpRequest.cpp HttpRequestParser.cpp
OBJDIR			:=.objs
OBJS			:=$(patsubst %.cpp, $(OBJDIR)/%.o, $(SRCS))

all				: $(OBJDIR) $(NAME)

$(OBJDIR)		:
	mkdir -p $@

$(NAME)			: $(OBJS)
	g++ -o $@ $^

$(OBJDIR)/%.o	: %.cpp
	g++ -c $< -o $@ -g

clean			:
	rm -rf $(OBJDIR)

fclean			: clean
	rm -rf $(NAME)

re				: fclean all

.PHONY			: all clean fclean re