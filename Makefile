NAME            := Webserv

SRCS            := ConfigParser.cpp Location.cpp Logger.cpp main.cpp ServerConfigs.cpp WebServ.cpp \
					HTTPRequest.cpp HttpRequestParser.cpp \
					Response.cpp ResponseHeader.cpp CGIHandler.cpp
OBJDIR          := .objs
OBJS            := $(patsubst %.cpp, $(OBJDIR)/%.o, $(SRCS))
DEPS            := $(OBJS:.o=.d)

INCLUDE         := -I./Includes -I./HttpRequest -I./HttpResponse -I/usr/include/c++/10/ \
					-I/usr/include/c++/10/x86_64-pc-linux-gnu/ -I/usr/include/c++/10/backward/ \
					-I/usr/include/x86_64-linux-gnu/c++/10/ -I/usr/include/c++/10/bits/ \
					-I/usr/include/c++/10/ext/ -I/usr/include/c++/10/profile/ -I/usr/include/c++/10/debug/ \
					-I/usr/include/c++/10/debug/safe-string/ -I/usr/include/c++/10/debug/safe-iterator/ \
					-I/usr/include/c++/10/debug/safe-local-statics/ -I/usr/include/c++/10/debug/safe-include/ \
					-I/usr/include/c++/10/debug/safe-include/backward/ -I/usr/include/c++/10/debug/safe-include/ext/ \
					-I/usr/include/c++/10/debug/safe-include/profile/ -I/usr/include/c++/10/debug/safe-include/debug/ \
					-I/usr/include/c++/10/debug/safe-include/debug/safe-string/ \
					-I/usr/include/c++/10/debug/safe-include/debug/safe-iterator/ \
					-I/usr/include/c++/10/debug/safe-include/debug/safe-local-statics/ \
					-I/usr/include/c++/10/debug/safe-include/debug/safe-include/ \
					-I/usr/include/c++/10/debug/safe-include/debug/safe-include/backward/ \
					-I/usr/include/c++/10/debug/safe-include/debug/safe-include/ext/ \
					-I/usr/include/c++/10/debug/safe-include/debug/safe-include/profile/ \
					-I/usr/include/c++/10/debug/safe-include/debug/safe-include/debug/ \
					-I/usr/include/c++/10/debug/safe-include/debug/safe-include/debug/safe-string/ \
					-I/usr/include/c++/10/debug/safe-include/debug/safe-include/debug/safe-iterator/ \
					-I/usr/include/c++/10/debug/safe-include/debug/safe-include/debug/safe-local-statics/ \
					-I/usr/include/c++/10/debug/safe-include/debug/safe-include/debug/safe-include/ \
					-I/usr/include/c++/10/debug/safe-include/debug/safe-include/debug/safe-include/backward/ \
					-I/usr/include/c++/10/debug/safe-include/debug/safe-include/debug

# Adding the library path
LIB_PATH        := -L/usr/lib/gcc/x86_64-linux-gnu/10

CC              := c++
CFLAGS          := -Wall -Werror -Wextra -g
DEPFLAGS        := -MMD
EXTRAFLAGS      := -std=c++98
DEBUG           := -fsanitize=address

all             : $(OBJDIR) $(NAME)

$(NAME)         : $(OBJS) Makefile
					$(CC) $(CFLAGS) $(EXTRAFLAGS) $(DEBUG) $(LIB_PATH) -o $@ $(OBJS) -lstdc++
					echo "(WebServ)COMPILING $@"

$(OBJDIR)/%.o   : Src/%.cpp Makefile
					$(CC) $(DEPFLAGS) $(CFLAGS) $(EXTRAFLAGS) $(INCLUDE) $(DEBUG) -c -o $@ $<
					echo "(WebServ)COMPILING $@"

$(OBJDIR)/%.o   : HttpRequest/%.cpp Makefile
					$(CC) $(DEPFLAGS) $(CFLAGS) $(EXTRAFLAGS) $(INCLUDE) $(DEBUG) -c -o $@ $<
					echo "(WebServ)COMPILING $@"

$(OBJDIR)/%.o   : HttpResponse/%.cpp Makefile
					$(CC) $(DEPFLAGS) $(CFLAGS) $(EXTRAFLAGS) $(INCLUDE) $(DEBUG) -c -o $@ $<
					echo "(WebServ)COMPILING $@"

$(OBJDIR)       : Makefile
					mkdir -p $@

-include $(DEPS)

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

debug-leaks:
	valgrind -s --tool=memcheck --leak-check=full --track-origins=yes --show-leak-kinds=all ./$(NAME)

run:
	@  ./$(NAME) config/test.conf

git: fclean
	@read -p "Enter commit message: " commit_message; \
	git add *; \
	git commit -m "$$commit_message"
	git push

.PHONY: all clean fclean re debug-leaks run git
.SILENT: