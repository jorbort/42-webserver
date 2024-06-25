NAME			:=test
SRCS			:=TestResponse.cpp Response.cpp ResponseHeader.cpp HTTPRequest.cpp HttpRequestParser.cpp \
				  ErrorResponse.cpp GetResponse.cpp
OBJDIR			:=.objs
OBJS			:=$(patsubst %.cpp, $(OBJDIR)/%.o, $(SRCS))
CFLAG			:=-Wall -Werror -Wextra -g
EXTRAFLAG		:=-std=c++98
DEBUGFLAG		:=-fsanitize="leak, address"
INCLUDES		:=-I. -I../HttpRequest -I../Includes

all				: $(OBJDIR) $(NAME)

$(OBJDIR)		:
	mkdir -p $@

$(NAME)			: $(OBJS)
	g++ $(CFLAG) $(EXTRAFLAG) $(INCLUDES) $(DEBUG) -o $@ $^

$(OBJDIR)/%.o	: %.cpp
	g++ $(CFLAG) $(EXTRAFLAG) $(INCLUDES) -c $< -o $@

$(OBJDIR)/%.o	: ../HttpRequest/%.cpp
	g++ $(CFLAG) $(EXTRAFLAG) $(INCLUDES) -c $< -o $@

clean			:
	rm -rf $(OBJDIR)

fclean			: clean
	rm -rf $(NAME)

re				: fclean all

.PHONY			: all clean fclean re