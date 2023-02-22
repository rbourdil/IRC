NAME = parser

OBJS = main.o parser.o lexer.o token.o 

HEADS = parser.hpp lexer.hpp scanner.hpp token.hpp

CC = c++

FLAGS = -Wall -Wextra -Werror -std=c++98

all:		$(NAME)

$(NAME):	$(OBJS)
			$(CC) $(FLAGS) $(OBJS) -o $(NAME)

$(OBJS):	%.o:%.cpp $(HEADS)
			$(CC) $(FLAGS) -c  $< -o $@

clean:
			rm -f $(OBJS)

fclean:		clean
			rm -f $(NAME)

re:			fclean all

.PHONY:		all clean fclean re
