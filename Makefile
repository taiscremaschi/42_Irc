NAME		= 		ircserv
SRCS		=		main.cpp Server.cpp Client.cpp IrcMessages.cpp Channel.cpp ServerManager.cpp MsgFormat.cpp
OBJS 		=		$(addprefix objs/, $(SRCS:.cpp=.o))
CXXFLAGS	=		-Wall -Wextra -Werror  -std=c++98 -g -Iincludes/
RM			=		rm -f

COLOUR_GREEN=\033[32m
COLOUR_RED=\033[31m
COLOUR_END=\033[0m
COLOUR_MAG=\001\e[0;35m\002

objs/%.o: */%.cpp
	@mkdir -p objs
	@c++ $(CXXFLAGS) -c $< -o $@

${NAME}: ${OBJS}
	@c++ $(CXXFLAGS) $(OBJS) -o $(NAME)
	@echo "$(COLOUR_MAG)\nNice! All files Compiled! $(COLOUR_GREEN)ᕦ$(COLOUR_RED)♥$(COLOUR_GREEN)_$(COLOUR_RED)♥$(COLOUR_GREEN)ᕤ\n$(COLOUR_END)"

all: ${NAME}

clean:
	@rm -f ${OBJS} ${OBJS_T}
	@echo "$(COLOUR_RED)Deleting all objs! ⌐(ಠ۾ಠ)¬\n$(COLOUR_END)"

fclean: clean
	@rm -f ${NAME}
	@clear
	
re:			fclean all

.PHONY: all clean fclean re 