NAME		:=test
OBJDIR		:=.objs
OBJS		:=TestResponse.o Response.o

all			: $(OBJDIR) $(NAME)

$(OBJDIR)	:
	mkdir -p $@

$(NAME)		: $(OBJS)
	g++ -o $@ $^

%.o			: %c
	g++ -c $< -o $(OBJDIR)/$@

clean		:
	rm -rf (OBJDIR)

fclean		: clean
	rm $(NAME)

re			: fclean all

.PHONY		: all clean fclean re